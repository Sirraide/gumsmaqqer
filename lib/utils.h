#ifndef GUMSMAQQER_UTILS_H
#define GUMSMAQQER_UTILS_H

#include "io.h"

#include <cassert>
#include <iostream>
#include <string>

#define Y		 "\033[33m"
#define B		 "\033[36m"
#define M		 "\033[35m"
#define R		 "\033[0m"
#define W		 "\033[1;37m"
#define G		 "\033[32m"
#define RED		 "\033[31m"
#define RED_BOLD "\033[1;31m"

#define FULLWIDTH_SPACE 0x3000

#ifdef WIN32
#	define GUMSMAQ_NORETURN __declspec(noreturn)
#	define GUMSMAQ_PACKED
#	define NEWLINE			 "\r\n"
#	define CHARS_IN_NEWLINE 2
#else
#	define GUMSMAQ_NORETURN __attribute__((noreturn))
#	define GUMSMAQ_PACKED	 __attribute__((packed))
#	define NEWLINE			 "\n"
#	define CHARS_IN_NEWLINE 1
#endif

#define WNEWLINE CAT(L, NEWLINE)
#define UNEWLINE CAT(U, NEWLINE)

#ifdef DEBUG_BUILD
//#define DEBUG_DO_THROW
#endif

#define GUMSMAQ_LOGOGRAM_WIDTH	72
#define GUMSMAQ_LOGOGRAM_HEIGHT 72

#define GUMSMAQ_LINE_HEIGHT (GUMSMAQ_LOGOGRAM_HEIGHT + int(GUMSMAQ_LOGOGRAM_HEIGHT * .5))
#define GUMSMAQ_LINE_WIDTH	(gumsmaq_max_letter_group_count * GUMSMAQ_LOGOGRAM_WIDTH \
							+ (gumsmaq_max_letter_group_count - 1) * gumsmaq_letter_kern)
#define GUMSMAQ_GROUP_INDENT_WIDTH ((GUMSMAQ_LOGOGRAM_WIDTH + gumsmaq_letter_kern) * gumsmaq_group_indent_count)

#define STR_(what) #what
#define STR(what)  STR_(what)
#define CAT_(X, Y) X##Y
#define CAT(X, Y)  CAT_(X, Y)

#define repeat(N_) for (size_t iter__ = 0, N__ = N_; iter__ < N__; iter__++)

#if defined(DEBUG_BUILD) && defined(DEBUG_DO_THROW)
#	define fatal(...) throw std::runtime_error(__FILE__ ":" STR(__LINE__))
#else
GUMSMAQ_NORETURN inline void fatal(const std::string& colour, const std::string& err) noexcept {
	std::cerr << colour << err << R << colour << "\nABORTED AND SUNKEN INTO DESPAIR"
			  << "\nEIDOLA OF PRODIGIOUS INEPTITUDE MAY AVAIL THEIR PITIFUL SELVES OF " R Y "-H" R "\n";
	exit(1);
}
#endif

extern int gumsmaq_max_letter_group_count;
extern int gumsmaq_max_line_count;
extern int gumsmaq_letter_kern;
extern int gumsmaq_group_indent_count;
extern int gumsmaq_inter_block_space;
extern int gumsmaq_max_block_count;

extern std::string assets_dir;

void		reset_colour();
int			Number(const char* arg);
void		SetText(std::string& what, const char* arg);
std::string RemoveColour(std::string& str);
std::string ToUTF8(const std::u32string& what);

#endif // GUMSMAQQER_UTILS_H
