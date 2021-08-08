#ifndef GUMSMAQQER_PRINTER_H
#define GUMSMAQQER_PRINTER_H
#include "renderer.h"
#include "utils.h"

#include <iostream>
#include <vector>
namespace Textual {
using letter_group_t = std::pair<std::string, int>;
using vector_t		 = std::vector<letter_group_t>;
using Visual::VectorFromAbbr;

struct block {
	std::vector<std::u32string> lines;

	block& operator+=(std::u32string& vec);
	block& operator+=(const std::u32string& vec);
	block& operator+=(const block& b);
};

std::u32string Text(const vector_t& groups);

std::vector<block>			Paragraph(const vector_t& groups);
std::tuple<block, int, int> LetterGroup(const letter_group_t& lg, int letters_to_print, int sublines, bool indented = false);

char32_t LoadLetter(const std::string& which);

}; // namespace Textual

#endif // GUMSMAQQER_PRINTER_H
