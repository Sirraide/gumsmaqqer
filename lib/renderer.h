#ifndef GUMSMAQQER_RENDERER_H
#define GUMSMAQQER_RENDERER_H

#include "image.h"

#include <string>
#include <utility>
#include <vector>

namespace gsmq {

namespace Visual {
using letter_group_t = std::pair<std::string, int>;
using vector_t		 = std::vector<letter_group_t>;

extern std::unordered_map<std::string, image> cached_letters;
void LoadAtlas(unsigned char* __atlas, const char** __index, const size_t __index_len);

image Paragraph(const vector_t& groups);
image Subline(const letter_group_t& lg, int start_index, int letter_count);
image LetterGroup(const letter_group_t& lg, bool indented = false);

const image& LoadLetter(const std::string& which);
vector_t	 VectorFromAbbr(const std::string& str);

}; // namespace Visual

}; // namespace gsmq

#endif // GUMSMAQQER_RENDERER_H
