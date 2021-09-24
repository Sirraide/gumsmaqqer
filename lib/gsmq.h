#ifndef GUMSMAQQER_GSMQ_H
#define GUMSMAQQER_GSMQ_H

#include "emojis.h"
#include "image.h"
#include "io.h"
#include "utils.h"
#include "renderer.h"
#include "printer.h"
#include "index.h"

namespace gsmq {
extern int		   gumsmaq_max_letter_group_count;
extern int		   gumsmaq_max_line_count;
extern int		   gumsmaq_letter_kern;
extern int		   gumsmaq_group_indent_count;
extern int		   gumsmaq_inter_block_space;
extern int		   gumsmaq_max_block_count;
extern std::string assets_dir;

void FindGumsmaqqingAssets();
} // namespace gsmq

#endif // GUMSMAQQER_GSMQ_H
