#include "../lib/io.h"
#include "renderer.h"
#include "utils.h"

#include <clocale>
#include <cstring>
using namespace std;

int gumsmaq_max_letter_group_count = 10;
int gumsmaq_max_line_count		   = 10;
int gumsmaq_letter_kern			   = -(72 / 3);
int gumsmaq_group_indent_count	   = 1;
int gumsmaq_inter_block_space	   = 15;

bool collect	   = false;
bool collect_files = false;
bool textual_mode  = false;
bool no_colour	   = false;
bool print_usage   = false;
bool verbose	   = false;

string				ofilename;
vector<io::infile*> infiles;
vector<string>		input_text;

__attribute__((noreturn)) inline void FatalArgs(const string& errmsg) {
	fatal(errmsg + "\nUse --help to print usage information.");
}

__attribute__((noreturn)) void PerrorArgs(const string& filename) {
	std::string s = "File '" + filename + "' opened by io::file is invalid";
	perror(s.c_str());
	fatal("\nUse --help to print usage information.");
}

int Number(const char* arg) {
	try {
		long l = strtol(arg, nullptr, 10);
		return int(l);
	} catch (exception& ignored) { fatal(string("Error: ") + arg + "is not a number."); }
}

void SetText(string& what, const char* arg) {
	what = arg;
	if (what.starts_with('\"') || what.starts_with('\'')) what = what.substr(1);
	if (what.ends_with('\"') || what.ends_with('\'')) what.erase(what.size() - 1);
}

string RemoveColour(string& str) {
	for (;;) {
		auto pos = str.find("\033");
		if (pos == string::npos) break;
		str.replace(pos, str[pos + 2] == '0' ? 4 : str[pos + 3] == ';' ? 7
																	   : 5,
			"");
	}
	return str;
}

__attribute((noreturn)) void PrintVersion() {
	static const string version_info = Y "This is version " B GUMSMAQQER_VERSION Y " of the "
	  "\033[1;33mGumsmaqqer\n" W Y
	  "Converts SGTF (Standard Gumsmaq Transcription Form) to either Unicode or PNG\n"
	  "See also --help for more detailed information\n";
	cout << version_info << flush;
	exit(0);
}

