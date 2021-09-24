#include "utils.h"

#include <codecvt>
#include <filesystem>
#include <locale>
using namespace std;

namespace gsmq {

int gumsmaq_max_letter_group_count = 10;
int gumsmaq_max_line_count		   = 10;
int gumsmaq_letter_kern			   = -(72 / 3);
int gumsmaq_group_indent_count	   = 1;
int gumsmaq_inter_block_space	   = 1;
int gumsmaq_max_block_count		   = 1;

string assets_dir;

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
// clang-format on

string ToUTF8(const u32string& what) {
	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
	return conv.to_bytes(what);
}

void FindGumsmaqqingAssets() {
	__find_raw_assets();
	if (assets_dir.empty()) {
#ifndef WIN32
		string pwd{filesystem::current_path().c_str()};
#else
		string pwd;
		pwd.reserve(100000);
		wcstombs(pwd.data(), filesystem::current_path().c_str(), 100000);
#endif
		fatal(RED, string("GUMSMAQQING ASSETS FOUND WANTING\nENSURE EXISTENCE OF ASSETS DIRECTORY " Y
		"assets" RED " CONTAINING ASSETS IN " Y)
		+ pwd + RED "\nELSE MUST NEEDS SET ENVIRONMENT VARIABLE " Y "GUMSMAQQER_ASSETS_DIR" //
		RED " TO ASSETS DIRECTORY CONTAINING ASSETS");
	}
}

void __find_raw_assets() {
	if (!assets_dir.empty()) return;
	if (filesystem::exists("./twemoji/assets/72x72")) assets_dir = "./twemoji/assets/72x72";
	else if (filesystem::exists("./assets/72x72"))
		assets_dir = "./assets/72x72";
	else if (filesystem::exists("./assets"))
		assets_dir = "./assets";
	else if (filesystem::exists("./72x72"))
		assets_dir = "./72x72";
	else {
		char* _assets_dir = std::getenv("GUMSMAQQER_ASSETS_DIR");
		if (!_assets_dir) {
			assets_dir = "";
			return;
		};
		assets_dir = _assets_dir;
	}

	if (!assets_dir.ends_with('/')) assets_dir += '/';
}

} // namespace gsmq