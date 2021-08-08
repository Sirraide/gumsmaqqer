#ifdef WIN32
#	include "windows.h"
#endif

#include "../lib/io.h"
#include "printer.h"
#include "renderer.h"
#include "utils.h"

#include <clocale>
#include <cstring>
using namespace std;

int gumsmaq_max_letter_group_count = 10;
int gumsmaq_max_line_count		   = 10;
int gumsmaq_letter_kern			   = -(72 / 3);
int gumsmaq_group_indent_count	   = 1;
int gumsmaq_inter_block_space	   = 1;
int gumsmaq_max_block_count		   = 1;

bool collect	   = false;
bool collect_files = false;
bool textual_mode  = false;
bool no_colour	   = false;
bool print_usage   = false;
bool verbose	   = false;
bool to_file	   = false;

#ifndef WIN32
uchar_t unewline[CHARS_IN_NEWLINE] = {L'\n'};
#else
uchar_t unewline[CHARS_IN_NEWLINE] = {L'\r', L'\n'};
#endif

string		ofilename;
io::infile* infile;
string		input_text;

const string help_message = RED "\nEIDOLA OF PRODIGIOUS INEPTITUDE MAY AVAIL THEIR PITIFUL SELVES OF " R Y "-h" R;

#ifdef WIN32
void WindowsInit() {
// enable ansi colour printing
#	ifdef ENABLE_VIRTUAL_TERMINAL_PROCESSING
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE err = GetStdHandle(STD_ERROR_HANDLE);
	DWORD  omode, emode;
	GetConsoleMode(out, &omode);
	GetConsoleMode(err, &emode);
	SetConsoleMode(out, omode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	SetConsoleMode(err, emode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#	endif
}
#endif

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

GUMSMAQ_NORETURN inline void FatalArgs(const string& errmsg) {
	string err = RED + errmsg;
#if !defined(WIN32) || defined(ENABLE_VIRTUAL_TERMINAL_PROCESSING)
	err = no_colour ? RemoveColour(err) : err;
#else
	err	 = RemoveColour(err);
#endif
	fatal(RED, err);
}

GUMSMAQ_NORETURN void PerrorArgs(const string& filename) {
	std::string s = RED "BAD FILE " G + filename + R;
	perror(s.c_str());
	fatal(RED, "");
}

GUMSMAQ_NORETURN void PrintVersion() {
	static const string version_info = Y "This is version " B GUMSMAQQER_VERSION Y " of the "
										 "\033[1;33mGUMSMAQQER\n" R Y
										 "Converts SGTF (Standard Visual Transcription Form) to either Unicode or PNG\n"
										 "See also --help for more detailed information\n" R;
	cout << version_info << flush;
	exit(0);
}

GUMSMAQ_NORETURN void PrintUsage() {
	static const string usage =
		Y "See also --help for more detailed information\n\n"
		  "Usage: " B EXECUTABLE_NAME Y " [ OPTIONS ]" G " <lines> " Y "[ OPTIONS ]\n"
		  "\n"
		  "  " G "<lines>" R " can be " Y "[ -s" R ", " Y "--string ] " G "<string>" R " or " Y "-f" R ", " Y "--file " G "<file>\n" R
		  "\n"
		  "  OPTIONS:\n"
		  "      " Y "-h" R ", " Y "--help" R "              Display help information. Note that " Y "--help" R " is much more\n"
		  "                              verbose than " Y "-h" R ".\n"
		  "      " Y "-o" R ", " Y "--out " G "<filename> " R "   Tell the "
		  "\033[1;33m"
		  "Gumsmaqqer" R " to output to " G "<filename>" R ".\n" Y
		  "      -g" R ", " Y "--group-length " G "<n>" R "  Set the group length to " G "<n>" R " (" G "<n> - 1" R " for indented lines).\n" Y
		  "      -l" R ", " Y "--lines " G "<n> " R "        Set the number of lines that may appear in a block;\n" Y
		  "      -k" R ", " Y "--kern " G "<n>" R "          Set the kerning between letters to " G "<n>" R " pixels.\n" Y
		  "      -b" R ", " Y "--block-space " G "<n>" R "   Set the space between blocks to " G "<n>" R " pixels.\n" Y
		  "      -i" R ", " Y "--indent " G "<n>" R "	      Set line indentation to " G "<n>" R ".\n" Y
		  "      -t" R ", " Y "--text" R "              Enable textual output mode.\n" Y
		  "      -N" R ", " Y "--no-colour" R "         Print help information without colours.\n" R;

	static const string							  usage_verbose =
		Y "This is version " B GUMSMAQQER_VERSION Y " of the "
		  "\033[1;33m"
		  "Gumsmaqqer\n" R Y
		  "Converts SGTF (Standard Visual Transcription Form) to either Unicode or PNG\n\n"
		  "Usage: " B EXECUTABLE_NAME Y " [ OPTIONS ]" G " <sgtf> " Y "[ OPTIONS ]\n"
		  "\n"
		  "  " G "<sgtf>" R " can be either:\n" Y
		  "      [ -s" R ", " Y "--string ] " G "<string>\n" Y
		  "      -f" R ", " Y "--file       " G "<file>\n" R
		  "  where " G "<string>" R " is text in SGTF, with or without quotation marks\n"
		  "  and " G "<file>" R " is a file containing the SGTF text to be converted.\n"
		  "  if both are specified, only " G "<file>" R " is converted."
		  "\n"
		  "  OPTIONS:\n"
		  "      " Y "-h" R ", " Y "--help" R "              Display help information. Note that " Y "--help" R " is much more\n"
		  "                              verbose than " Y "-h" R ".\n"
		  "\n"
		  "      " Y "-o" R ", " Y "--out " G "<filename> " R "   Tell the "
		  "\033[1;33m"
		  "GUMSMAQQER" R " to output the first " G "<string> " R "or" G " <file>\n" R
		  "                              to " G "<filename>." W
		  "Default: 'gumsmaqqed.png'" R ".\n"
		  "\n" Y
		  "      -g" R ", " Y "--group-length " G "<n>" R "  Set the number of letters that may appear in a line to " G "<n>\n" R
		  "                              (" G "<n> - 1" R " for indented lines). Must be greater than 0." W " Default: 10" R ".\n"
		  "\n" Y
		  "      -l" R ", " Y "--lines " G "<n> " R "        Set the number of lines that may appear in a block;\n"
		  "                              note also that is does not matter how many letters a line/group\n"
		  "                              contains. Must be greater than 0. " W "Default: 10" R ".\n"
		  "\n" Y
		  "      -k" R ", " Y "--kern " G "<n>" R "          Set the kerning between letters to " G "<n>" R " pixels. This determines\n"
		  "                              how close together or far apart the letters in a line are printed.\n"
		  "                              Negative values may be specified. " W "Default: -24" R ".\n"
		  "\n"
		  "                              Note that this setting has no effect when in textual output mode.\n"
		  "\n" Y
		  "      -b" R ", " Y "--block-space " G "<n>" R "   Set the space between blocks to " G "<n>" R " pixels. " W "Default: 15" R ".\n"
		  "\n" Y
		  "      -i" R ", " Y "--indent " G "<n>" R "	      Set line indentation to " G "<n>" R " letters for all but the first sub-line\n"
		  "                              in a group. May not be negative and must be less than the value\n"
		  "                              specified for " Y "--group-length" R ". " W "Default: 1" R ".\n"
		  "\n" Y
		  "      -t" R ", " Y "--text" R "              Enable textual output mode. The SGTF is converted to Unicode letters.\n"
		  "                              instead of to an image. " W "Default: disabled" R ".\n"
		  "\n" Y
		  "      -N" R ", " Y "--no-colour" R "         Print help information without colours. Does nothing otherwise. " W "Default:\n"
		  "                              disabled" R "."
		  "\n"
		  "  EXAMPLES:\n"
		  "      " B EXECUTABLE_NAME R " \"gA4L45T5NACL2\"\n"
		  "      " B EXECUTABLE_NAME Y " -f" R " my_gumsmaq.sgtf.txt " Y "-o " R " compiled.gumsmaq.png\n"
		  "      " B EXECUTABLE_NAME R " \"gA4L45T5NACL2\" " Y "-o" R " output_file.png\n";

	auto help = verbose ? usage_verbose : usage;
#if !defined(WIN32) || defined(ENABLE_VIRTUAL_TERMINAL_PROCESSING)
	help = no_colour ? RemoveColour(help) : help;
#else
	help = RemoveColour(help);
#endif
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

#define SETNUMBER(option_name, what) SETVAL("MISSING VALUE\nMUST NEEDS PASS NUMBER AFTER " Y option_name R, what = Number(argv[i]))

#define DEPTH(depth, parsed_so_far) \
	if (len == depth) FatalArgs("BAD OPTION \"" Y parsed_so_far RED "\"" R);

void HandleArguments(int argc, char** argv) {
	if (argc == 1) FatalArgs("\"AND IF THOU GAZE LONG INTO AN ABYSS, THE ABYSS WILL GAZE BACK INTO THEE\"");
	for (int i = 1; i < argc; i++) {
		auto len = strlen(argv[i]);
		// clang-format off
		switch (argv[i][0]) {
			case '-':
				DEPTH(1, "-")
				switch (argv[i][1]) {
					case 'h': print_usage = true; continue;
					case 'o': opt_out: SETVAL("MUST NEEDS SPECIFY OUTPUT FILE NAME AFTER " Y "-o" R, ofilename = argv[i]; to_file = true)
					case 'g': opt_group_length: SETNUMBER("-g", gumsmaq_max_letter_group_count)
					case 'l': opt_lines: SETNUMBER("-l", gumsmaq_max_line_count)
					case 'k': opt_kern: SETNUMBER("-k", gumsmaq_letter_kern)
					case 'b': opt_block_space: SETNUMBER("-b", gumsmaq_inter_block_space)
					case 'i': opt_indent: SETNUMBER("-i", gumsmaq_group_indent_count)
					case 'f': opt_file: FatalArgs("OPTION " R Y "-f" RED " CURRENTLY NOT SUPPORTED"); //SETVAL("Please specify an existing file after " Y "-f" R, infile = new io::infile(argv[i], PerrorArgs))
					case 't': opt_text: textual_mode = true; continue;
					case 'N': opt_no_colour: no_colour = true; continue;
					case 'v': opt_version: PrintVersion();
					case 's':
					opt_string:
						SETVAL("MUST NEEDS SPECIFY STRING AFTER " Y "-s" R, SetText(input_text, argv[i]))
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
						ALIAS("--text") goto opt_text;
						ALIAS("--no-colour") goto opt_no_colour;
						ALIAS("--version") goto opt_version;
					default: FatalArgs("BAD OPTION \"" Y + string(argv[i]) + RED "\"" R);
				}

			default:
				if (input_text.empty()) SetText(input_text, argv[i]);
				else FatalArgs("BAD OPTION \"" Y + string(argv[i])
							   + R "\"\nDO NOTE THAT ONLY ONE STRING MAY BE SPECIFIED");
		}
		// clang-format on
	}
	if (print_usage) PrintUsage();
	if (input_text.empty() && !infile) FatalArgs("\"AND IF THOU GAZE LONG INTO AN ABYSS, THE ABYSS WILL GAZE BACK INTO THEE\"");
	if (ofilename.empty()) ofilename = "gumsmaqqed.png";
	gumsmaq_max_letter_group_count = max(1, gumsmaq_max_letter_group_count);
	gumsmaq_max_line_count		   = max(1, gumsmaq_max_line_count);
	gumsmaq_group_indent_count	   = clamp(gumsmaq_group_indent_count, 0, gumsmaq_max_letter_group_count - 1);
}

int main(int argc, char** argv) {
#if !defined(WIN32) || defined(ENABLE_VIRTUAL_TERMINAL_PROCESSING)
	atexit(reset_colour);
#endif

	setlocale(LC_ALL, "");

#ifdef WIN32
	WindowsInit();
#endif

	HandleArguments(argc, argv);

	auto sgtf	 = input_text.empty() ? infile->mmap() : input_text;
	auto gumsmaq = Visual::VectorFromAbbr(sgtf);

	if (textual_mode) {
		auto t = Textual::Text(gumsmaq);
		if (!to_file) wcout << t;
		else {
			io::wofile out(ofilename, io::perror_and_exit);
			for(auto uc : t) out.write(uc);
		}

	} else {
		auto img = Visual::Paragraph(gumsmaq);
		img.save(ofilename);
	}
}