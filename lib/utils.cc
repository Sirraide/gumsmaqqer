#include "utils.h"

#include <codecvt>
#include <locale>
using namespace std;

int gumsmaq_max_letter_group_count = 10;
int gumsmaq_max_line_count		   = 10;
int gumsmaq_letter_kern			   = -(72 / 3);
int gumsmaq_group_indent_count	   = 1;
int gumsmaq_inter_block_space	   = 1;
int gumsmaq_max_block_count		   = 1;

void reset_colour() {
	cout << R;
}

int Number(const char* arg) {
	try {
		long l = strtol(arg, nullptr, 10);
		return int(l);
	} catch (exception& ignored) { fatal(RED, "BAD NUMBER \"" G + string(arg)); }
}

void SetText(string& what, const char* arg) {
	what = arg;
	if (what.starts_with('\"') || what.starts_with('\'')) what = what.substr(1);
	if (what.ends_with('\"') || what.ends_with('\'')) what.erase(what.size() - 1);
}

// clang-format off
string RemoveColour(string& str) {
	for (;;) {
		auto pos = str.find('\033');
		if (pos == string::npos) break;
		str.replace(pos, str[pos + 2] == '0' ? 4 : str[pos + 3] == ';' ? 7 : 5, "");
	}
	return str;
}

string ToUTF8(const u32string& what) {
	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
	return conv.to_bytes(what);
}

// clang-format on
