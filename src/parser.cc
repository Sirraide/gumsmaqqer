#include "parser.h"

#include <cctype>
#include <stdexcept>
#include "emojis.h"
#include "utils.h"

using namespace std;

namespace Gumsmaq {
#define CONTINUE          \
	if (it == end) break; \
	else                  \
		continue
vector_t VectorFromAbbr(const wstring &str) {
	vector_t   ret;
	wstring	   buf;
	const auto end = str.end();

	for (auto it = str.begin(); it != end;) {
		while (it != end && !isdigit(*it)) {
			if (!isspace(*it)) buf += *it;
			it++;
		}
		if (it == end) throw std::runtime_error("Invalid Gumsmaq: missing number");

		if (isdigit(*it)) {
			wstring num;
			do num += *it++;
			while (it != end && isdigit(*it));
			ret.emplace_back(buf, wcstol(num.c_str(), nullptr, 10));
			buf.clear();
			CONTINUE;
		}
	}
	return ret;
}

wstring VectorToUnicode(const vector_t& gumsmaq) {
	wstring out;
	for(const auto& g : gumsmaq) repeat(g.second) out += abbr_gumsmaq_table.at(g.first);
	return out;
}

} // namespace Gumsmaq
