#include "renderer.h"

#include "emojis.h"
#include "utils.h"

#include <cctype>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
using namespace std;

namespace Gumsmaq {
#define CONTINUE          \
	if (it == end) break; \
	else                  \
		continue
vector_t VectorFromAbbr(const wstring& str) {
	vector_t   ret;
	wstring	   buf;
	const auto end = str.end();

	for (auto it = str.begin(); it != end;) {
		while (it != end && !isdigit(*it)) {
			if (!isspace(*it)) buf += *it;
			it++;
		}
		if (it == end) throw std::runtime_error("Invalid Gumsmaq: missing number");

		if (isdigit(*it)) {
			wstring num;
			do num += *it++;
			while (it != end && isdigit(*it));
			ret.emplace_back(buf, wcstol(num.c_str(), nullptr, 10));
			buf.clear();
			CONTINUE;
		}
	}
	return ret;
}

wstring VectorToUnicode(const vector_t& gumsmaq) {
	wstring out;
	for (const auto& g : gumsmaq) repeat(g.second) out += abbr_gumsmaq_table.at(g.first);
	return out;
}

const image& LoadLetter(const wstring& which) {
	static unordered_map<wstring, image> cached_letters;
#ifdef WIN32
	static const string img_root = "twemoji\\assets\\72x72\\";
#else
	static const string img_root = "twemoji/assets/72x72/";
#endif
	if (!cached_letters.contains(which)) {
		if (!abbr_gumsmaq_table.contains(which)) {
			wcerr << L"Error: '" << which << L"' is not valid gumsmaq. Aborted";
			exit(1);
		}

		stringstream stream;
		stream << hex << (int) abbr_gumsmaq_table.at(which);
		string img_path = img_root + stream.str() + ".png";
		image  letter(img_path);
		cached_letters.insert({which, move(letter)});
	}
	return cached_letters.at(which);
}

image ArrangeGroup(const letter_group_t& lg, int start_index, int letter_count) {
	image img{letter_count * GUMSMAQ_LOGOGRAM_WIDTH + (letter_count - 1) * gumsmaq_letter_kern,
		letter_count == 1 ? GUMSMAQ_LOGOGRAM_HEIGHT : GUMSMAQ_LINE_HEIGHT};
	int	  x = 0, y = 0, stride = GUMSMAQ_LOGOGRAM_WIDTH + gumsmaq_letter_kern;
	bool  lower = true;
	for (int i = start_index; i < start_index + letter_count; i++) {
		auto l = LoadLetter(lg.first);
		img.WriteAtIfAlpha(x, y, l);
		y	  = lower ? GUMSMAQ_LOGOGRAM_HEIGHT / 2 : 0;
		lower = !lower;
		x += stride;
	}
	return img;
}

#define max_count gumsmaq_max_letter_group_count
image LetterGroup(const letter_group_t& group, bool indented) {
	image		  img;
	int			  first_count = indented ? max_count - 1 : max_count;
	vector<image> lines;
	if (group.second > first_count) {
		int	  remaining = group.second - first_count;
		image first		= ArrangeGroup(group, 0, first_count);

		int count		 = max_count - 1;
		int count_groups = remaining / count;

		int start_index = first_count;
		for (int i = 0; i < count_groups; i++) {
			start_index += count;
			lines.push_back(ArrangeGroup(group, start_index, count));
		}

		image last;
		if (remaining % count)
			last = ArrangeGroup(group, start_index + count, remaining % count);

		int ht = GUMSMAQ_LINE_HEIGHT;
		ht += GUMSMAQ_LINE_HEIGHT * lines.size();
		if (remaining % count) ht += GUMSMAQ_LINE_HEIGHT;

		img = image{GUMSMAQ_LINE_WIDTH, ht};

		img.WriteAtIfAlpha(indented ? gumsmaq_group_indent : 0, 0, first);
		int y = GUMSMAQ_LINE_HEIGHT;
		for (const auto& line : lines) {
			img.WriteAtIfAlpha(gumsmaq_group_indent, y, line);
			y += GUMSMAQ_LINE_HEIGHT;
		}
		if (remaining % count > 0)
			img.WriteAtIfAlpha(gumsmaq_group_indent, y, last);
	} else {
		image line = ArrangeGroup(group, 0, group.second);
		img		   = image{GUMSMAQ_LINE_WIDTH, GUMSMAQ_LINE_HEIGHT};
		img.WriteAtIfAlpha(indented ? gumsmaq_group_indent : 0, 0, line);
	}
	return img;
}

image Paragraph(const vector_t& groups) {
	int					  remaining_lines = gumsmaq_max_line_count;
	vector<vector<image>> blocks;
	vector<image>		  current_block;
	for (const auto& group : groups) {
		if (group.second < 1) {
			wcerr << "Warning: group '" << group.first << to_wstring(group.second)
				  << "' is empty and was skipped\n";
			continue;
		}
		if (remaining_lines <= 0) {
			blocks.push_back(current_block);
			current_block.clear();
			remaining_lines = 10;
		}
		int chars_available = !remaining_lines ? 0 : remaining_lines * (max_count - 1);
		if (chars_available + 1 >= group.second) {
			/// this part is the worst...
			int line_diff = group.second / (max_count - 1);
			if (group.second == 1) line_diff = 1;
			else if (int mod = group.second - 1; group.second % 9 && mod > 0 && mod % 9)
				line_diff++;
			remaining_lines -= line_diff;

			current_block.push_back(LetterGroup(group));
		} else {
			if (chars_available)
				current_block.push_back(LetterGroup({group.first, chars_available + 1}));

			blocks.push_back(current_block);
			current_block.clear();

			int letters_to_print		  = group.second - chars_available - 1;
			int letters_in_indented_block = gumsmaq_max_line_count * (max_count - 1);
			if (letters_to_print >= letters_in_indented_block)
				do {
					current_block.push_back(LetterGroup({group.first, letters_in_indented_block},
						chars_available));
					blocks.push_back(current_block);
					current_block.clear();
					chars_available = 1; // dummy value so that !chars_available is false
				} while ((letters_to_print -= letters_in_indented_block) >= letters_in_indented_block);

			if (letters_to_print) {
				current_block.push_back(LetterGroup({group.first, letters_to_print}, chars_available));
				/// ... and this part ...
				int line_diff = letters_to_print / (max_count - 1);
				if (letters_to_print == 1) line_diff = 1;
				else if (int mod = letters_to_print - 1; letters_to_print % 9 && mod > 0 && mod % 9)
					line_diff++;
				remaining_lines = gumsmaq_max_line_count - line_diff;
			} else
				remaining_lines = gumsmaq_max_line_count;
		}
	}
	if (!current_block.empty())
		blocks.push_back(current_block);

	int bsiz = blocks.size();
	int ht	 = 0;
	if (blocks.size() == 1)
		for (const auto& block : blocks[0]) ht += block.height;
	else
		ht = gumsmaq_max_line_count * GUMSMAQ_LINE_HEIGHT;
	image img{bsiz * GUMSMAQ_LINE_WIDTH + (bsiz - 1) * gumsmaq_inter_block_space, ht};

	int x = 0;
	for (const auto& block : blocks) {
		int	  yy = 0;
		image block_img{GUMSMAQ_LINE_WIDTH, gumsmaq_max_line_count * GUMSMAQ_LINE_HEIGHT};
		for (const auto& lgroup : block) {
			block_img.WriteAtIfAlpha(0, yy, lgroup);
			yy += lgroup.height;
		}
		img.WriteAtIfAlpha(x, 0, block_img);
		x += block_img.width + gumsmaq_inter_block_space;
	}

	return img;
}
#undef max_count
} // namespace Gumsmaq
