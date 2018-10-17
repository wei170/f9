/**
 * Main
 */
#include <f9cc.h>
#include <unistd.h>
#include <string.h>

struct command	cmd;
char			nextchar;

static void usage(int exitcode) {
		fprintf(exitcode ? stderr : stdout,
			"Usage: f9cc [ -o ] <file>\n"
			"\n"
			"  -o filename        Output to the specified file\n"
			"\n");
		exit(exitcode);
}

static void parseOpt(int argc, char **argv) {
	for (;;) {
		int opt = getopt(argc, argv, "o");
		if (opt == -1) break;
		switch(opt) {
			case 'o':
				cmd.outputFile = fopen(optarg, "w");
				break;
			default:
				usage(1);
		}
	}

	if (optind != argc - 2) usage(1);

	cmd.inputFile = fopen(argv[optind+1], "r");
	if (cmd.inputFile == NULL) {
		fprintf(stderr, "%s does not exist.\n", argv[optind]);
		exit(1);
	}
}

/************************************************************************/
/*																		*/
/* errmsg  --  print an error message and quit							*/
/*																		*/
/************************************************************************/

void    errmsg(char *msg) {
	fprintf(stderr, "error on line %d at position %d: %s\n\n",
			pos.row, pos.col, msg);
	exit(1);
}


int main(int argc, char **argv) {
	parseOpt(argc, argv);
	parser();
}

