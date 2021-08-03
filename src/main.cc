#include "emojis.h"
#include "image.h"
#include "parser.h"

#include <clocale>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main(int argc, char** argv) {
	setlocale(LC_ALL, "");
	//	auto gumsmaq = Gumsmaq::VectorToUnicode(Gumsmaq::VectorFromAbbr(L"gA2"));
	//	wcout << gumsmaq << endl;
	string path = "twemoji/assets/72x72/";
	stringstream stream;
	stream << hex << (int) Gumsmaq::green_apple;
	path += stream.str() + ".png";
	image emoji(path);
	image img{emoji.width * 4, 2 * emoji.height};

	img.WriteAt(0, 0, emoji);
	img.WriteAt(emoji.width, emoji.height / 2, emoji);
	img.WriteAt(2*emoji.width, 0, emoji);
	img.WriteAt(3*emoji.width , emoji.height / 2, emoji);

	img.save("test.png");
}