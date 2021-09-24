#include "renderer.h"

#include "../lib/gsmq.h"

#include <cctype>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
using namespace std;
namespace gsmq::Visual {
#define CONTINUE          \
	if (it == end) break; \
	else                  \
		continue

unordered_map<string, image> cached_letters;

void LoadAtlas(unsigned char* __atlas, const char** __index, const size_t __index_len) {
	for (size_t __i = 0; __i < __index_len; __i++) {
		image __img(GUMSMAQ_LOGOGRAM_WIDTH, GUMSMAQ_LOGOGRAM_HEIGHT);
		__img.data	 = __atlas + __i * __img.size;
		__img.nofree = true;
		string __s(__index[__i]);

		cached_letters[__s]		   = move(__img);
		cached_letters[__s].nofree = true;
		__img.nofree			   = true;
	}
}

vector_t VectorFromAbbr(const string& str) {
	vector_t   ret;
	string	   buf;
	const auto end = str.end();

	for (auto it = str.begin(); it != end;) {
		while (it != end && !isdigit(*it)) {
			if (!isspace(*it)) buf += *it;
			it++;
		}
		if (it == end) fatal(RED_BOLD, "INVALID GUMSMAQ");

		if (isdigit(*it)) {
			string num;
			do num += *it++;
			while (it != end && isdigit(*it));
			ret.emplace_back(buf, strtol(num.c_str(), nullptr, 10));
			buf.clear();
			CONTINUE;
		}
	}
	return ret;
}

const image& LoadLetter(const string& which) {
	if (!cached_letters.contains(which)) {
		if (!gsmq::__sgtf.contains(which)) fatal(RED_BOLD, "INVALID GUMSMAQ \"" RED + which + RED_BOLD "\"");
		stringstream stream;
		stream << hex << (int) gsmq::__sgtf.at(which);
		string img_path = assets_dir + stream.str() + ".png";
		image  letter(img_path);
		cached_letters.insert({which, move(letter)});
	}
	return cached_letters.at(which);
}

image Subline(const letter_group_t& lg, int start_index, int letter_count) {
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

#define max_count		   gumsmaq_max_letter_group_count
#define max_count_indented (gumsmaq_max_letter_group_count - gumsmaq_group_indent_count)
image LetterGroup(const letter_group_t& group, bool indented) {
	image		  img;
	int			  first_count = indented ? max_count_indented : max_count;
	vector<image> lines;
	if (group.second > first_count) {
		int	  remaining = group.second - first_count;
		image first		= Subline(group, 0, first_count);

		int count		 = max_count_indented;
		int count_groups = remaining / count;

		int start_index = first_count;
		for (int i = 0; i < count_groups; i++) {
			start_index += count;
			lines.push_back(Subline(group, start_index, count));
		}

		image last;
		if (remaining % count)
			last = Subline(group, start_index + count, remaining % count);

		int ht = GUMSMAQ_LINE_HEIGHT;
		ht += GUMSMAQ_LINE_HEIGHT * lines.size();
		if (remaining % count) ht += GUMSMAQ_LINE_HEIGHT;

		img = image{GUMSMAQ_LINE_WIDTH, ht};

		img.WriteAtIfAlpha(indented ? GUMSMAQ_GROUP_INDENT_WIDTH : 0, 0, first);
		int y = GUMSMAQ_LINE_HEIGHT;
		for (const auto& line : lines) {
			img.WriteAtIfAlpha(GUMSMAQ_GROUP_INDENT_WIDTH, y, line);
			y += GUMSMAQ_LINE_HEIGHT;
		}
		if (remaining % count > 0)
			img.WriteAtIfAlpha(GUMSMAQ_GROUP_INDENT_WIDTH, y, last);
	} else {
		image line = Subline(group, 0, group.second);
		img		   = image{GUMSMAQ_LINE_WIDTH, GUMSMAQ_LINE_HEIGHT};
		img.WriteAtIfAlpha(indented ? GUMSMAQ_GROUP_INDENT_WIDTH : 0, 0, line);
	}
	return img;
}

image Paragraph(const vector_t& groups) {
	int					  remaining_lines = gumsmaq_max_line_count;
	vector<vector<image>> blocks;
	vector<image>		  current_block;
	for (const auto& group : groups) {
		if (group.second < 1) {
			cerr << RED "VACUOUS GUMSMAQ \"" << group.first << to_string(group.second)
				 << "\" LOST IN SPACE AND TIME\n" R;
			continue;
		}
		if (remaining_lines <= 0) {
			blocks.push_back(current_block);
			current_block.clear();
			remaining_lines = gumsmaq_max_line_count;
		}
		int chars_available = !remaining_lines ? 0 : remaining_lines * max_count_indented;
		if (chars_available + 1 >= group.second) {
			/// this part is the worst...
			int line_diff = group.second / max_count_indented;
			if (group.second == 1) line_diff = 1;
			else if (int mod = group.second - 1;
					 group.second % max_count_indented
					 && mod > 0
					 && mod % max_count_indented)
				line_diff++;
			remaining_lines -= line_diff;

			current_block.push_back(LetterGroup(group));
		} else {
			if (chars_available)
				current_block.push_back(LetterGroup({group.first, chars_available + 1}));

			blocks.push_back(current_block);
			current_block.clear();

			int letters_to_print		  = group.second - chars_available - 1;
			int letters_in_indented_block = gumsmaq_max_line_count * max_count_indented;
			if (letters_to_print >= letters_in_indented_block)
				do {
					current_block.push_back(LetterGroup({group.first, letters_in_indented_block},
						chars_available));
					blocks.push_back(current_block);
					current_block.clear();
					chars_available = 1; /// dummy value so that it's true in the call to LetterGroup above
				} while ((letters_to_print -= letters_in_indented_block) >= letters_in_indented_block);

			if (letters_to_print) {
				current_block.push_back(LetterGroup({group.first, letters_to_print}, chars_available));
				/// ... and this part ...
				int line_diff = letters_to_print / max_count_indented;
				if (letters_to_print == 1) line_diff = 1;
				else if (int mod = letters_to_print - 1;
						 letters_to_print % max_count_indented
						 && mod > 0
						 && mod % max_count_indented)
					line_diff++;
				remaining_lines = gumsmaq_max_line_count - line_diff;
			} else
				remaining_lines = gumsmaq_max_line_count;
		}
	}
	if (!current_block.empty())
		blocks.push_back(current_block);

	if (blocks.empty())
		fatal(RED, "GUMSMAQQING YIELDED ONLY VAST EMPTINESS");

	const int bsiz = blocks.size();
	if (!bsiz) return image{0, 0};
	int ht = 0;
	if (blocks.size() == 1)
		for (const auto& block : blocks[0]) ht += block.height;
	else
		ht = gumsmaq_max_line_count * GUMSMAQ_LINE_HEIGHT;

	const int inter_block_space = gumsmaq_inter_block_space * GUMSMAQ_LOGOGRAM_WIDTH;
	image	  img{bsiz * GUMSMAQ_LINE_WIDTH + (bsiz - 1) * inter_block_space, ht};

	int x = 0;
	for (const auto& block : blocks) {
		int	  yy = 0;
		image block_img{GUMSMAQ_LINE_WIDTH, gumsmaq_max_line_count * GUMSMAQ_LINE_HEIGHT};
		for (const auto& lgroup : block) {
			block_img.WriteAtIfAlpha(0, yy, lgroup);
			yy += lgroup.height;
		}
		img.WriteAtIfAlpha(x, 0, block_img);
		x += block_img.width + inter_block_space;
	}

	return img;
}
#undef max_count

} // namespace gsmq::Visual