__attribute__((noreturn)) void PrintUsage() {
	static const string usage =
		Y "See also --help for more detailed information\n\n"
		  "Usage: " B EXECUTABLE_NAME Y " [ OPTIONS ]" G " <data> " Y "[ OPTIONS ]\n"
		  "\n"
		  "  " G "<data>" W " can be " Y "[ -s" W ", " Y "--string ] " G "<string>" W " or " Y "-f" W ", " Y "--file " G "<file>\n" W
		  "\n"
		  "  OPTIONS:\n"
		  "      " Y "-h" W ", " Y "--help" W "              Display help information. Note that " Y "--help" W " is much more\n"
		  "                              verbose than " Y "-h" W ".\n"
		  "      " Y "-o" W ", " Y "--out " G "<filename> " W "   Tell the "
		  "\033[1;33m"
		  "Gumsmaqqer" W " to output to " G "<filename>" W ".\n" Y
		  "      -g" W ", " Y "--group-length " G "<n>" W "  Set the group length to " G "<n>" W " (" G "<n> - 1" W " for indented lines).\n" Y
		  "      -l" W ", " Y "--lines " G "<n> " W "        Set the number of lines that may appear in a block;\n" Y
		  "      -k" W ", " Y "--kern " G "<n>" W "          Set the kerning between letters to " G "<n>" W " pixels.\n" Y
		  "      -b" W ", " Y "--block-space " G "<n>" W "   Set the space between blocks to " G "<n>" W " pixels.\n" Y
		  "      -i" W ", " Y "--indent " G "<n>" W "	      Set line indentation to " G "<n>\n" Y
		  "      -m" W ", " Y "--merge" W "             Merge all input " G "<string>" W "s into one file.\n" Y
		  "      -M" W ", " Y "--merge-files" W "       Same as " Y "--merge" W " but for files.\n" Y
		  "      -t" W ", " Y "--text" W "              Switch to textual output mode for all following " G "<string>" W "s and " G "<file>" W "s.\n" Y
		  "      -I" W ", " Y "--image" W "             This is the opposite of " Y "--text\n" W;

	static const string							  usage_verbose =
		Y "This is version " B GUMSMAQQER_VERSION Y " of the "
		  "\033[1;33m"
		  "Gumsmaqqer\n" W Y
		  "Converts SGTF (Standard Gumsmaq Transcription Form) to either Unicode or PNG\n\n"
		  "Usage: " B EXECUTABLE_NAME Y " [ OPTIONS ]" G " <sgtf> " Y "[ OPTIONS ]\n"
		  "\n"
		  "  " G "<sgtf>" W " can be either (or multiple of):\n" Y
		  "      [ -s" W ", " Y "--string ] " G "<string>\n" Y
		  "      -f" W ", " Y "--file       " G "<file>\n" W
		  "  where " G "<string>" W " is text in SGTF, with or without quotation marks\n"
		  "  and " G "<file>" W " is a file containing the SGTF text to be converted.\n"
		  "\n"
		  "  OPTIONS:\n"
		  "      " Y "-h" W ", " Y "--help" W "              Display help information. Note that " Y "--help" W " is much more\n"
		  "                              verbose than " Y "-h" W ".\n"
		  "\n"
		  "      " Y "-o" W ", " Y "--out " G "<filename> " W "   Tell the "
		  "\033[1;33m"
		  "Gumsmaqqer" W " to output the first " G "<string> " W "or" G " <file>\n" W
		  "                              to " G "<filename>" W ". If multiple " G "<string>" W "s or  " G "<file>" W "s passed, " Y "--out\n" W
		  "                              may be used multiople times to specify output files\n"
		  "                              for the respective strings, in order. Default: 'gumsmaq.png'.\n"
		  "\n"
		  "                              See also " Y "--merge" W " and " Y "--merge-files\n"
		  "\n"
		  "      -g" W ", " Y "--group-length " G "<n>" W "  Set the number of letters that may appear in a line to " G "<n>\n" W
		  "                              (" G "<n> - 1" W " for indented lines). Must be greater than 0. Default: 10.\n"
		  "\n" Y
		  "      -l" W ", " Y "--lines " G "<n> " W "        Set the number of lines that may appear in a block;\n"
		  "                              note also that is does not matter how many letters a line/group\n"
		  "                              contains. Must be greater than 0. Default: 10.\n"
		  "\n" Y
		  "      -k" W ", " Y "--kern " G "<n>" W "          Set the kerning between letters to " G "<n>" W " pixels. This determines\n"
		  "                              how close together or far apart the letters in a line are printed.\n"
		  "                              Negative values may be specified. Default: -24.\n"
		  "\n"
		  "                              Note that this setting has no effect when in textual output mode.\n"
		  "\n" Y
		  "      -b" W ", " Y "--block-space " G "<n>" W "   Set the space between blocks to " G "<n>" W " pixels. Default: 15.\n"
		  "\n" Y
		  "      -i" W ", " Y "--indent " G "<n>" W "	      Set line indentation to " G "<n>" W " letters for all but the first sub-line\n"
		  "                              in a group. May not be negative and must be less than the value\n"
		  "                              specified for " Y "--group-length" W ". Default: 1.\n"
		  "\n" Y
		  "      -m" W ", " Y "--merge" W "             Merge all input " G "<string>" W "s into one file. The first filename\n"
		  "                              specified with " Y "--out" W " that corresponds to a string is chosen.\n"
		  "                              The remainder is ignored but not taken into account for resolving\n" Y
		  "                              --out" W " filenames for " G "<files>" W "s. Disabled by default.\n"
		  "\n" Y
		  "      -M" W ", " Y "--merge-files" W "       Same as " Y "--merge" W " but for files. Disabled by default.\n"
		  "\n" Y
		  "      -t" W ", " Y "--text" W "              Switch to textual output mode for all following " G "<string>" W "s or " G "<file>" W "s.\n"
		  "                              The SGTF is converted to Unicode letters instead of to an image.\n" G
		  "                              <files>" W " are output to .txt files. " G "<string>" W "s are output to stdout,\n"
		  "                              unless " Y "--out " G "<files>" W "s are specified. Disabled by default.\n"
		  "\n" Y
		  "      -I" W ", " Y "--image" W "             Switch to converting SGTF to a .png instead of text for all following\n" G
		  "                              <string>" W "s and " G "<file>" W "s. This is the opposite of " Y "--text" W ". Enabled by default.\n"
		  "\n"
		  "  EXAMPLES:\n"
		  "      " B EXECUTABLE_NAME W " \"gA4L45T5NACL2\"\n"
		  "      " B EXECUTABLE_NAME Y " -f" W " my_gumsmaq.sgtf.txt " Y "-o " W " compiled.gumsmaq.png\n"
		  "      " B EXECUTABLE_NAME W " \"gA4L45T5NACL2\" " Y "-o" W " output_file.png\n";

	auto help = verbose ? usage_verbose : usage;
	help	  = no_colour ? RemoveColour(help) : help;
	cout << help << flush;
	exit(0);
}

