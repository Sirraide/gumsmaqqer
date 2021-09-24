#ifndef GUMSMAQQER_INDEX_H
#define GUMSMAQQER_INDEX_H

#include "image.h"
#include "emojis.h"

#include <vector>

namespace gsmq {
extern std::map<std::string, image> __index;
void __generate_letter_index(std::string __path);
} // namespace gsmq

#endif // GUMSMAQQER_INDEX_H
