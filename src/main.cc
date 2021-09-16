#ifdef WIN32
#	include "windows.h"
#endif

#include "../lib/io.h"
#include "printer.h"
#include "renderer.h"
#include "utils.h"

#include <clocale>
#include <cstdlib>
#include <cstring>
#include <filesystem>
using namespace std;

bool collect		 = false;
bool collect_files	 = false;
bool textual_mode	 = false;
bool no_colour		 = false;
bool print_usage	 = false;
bool verbose		 = false;
bool to_file		 = false;
bool do_reset_colour = true;

string		assets_dir;
string		ofilename;
io::infile* infile;
string		input_text;

#ifdef WIN32
#	ifdef ENABLE_VIRTUAL_TERMINAL_PROCESSING
static DWORD omode, emode;

void EnableVirtualTerminalProcessing() {
	// enable ansi colour printing
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE err = GetStdHandle(STD_ERROR_HANDLE);
	GetConsoleMode(out, &omode);
	GetConsoleMode(err, &emode);
	SetConsoleMode(out, omode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	SetConsoleMode(err, emode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	do_reset_colour = true;
	reset_colour();
}

void DisableVirtualTerminalProcessing() {
	// enable ansi colour printing
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE err = GetStdHandle(STD_ERROR_HANDLE);
	if (do_reset_colour) reset_colour();
	SetConsoleMode(out, omode);
	SetConsoleMode(err, emode);
	do_reset_colour = false;
}
#	endif
#endif

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

static const string version_info = Y "THIS IS VERSION " B GUMSMAQQER_VERSION Y " OF THE "
									 "\033[1;33mGUMSMAQQER\n" R Y
									 "CONVERT SGTF (STANDARD GUMSMAQ TRANSLITERATION FORM) TO PNG OR UNICODE\n";
#ifdef WIN32
static const string windows_warning = RED "DO NOTE THAT ON THE INFERIOR OPERATING SYSTEM THAT IS\n"
										  "'windows', COMPILATION OF SGTF TO UNICODE IS NOT SUPPORTED.\n"
										  "ENABLE USE OF " Y "--text" RED "BY REPLACING THE OPERATING SYSTEM\nIN QUESTION WITH" G " LINUX\n" RED;
#endif

GUMSMAQ_NORETURN void PrintVersion() {
	cout << version_info << flush;
	exit(0);
}

GUMSMAQ_NORETURN void PrintUsage() {
	static const string usage =
		Y "SUCH AS ARE UTTERLY INCOMPETENT SHALL HAVE RECOURSE TO --HELP\n"
		  "USAGE: " B EXECUTABLE_NAME Y " [ OPTIONS ]" G " <LINES> " Y "[ OPTIONS ]\n"
		  "\n"
		  "  " G "<LINES>" R " SHALL BE " Y "[ -S" R ", " Y "--STRING ] " G "<STRING>" R " OR " Y "-F" R ", " Y "--FILE " G "<FILE>\n" R
		  "\n"
		  "  OPTIONS:\n"
		  "      " Y "-H" R ", " Y "--HELP" R "              ADMISSION OF INEPTITUDE.\n"
		  "      " Y "-O" R ", " Y "--OUT " G "<FILENAME> " R "   WRITE TO " G "<FILENAME>" R ".\n" Y
		  "      -G" R ", " Y "--GROUP-LENGTH " G "<N>" R "  SET GROUP LENGTH TO " G "<N>" R " (" G "<N> - 1" R " WHEN INDENTED).\n" Y
		  "      -L" R ", " Y "--LINES " G "<N> " R "        SET LINES PER BLOCK TO " G "<N>" R "\n" Y
		  "      -K" R ", " Y "--KERN " G "<N>" R "          SET KERNING TO " G "<N>" R " PX.\n" Y
		  "      -B" R ", " Y "--BLOCK-SPACE " G "<N>" R "   SET INTERBLOCK SPACE TO " G "<N>" R " PX.\n" Y
		  "      -I" R ", " Y "--INDENT " G "<N>" R "	      SET INDENTATION TO " G "<N>" R ".\n" Y
		  "      -T" R ", " Y "--TEXT" R "              COMPILE SGTF TO TEXT.\n" Y
		  "      -N" R ", " Y "--NO-COLOUR" R "         MONOCHROMATIC ADMISSION OF INEPTITUDE.\n" R;

	static const string usage_verbose = version_info +
#ifdef WIN32
										windows_warning + "\n\n"
#endif
														  // clang-format off
"USAGE: " B EXECUTABLE_NAME Y " [ OPTIONS ]" G " <SGTF> " Y "[ OPTIONS ]\n"
"\n"
"  " G "<SGTF>" R " SHALL BE:\n" Y "      [ -S" R ", " Y "--STRING ] " G "<STRING>\n" Y "      -F" R ", " Y "--FILE       " G "<FILE>\n" R "  WHERE " G "<STRING>" R " IS TEXT IN SGTF OPTIONALLY ENCLOSED BY" Y " \" " R "TERMINATED BY WHITESPACE\n"
"  AND " G "<FILE>" R " IS A FILE CONTATINING SGTF.\n"
"  SHOULD BOTH BE SPECIFIED, ONLY " G "<FILE>" R " WILL NOT BE DISCARDED."
"\n"
"  OPTIONS:\n"
"      " Y "-H" R ", " Y "--HELP" R "              ADMISSION OF INEPTITUDE. NOTE THAT " Y "--HELP" R " IS MUCH MORE\n"
"                              VERBOSE THAN " Y "-H" R ".\n"
"\n"
"      " Y "-O" R ", " Y "--OUT " G "<FILENAME> " R "   INSTRUCT THE \033[1;33mGUMSMAQQER" R " TO WRITE TO " G "<FILENAME>\n" W "                              INITIAL: 'GUMSMAQQED.PNG'" R ".\n"
"\n" Y "      -G" R ", " Y "--GROUP-LENGTH " G "<N>" R "  SET NUMBER OF LETTERS THAT MAY APPEAR IN A SINGLE LINE TO " G "<N>\n" R "                              (" G "<N> - 1" R " WHEN INDENTED). MUST EXCEED 0." W " INITIAL: 10" R ".\n"
"\n" Y "      -L" R ", " Y "--LINES " G "<N> " R "        SET NUMBER OF LINES THAT MAY APPEAR IN A SINGLE BLOCK;\n"
"                              MUST EXCEED 0. " W "INITIAL: 10" R ".\n"
"\n" Y "      -K" R ", " Y "--KERN " G "<N>" R "          SET KERNING BETWEEN LETTERS TO " G "<N>" R " PIXELS. DETERMINES\n"
"                              HOW CLOSE TOGETHER OR FAR APART LETTERS IN A LINE ARE PRINTED.\n"
"                              NEED NOT EXCEED 0. " W "INITIAL: -24" R ".\n"
"\n"
"                              SETTING IS EMPTY AND VACUOUS IN TEXTUAL OUTPUT MODE.\n"
"\n" Y "      -B" R ", " Y "--BLOCK-SPACE " G "<N>" R "   SET INTERBLOCK SPACE TO " G "<N>" R " PIXELS. " W "INITIAL: 15" R ".\n"
"\n" Y "      -I" R ", " Y "--INDENT " G "<N>" R "	      SET LINE INDENTATION TO " G "<N>" R " LETTERS FOR ALL BUT THE FIRST SUBLINE\n"
"                              IN A GROUP. MUST EXCEED 0 AND MAY NOT EXCEED VALUE\n"
"                              SPECIFIED FOR " Y "--GROUP-LENGTH" R ". " W "INITIAL: 1" R ".\n"
"\n" Y "      -T" R ", " Y "--TEXT" R "              ENABLE TEXTUAL OUTPUT MODE. SGTF IS COMPILED TO UNICODE LETTERS.\n"
"                              INSTEAD OF AN IMAGE. " W "INITIAL: DISABLED" R ".\n"
"\n" Y "      -N" R ", " Y "--NO-COLOUR" R "         PRINT MONOCHROMATIC ADMISSION OF INEPTITUDE. " W "INITIAL: DISABLED" R "."
"\n"
"  EXAMPLES:\n"
"      " B EXECUTABLE_NAME R " \"gA4L45T5NACL2\"\n"
//		  "      " B EXECUTABLE_NAME Y " -F" R " GUMSMAQ.SGTF.TXT " Y "-O " R " COMPILED.PNG\n"
"      " B EXECUTABLE_NAME R " \"gA4L45T5NACL2\" " Y "-O" R " OUTPUT.PNG\n";
	// clang-format on
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
	if (argc == 1) FatalArgs("AND IF THOU GAZE LONG INTO AN ABYSS, THE ABYSS WILL GAZE BACK INTO THEE");
	for (int i = 1; i < argc; i++) {
		auto len = strlen(argv[i]);
		// clang-format off
		switch (argv[i][0]) {
			case '-':
				DEPTH(1, "-")
				switch (argv[i][1]) {
					case 'H': print_usage = true; continue;
					case 'O': opt_out: SETVAL("MUST NEEDS SPECIFY OUTPUT FILE NAME AFTER " Y "-O" R, ofilename = argv[i]; to_file = true)
					case 'G': opt_group_length: SETNUMBER("-G", gumsmaq_max_letter_group_count)
					case 'L': opt_lines: SETNUMBER("-L", gumsmaq_max_line_count)
					case 'K': opt_kern: SETNUMBER("-K", gumsmaq_letter_kern)
					case 'B': opt_block_space: SETNUMBER("-B", gumsmaq_inter_block_space)
					case 'I': opt_indent: SETNUMBER("-I", gumsmaq_group_indent_count)
					case 'F': opt_file: FatalArgs("OPTION " R Y "-F" RED " CURRENTLY NOT SUPPORTED"); //SETVAL("MUST NEED SPECIFY EXISTING FILE AFTER " Y "-F" R, infile = new io::infile(argv[i], PerrorArgs))
#ifndef WIN32
					case 'T': opt_text: textual_mode = true; continue;
#else
					case 'T': opt_text: fatal(RED, windows_warning);
#endif
					case 'N': opt_no_colour: no_colour = true; continue;
					case 'V': opt_version: PrintVersion();
					case 'S':
					opt_string:
						SETVAL("MUST NEEDS SPECIFY STRING AFTER " Y "-S" R, SetText(input_text, argv[i]))
					case '-':
						DEPTH(2, "--")
						ALIAS("--HELP") { print_usage = true; verbose = true; continue; }
						ALIAS("--OUT") goto opt_out;
						ALIAS("--GROUP-LENGTH") goto opt_group_length;
						ALIAS("--LINES") goto opt_lines;
						ALIAS("--KERN") goto opt_kern;
						ALIAS("--BLOCK-SPACE") goto opt_block_space;
						ALIAS("--INDENT") goto opt_indent;
						ALIAS("--FILE") goto opt_file;
						ALIAS("--STRING") goto opt_string;
						ALIAS("--TEXT") goto opt_text;
						ALIAS("--NO-COLOUR") goto opt_no_colour;
						ALIAS("--VERSION") goto opt_version;
					default: FatalArgs("BAD OPTION \"" Y + string(argv[i]) + RED "\"" R);
				}

			default:
				if (input_text.empty()) SetText(input_text, argv[i]);
				else FatalArgs("BAD OPTION \"" Y + string(argv[i])
				+ RED "\"\nHERETICALLY UNCONJOINED GUMSMAQ SHALL NOT BE TOLERATED");
		}
		// clang-format on
	}
	if (print_usage) PrintUsage();
	if (input_text.empty() && !infile) FatalArgs("AND IF THOU GAZE LONG INTO AN ABYSS, THE ABYSS WILL GAZE BACK INTO THEE");
	if (ofilename.empty()) ofilename = "gumsmaqqed.png";
	gumsmaq_max_letter_group_count = max(1, gumsmaq_max_letter_group_count);
	gumsmaq_max_line_count		   = max(1, gumsmaq_max_line_count);
	gumsmaq_group_indent_count	   = clamp(gumsmaq_group_indent_count, 0, gumsmaq_max_letter_group_count - 1);
}

int main(int argc, char** argv) {
#if defined(WIN32) && defined(ENABLE_VIRTUAL_TERMINAL_PROCESSING)
	atexit(DisableVirtualTerminalProcessing);
#endif

	setlocale(LC_ALL, "en_US.utf-8");

#if defined(WIN32) && defined(ENABLE_VIRTUAL_TERMINAL_PROCESSING)
	EnableVirtualTerminalProcessing();
#endif

	if (filesystem::exists("./twemoji/assets/72x72")) assets_dir = "./twemoji/assets/72x72";
	else if (filesystem::exists("./assets/72x72"))
		assets_dir = "./assets/72x72";
	else if (filesystem::exists("./assets"))
		assets_dir = "./assets";
	else if (filesystem::exists("./72x72"))
		assets_dir = "./72x72";
	else {
		char* _assets_dir = std::getenv("GUMSMAQQER_ASSETS_DIR");
#ifndef WIN32
		string pwd{filesystem::current_path().c_str()};
#else
		string pwd;
		pwd.reserve(100000);
		wcstombs(pwd.data(), filesystem::current_path().c_str(), 100000);
#endif
		if (!_assets_dir) fatal(RED, string("GUMSMAQQING ASSETS FOUND WANTING\nENSURE EXISTENCE OF ASSETS DIRECTORY " Y "assets" RED " CONTAINING ASSETS IN " Y) + pwd + RED "\nELSE ENVIRONMENT VARIABLE " Y "GUMSMAQQER_ASSETS_DIR" RED " MUST BE SET TO ASSETS DIRECTORY " Y "assets" RED);
		assets_dir = _assets_dir;
	}

	if (!assets_dir.ends_with('/')) assets_dir += '/';

	HandleArguments(argc, argv);

	auto sgtf = input_text.empty() ? infile->mmap() : input_text;

	if (sgtf.find(' ') != string::npos) {
		fatal(RED, "HERETICALLY UNCONJOINED GUMSMAQ SHALL NOT BE TOLERATED");
	}

	auto gumsmaq = Visual::VectorFromAbbr(sgtf);

	if (textual_mode) {
		auto text = ToUTF8(Textual::Text(gumsmaq));
		if (!to_file) cout << text;
		else
			io::ofile(ofilename, io::perror_and_exit).write(text);
	} else {
		auto img = Visual::Paragraph(gumsmaq);
		img.save(ofilename);
	}
}