#define SETVAL(errmsg, ...)        \
	if (i >= argc - 1)             \
		FatalArgs(string(errmsg)); \
	else {                         \
		i++;                       \
		__VA_ARGS__;               \
		continue;                  \
	}

#define ALIAS(opt_str) \
	if (!strcmp(argv[i], opt_str))

#define SETNUMBER(option_name, what) SETVAL("Error: missing number after " option_name "", what = Number(argv[i]))

#define DEPTH(depth, parsed_so_far) \
	if (len == depth) FatalArgs("Unrecognised option '" parsed_so_far "'");

void HandleArguments(int argc, char** argv) {
	if (argc == 1) PrintUsage();
	for (int i = 1; i < argc; i++) {
		auto len = strlen(argv[i]);
		switch (argv[i][0]) {
			case '-':
				DEPTH(1, "-")
				// clang-format off
				switch (argv[i][1]) {
					case 'h': print_usage = true; continue;
					case 'o': opt_out: SETVAL("Please specify an output file name after -o", ofilename = argv[i])
					case 'g': opt_group_length: SETNUMBER("-g", gumsmaq_max_letter_group_count)
					case 'l': opt_lines: SETNUMBER("-l", gumsmaq_max_line_count)
					case 'k': opt_kern: SETNUMBER("-k", gumsmaq_letter_kern)
					case 'b': opt_block_space: SETNUMBER("-b", gumsmaq_inter_block_space)
					case 'i': opt_indent: SETNUMBER("-i", gumsmaq_group_indent_count)
					case 'f': opt_file: SETVAL("Please specify an existing file after -f", infiles.push_back(new io::infile(argv[i], PerrorArgs)))
					case 'm': opt_merge: collect = true; continue;
					case 'M': opt_merge_files: collect_files = true; continue;
					case 't': opt_text: textual_mode = true; continue;
					case 'N': opt_no_colour: no_colour = true; continue;
					case 'v': opt_version: PrintVersion();
					case 's':
					opt_string:
						SETVAL("Please specify a string after '-s'", input_text.emplace_back(argv[i]))
					case '-':
						DEPTH(2, "--")
						ALIAS("--help") { print_usage = true; verbose = true; continue; }
						ALIAS("--out") goto opt_out;
						ALIAS("--group-length") goto opt_group_length;
						ALIAS("--lines") goto opt_lines;
						ALIAS("--kern") goto opt_kern;
						ALIAS("--block-space") goto opt_block_space;
						ALIAS("--indent") goto opt_indent;
						ALIAS("--file") goto opt_file;
						ALIAS("--string") goto opt_string;
						ALIAS("--merge") goto opt_merge;
						ALIAS("--merge-files") goto opt_merge_files;
						ALIAS("--text") goto opt_text;
						ALIAS("--no-colour") goto opt_no_colour;
						ALIAS("--version") goto opt_version;
				}
				// clang-format on
			default:
				if (input_text.empty()) input_text.emplace_back(argv[i]);
				else
					FatalArgs("Error: unknown option '" + string(argv[i]) + "'.");
		}
	}
	if (print_usage) PrintUsage();
	if (input_text.empty() && infiles.empty()) FatalArgs("Error: please specify a <string> or <file> to transliterate");
	if (ofilename.empty()) ofilename = "gumsmaq.png";
	gumsmaq_max_letter_group_count = max(1, gumsmaq_max_letter_group_count);
	gumsmaq_max_line_count		   = max(1, gumsmaq_max_line_count);
	gumsmaq_group_indent_count	   = clamp(gumsmaq_group_indent_count, 0, gumsmaq_max_letter_group_count - 1);
}

int main(int argc, char** argv) {
	setlocale(LC_ALL, "");

	HandleArguments(argc, argv);

	// auto gumsmaq = Gumsmaq::VectorFromAbbr(input_text);
	// auto img	 = Gumsmaq::Paragraph(gumsmaq);
	// img.save(ofilename);
}