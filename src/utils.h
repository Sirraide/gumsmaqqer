#ifndef GUMSMAQQER_UTILS_H
#define GUMSMAQQER_UTILS_H

#define GUMSMAQ_LOGOGRAM_WIDTH	72
#define GUMSMAQ_LOGOGRAM_HEIGHT 72

#define GUMSMAQ_LINE_HEIGHT (GUMSMAQ_LOGOGRAM_HEIGHT + int(GUMSMAQ_LOGOGRAM_HEIGHT * .5))
#define GUMSMAQ_LINE_WIDTH	(gumsmaq_max_letter_group_count * GUMSMAQ_LOGOGRAM_WIDTH \
							+ (gumsmaq_max_letter_group_count - 1) * gumsmaq_letter_kern)

#define STR(what)  #what
#define CAT_(X, Y) X##Y
#define CAT(X, Y)  CAT_(X, Y)

#define repeat(N_) for (size_t iter__ = 0, N__ = N_; iter__ < N__; iter__++)

extern int gumsmaq_max_letter_group_count;
extern int gumsmaq_max_line_count;
extern int gumsmaq_letter_kern;
extern int gumsmaq_group_indent;
extern int gumsmaq_inter_block_space;

#endif // GUMSMAQQER_UTILS_H
