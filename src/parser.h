#ifndef GUMSMAQQER_PARSER_H
#define GUMSMAQQER_PARSER_H
#include <string>
#include <vector>
#include <utility>

namespace Gumsmaq {
using gumsmaq_t = std::pair<std::wstring, long>;
using vector_t	= std::vector<gumsmaq_t>;

vector_t VectorFromAbbr(const std::wstring &str);
std::wstring VectorToUnicode(const vector_t& gumsmaq);

};

#endif // GUMSMAQQER_PARSER_H
