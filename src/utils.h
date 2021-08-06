#ifndef GUMSMAQQER_UTILS_H
#define GUMSMAQQER_UTILS_H

#include "../lib/io.h"
#include "version.h"

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

#define GUMSMAQQER_MAJOR_VERSION 0
#define GUMSMAQQER_MINOR_VERSION 7
#define GUMSMAQQER_VERSION        \
	STR(GUMSMAQQER_MAJOR_VERSION) \
	"." STR(GUMSMAQQER_MINOR_VERSION) "." STR(GUMSMAQQER_PATCH_VERSION)

#ifdef WIN32
#	define GUMSMAQ_NORETURN __declspec(noreturn)
#	define GUMSMAQ_PACKED
template <typename T>
inline T clamp(T val, T lo, T hi) {
	return val < lo ? lo : val > hi ? hi
									: val;
}
#else
#	define GUMSMAQ_NORETURN __attribute__((noreturn))
#	define GUMSMAQ_PACKED	 __attribute__((packed))
#endif

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
GUMSMAQ_NORETURN inline void fatal(const std::string colour, const std::string& err) noexcept {
	std::cerr << colour << err << R << colour << "\nABORTED AND SUNKEN INTO DESPAIR"
			  << "\nEIDOLA OF PRODIGIOUS INEPTITUDE MAY AVAIL THEIR PITIFUL SELVES OF " R Y "-h" R "\n";
	exit(1);
}
//GUMSMAQ_NORETURN inline void fatal(const std::wstring colour, const std::wstring& err) noexcept {
//	std::wcerr << err << R << colour << L"\nABORTED AND SUNKEN INTO DESPAIR"
//	<< L"\nEIDOLA OF PRODIGIOUS INEPTITUDE MAY AVAIL THEIR PITIFUL SELVES OF " R Y "-h" R "\n";
//	exit(1);
//}
#endif

std::wstring strtowcs(const std::string& str);

extern int gumsmaq_max_letter_group_count;
extern int gumsmaq_max_line_count;
extern int gumsmaq_letter_kern;
extern int gumsmaq_group_indent_count;
extern int gumsmaq_inter_block_space;

#endif // GUMSMAQQER_UTILS_H
