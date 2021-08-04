#include "emojis.h"
#include "image.h"
#include "renderer.h"
#include "utils.h"

#include <clocale>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int gumsmaq_max_letter_group_count = 10;
int gumsmaq_max_line_count		   = 10;
int gumsmaq_letter_kern			   = - (72 / 3);
int gumsmaq_group_indent = GUMSMAQ_LOGOGRAM_WIDTH + gumsmaq_letter_kern;
int gumsmaq_inter_block_space = 15;


int main(void) {
	setlocale(LC_ALL, "");

	//	auto gumsmaq = Gumsmaq::VectorToUnicode(Gumsmaq::VectorFromAbbr(L"gA2"));
	//	wcout << gumsmaq << endl;
//	auto emoji = Gumsmaq::LoadLetter(L"gA");
//	image img{emoji.width * 4, 2 * emoji.height};
//	img.WriteAtIfAlpha(0, 0, emoji);
//	img.WriteAtIfAlpha(emoji.width - emoji.width / 3, emoji.height / 2, emoji);
//	img.WriteAtIfAlpha(emoji.width + emoji.width / 3, 0, emoji);
//	img.WriteAtIfAlpha(2 * emoji.width, emoji.height / 2, emoji);

auto img = Gumsmaq::Paragraph({
		{L"L", 92},
		{L"gA", 91},
		{L"T", 150},
		{L"gA", 7}
	});
	img.save("test.png");
}