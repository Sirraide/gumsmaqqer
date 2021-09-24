#include "index.h"

#include "renderer.h"
#include "utils.h"

using namespace std;

namespace gsmq {
extern std::string			 assets_dir;
std::map<std::string, image> __index;

#ifndef WIN32
#include <signal.h>
#else
#include <exception>
#endif

void __generate_letter_index(string __path) {
	cerr << RED "LIBGUMSMAQQ DESCENDED INTO THE LOWEST ABYSS AND BEYOND. DO NOT USE.\n";
#ifndef WIN32
		raise(SIGABRT);
#else
		std::terminate();
#endif

//	__find_raw_assets();
//	if (assets_dir.empty()) {
//		fatal(RED, "ERROR: RAW GUMSMAQQING ASSETS NOT FOUND!\n"
//				   "ENSURE EXISTENCE OF DIRECTORY ASSETS CONTAINING ASSETS");
//	}
//
//	vector<string> __idx;
//	for (const auto __letter : __sgtf) {
//		image __img = Visual::LoadLetter(__letter.first);
//		__idx.push_back(__letter.first);
//		__index[__letter.first] = __img;
//	}
//	//	image  __atlas(GUMSMAQ_LOGOGRAM_WIDTH * __index.size(), GUMSMAQ_LOGOGRAM_HEIGHT);
//	string __s;
//
//	for (const auto& __img : __index) {
//		for (size_t __i = 0; __i < __img.second.size; __i++) {
//			if (!(__i++ % 16)) __s += "\n\t";
//			int __a = __img.second.data[__i];
//			if (!__a) __s += "0x00, ";
//			else {
//				char hex[20] = {0};
//				sprintf(hex, "0x%x", (int) __a);
//				__s += string(hex) + ", ";
//			}
//		}
//
//		//__atlas.WriteAt(__j++ * GUMSMAQ_LOGOGRAM_WIDTH, 0, __img.second);
//	}
//
//	//	{
//	//		string __s;
//	//		for (const auto& __el : __idx) __s += __el + " ";
//	//		__s.erase(__s.size() - 1);
//	//
//	//		io::file<char, io::filemode::wb>("atlas.dat").raw(__atlas.data, 1, __atlas.size);
//	//		io::ofile("atlas.idx").write(__s);
//	//	}
//
//	io::file<char, io::filemode::wb>__h(__path);
//
//	string __Index;
//
//	for (const auto& __el : __idx) __Index += "\t\"" + __el + "\",\n";
//
//	__h.writef("// This file was automatically generated. Do not modify.\n"
//			   "inline constexpr const char *__atlas_index[] = {\n%s};\n\n"
//			   "inline constexpr const size_t __atlas_index_count = sizeof __atlas_index / sizeof *__atlas_index;\n\n",
//			   __Index.c_str());
//
////	__s.clear();
////
////	for (size_t __i = 0; __i < __index.size(); __i++) {
////		if (!(__i % 16)) __s += "\n\t";
////		int __a = __atlas.data[__i];
////		if (!__a) __s += "0x00, ";
////		else {
////			char hex[20] = {0};
////			sprintf(hex, "0x%x", (int) __a);
////			__s += string(hex) + ", ";
////		}
////	}
//
//	__h.write("inline constexpr const unsigned char __atlas[] = {"); //, __s.data()
//	__h.raw(__s.data(), __s)
//	__h.write("\n};");
}

} // namespace gsmq
