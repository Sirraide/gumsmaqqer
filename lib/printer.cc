#include "printer.h"

#include "../lib/gsmq.h"

#include <string>
using namespace std;
namespace gsmq {

namespace Textual {
block& block::operator+=(u32string& ustr) {
	int spaces = gumsmaq_max_letter_group_count - ustr.size();
	if (spaces > 0) repeat(spaces) ustr.push_back(FULLWIDTH_SPACE);
	lines.push_back(ustr);
	return *this;
}

block& block::operator+=(const u32string& vec) {
	lines.push_back(vec);
	return *this;
}

block& block::operator+=(const block& b) {
	for (const auto& line : b.lines)
		*this += line;
	return *this;
}

char32_t LoadLetter(const string& which) {
	if (!gsmq::__sgtf.contains(which)) fatal(RED_BOLD, "INVALID GUMSMAQ \"" RED + which + RED_BOLD "\"");
	return gsmq::__sgtf.at(which);
}

tuple<block, int, int> LetterGroup(const letter_group_t& lg, int letters_to_print, int sublines, bool indented) {
	const char32_t letter = LoadLetter(lg.first);
	block		   group;

	u32string subline;

	auto max_letters_first_line = indented
									  ? gumsmaq_max_letter_group_count - gumsmaq_group_indent_count
									  : gumsmaq_max_letter_group_count;

	if (letters_to_print <= max_letters_first_line && sublines > 0) {
		subline.resize(indented ? letters_to_print + gumsmaq_group_indent_count : letters_to_print);
		int i = 0;
		if (indented)
			for (; i < gumsmaq_group_indent_count; i++) subline[i] = FULLWIDTH_SPACE;
		for (; i < letters_to_print; i++) subline[i] = letter;
		group += subline;
		return {group, 0, sublines - 1};
	}

	subline.resize(gumsmaq_max_letter_group_count);
	repeat(gumsmaq_max_letter_group_count) subline[iter__] = letter;

	if (!indented) {
		letters_to_print -= gumsmaq_max_letter_group_count;
		sublines--;
		group += subline;
	}

	if (letters_to_print > 0) {
		const int letters_in_subline					   = gumsmaq_max_letter_group_count - gumsmaq_group_indent_count;
		repeat(gumsmaq_group_indent_count) subline[iter__] = FULLWIDTH_SPACE;

		sublines++;

		while (--sublines && letters_to_print >= letters_in_subline) {
			letters_to_print -= letters_in_subline;
			group += subline;
		}

		if (sublines > 0 && letters_to_print > 0) {
			subline.resize(letters_to_print + gumsmaq_group_indent_count);
			letters_to_print = 0;
			sublines--;
			group += subline;
		}
	}

	letters_to_print = max(0, letters_to_print);
	sublines		 = max(0, sublines);

	return {group, letters_to_print, sublines};
}

vector<block> Paragraph(const vector_t& groups) {
	block		  p;
	int			  remaining_lines = gumsmaq_max_line_count;
	vector<block> blocks;
	block		  current_block;
	for (const auto& g : groups) {
		if (g.second < 1) {
			cerr << RED "VACUOUS GUMSMAQ \"" << g.first << to_string(g.second)
				 << "\" LOST IN SPACE AND TIME\n" R;
			continue;
		}
		auto letters_to_print = g.second;

		auto first = LetterGroup(g, letters_to_print, remaining_lines);
		current_block += get<0>(first);
		letters_to_print = get<1>(first);
		remaining_lines	 = get<2>(first);

		if (remaining_lines < 1) {
			blocks.push_back(current_block);
			current_block.lines.clear();
			remaining_lines = gumsmaq_max_line_count;
		}

		if (!letters_to_print) continue;

		remaining_lines			  = gumsmaq_max_line_count;
		const int max_block_chars = gumsmaq_max_line_count * (gumsmaq_max_letter_group_count - gumsmaq_group_indent_count);

		if (letters_to_print < max_block_chars) {
		last_block:
			auto last		= LetterGroup(g, letters_to_print, gumsmaq_max_line_count, true);
			current_block	= get<0>(last);
			remaining_lines = get<2>(last);
			continue;
		}

		current_block = get<0>(LetterGroup(g, max_block_chars, gumsmaq_max_line_count, true));
		do blocks.push_back(current_block);
		while (letters_to_print -= max_block_chars);

		if (letters_to_print > 0) goto last_block;
	}
	if (!current_block.lines.empty()) blocks.push_back(current_block);
	if (blocks.empty()) fatal(RED, "GUMSMAQQING YIELDED ONLY VAST EMPTINESS");
	return blocks;
}
u32string Text(const vector_t& groups) {
	auto	  blocks   = Paragraph(groups);
	auto*	  composed = new u32string[gumsmaq_max_line_count];
	u32string inter_block_space;
	u32string text;
	repeat(gumsmaq_inter_block_space) inter_block_space += FULLWIDTH_SPACE;

	for (size_t b_i = 0, bsize = blocks.size(); b_i < bsize; b_i++) {
		if (blocks[b_i].lines.size() > gumsmaq_max_line_count)
			fatal(RED, "Internal error [Textual::Text()]: b.lines.size() exceeded max line count");
		for (int i = 0; i < blocks[b_i].lines.size(); i++) {
			composed[i] += blocks[b_i].lines[i];
			if (b_i < bsize - 1) composed[i] += inter_block_space;
		}
	}

	repeat(gumsmaq_max_line_count) {
		text += composed[iter__];
		text += UNEWLINE;
	}

	delete[] composed;
	return text;
}

} // namespace Textual

} // namespace gsmq