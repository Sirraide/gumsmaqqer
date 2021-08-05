#ifndef GUMSMAQQER_RENDERER_H
#define GUMSMAQQER_RENDERER_H
#include "image.h"

#include <string>
#include <utility>
#include <vector>

namespace Gumsmaq {
using letter_group_t = std::pair<std::string, int>;
using vector_t		 = std::vector<letter_group_t>;

image Paragraph(const vector_t& groups);
image ArrangeGroup(const letter_group_t& lg, int start_index, int letter_count);
image LetterGroup(const letter_group_t& lg, bool indented = false);

const image& LoadLetter(const std::string& which);
vector_t	 VectorFromAbbr(const std::string& str);
std::string VectorToUnicode(const vector_t& gumsmaq);

}; // namespace Gumsmaq

#endif // GUMSMAQQER_RENDERER_H
