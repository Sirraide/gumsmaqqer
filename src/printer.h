#ifndef GUMSMAQQER_PRINTER_H
#define GUMSMAQQER_PRINTER_H
#include "renderer.h"
#include "utils.h"

#include <iostream>
#include <vector>
namespace Textual {
using ustring_t		 = std::vector<uchar_t>;
using letter_group_t = std::pair<std::string, int>;
using vector_t		 = std::vector<letter_group_t>;
using Visual::VectorFromAbbr;

struct block {
	std::vector<ustring_t> lines;

	block& operator+=(ustring_t& vec);
	block& operator+=(const ustring_t& vec);
	block& operator+=(const block& b);
};

ustring_t Text(const vector_t& groups);

ustring_t& operator+=(ustring_t& dest, const ustring_t& src);

std::vector<block>			Paragraph(const vector_t& groups);
std::tuple<block, int, int> LetterGroup(const letter_group_t& lg, int letters_to_print, int sublines, bool indented = false);

uchar_t		LoadLetter(const std::string& which);
std::string VectorToUnicode(const vector_t& gumsmaq);

}; // namespace Textual

template <typename T, typename Tr>
std::basic_ostream<T, Tr>& operator<<(std::basic_ostream<T, Tr>& ostr, const Textual::ustring_t& ustr) {
	for (const auto& u : ustr) {
#ifndef WIN32
		ostr << (wchar_t) u;
#else
		throw std::runtime_error("Forgot to implement printing emojis to console on windows");
#endif
	}
	if constexpr (std::is_same_v<T, char>) ostr << NEWLINE;
	else
		ostr << WNEWLINE;
	return ostr;
}

template <typename T, typename Tr>
std::basic_ostream<T, Tr>& operator<<(std::basic_ostream<T, Tr>& ostr, const Textual::block& b) {
	for (const auto& line : b.lines) {
		for (const auto& u : line) {
#ifndef WIN32
			ostr << (wchar_t) u;
#else
			throw std::runtime_error("Forgot to implement printing emojis to console on windows");
#endif
		}
		if constexpr (std::is_same_v<T, char>) ostr << NEWLINE;
		else
			ostr << WNEWLINE;
	}
	return ostr;
}

#endif // GUMSMAQQER_PRINTER_H
