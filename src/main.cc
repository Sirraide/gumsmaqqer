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
int gumsmaq_letter_kern			   = -(72 / 3);
int gumsmaq_group_indent		   = GUMSMAQ_LOGOGRAM_WIDTH + gumsmaq_letter_kern;
int gumsmaq_inter_block_space	   = 15;

int main(void) {
	setlocale(LC_ALL, "");

	auto gumsmaq = Gumsmaq::VectorFromAbbr(L"L91T91NACL1");
	auto img	 = Gumsmaq::Paragraph(gumsmaq);
	img.save("test.png");
}