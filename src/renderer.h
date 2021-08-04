#ifndef GUMSMAQQER_RENDERER_H
#define GUMSMAQQER_RENDERER_H
#include <string>
#include <utility>
#include <vector>
#include "image.h"

namespace Gumsmaq {
using letter_group_t = std::pair<std::wstring, int>;
using vector_t	= std::vector<letter_group_t>;

// Render image from Standard Gumsmaq Transcription Form
image SGQTFToImage(const std::wstring& str);

// Convert from Standard Gumsmaq Transcription Form to text
std::wstring SGQTFToText(const std::wstring& sgqtf_text);

image Paragraph(const vector_t& groups);
image ArrangeGroup(const letter_group_t& lg, int start_index,
				int letter_count);
image LetterGroup(const letter_group_t& lg, bool indented = false);

const image& LoadLetter(const std::wstring& which);
vector_t	 VectorFromAbbr(const std::wstring& str);
std::wstring VectorToUnicode(const vector_t& gumsmaq);

}; // namespace Gumsmaq

#endif // GUMSMAQQER_RENDERER_H
