#include "../lib/gsmq.h"

#include <iostream>
//using namespace std;

//string padd20(const string& str) {
//	string ret = str;
//	while (ret.size() < 20) ret += ' ';
//	return ret;
//}

int main(int argc, char** argv) {
//	const string opt[]	  = {"--print", "--img"};
//	bool		 print	  = false;
//	bool		 flag_img = false;
//
//	for (int i = 1; i < argc; i++)
//		if (argv[i] == opt[0]) print = true;
//		else if (argv[i] == opt[1])
//			flag_img = true;

	gsmq::__generate_letter_index("src/gumsmaqqer.h");
//
//	if (print) {
//		for (const auto& el : gsmq::__index) {
//			u32string ustr;
//			ustr += gsmq::__sgtf.at(el.first);
//			cout << padd20(el.first) << "\t" << gsmq::ToUTF8(ustr) << "\n";
//		}
//	}
//
//	std::map<std::string, gsmq::image> index;
//	for (const auto& letter : gsmq::Visual::cached_letters) {
//		index[letter.first] = letter.second;
//	}
//
//	string s;
//
//	gsmq::image atlas(GUMSMAQ_LOGOGRAM_WIDTH * index.size(), GUMSMAQ_LOGOGRAM_HEIGHT);
//	size_t		i = 0;
//	for (const auto& img : index) {
//		atlas.WriteAt(i++ * GUMSMAQ_LOGOGRAM_WIDTH, 0, img.second);
//		s += img.first + " ";
//	}
//
//	s.erase(s.size() - 1);
//
//	gsmq::io::file<char, gsmq::io::filemode::wb>("atlas2.dat").raw(atlas.data, 1, atlas.size);
//	gsmq::io::ofile("atlas2.idx").write(s);
//
//	string _atlas1 = gsmq::io::infile("atlas.dat").mmap();
//	string _atlas2 = gsmq::io::infile("atlas2.dat").mmap();
//	string _index1 = gsmq::io::infile("atlas.idx").mmap();
//	string _index2 = gsmq::io::infile("atlas2.idx").mmap();
//
//	bool flag = _index1 != _index2 || _atlas1 != _atlas2;
//
//	::remove("atlas.dat");
//	::remove("atlas2.dat");
//	::remove("atlas.idx");
//	::remove("atlas2.idx");
//
//	if (flag) gsmq::fatal(RED, "ERROR: DIFF FAILED. COULD NOT GENERATE SPRITESHEET.");
//	else
//		cout << "\033[32mSUCCESSFULLY GENERATED ATLAS\033[0m\n";
}