/**
 * Main
 */
#include <f9cc.h>

struct command	cmd;
char			nextchar;
char			err[] = "Save errors into the .tmpf9err\n";

static void usage(int exitcode) {
		fprintf(exitcode ? stderr : stdout,
			"Usage: f9cc [ -o ] <file>\n"
			"\n"
			"  -o filename        Output to the specified file\n"
			"\n");
		exit(exitcode);
}

static void parseOpt(int argc, char **argv) {
	int		opt;

	while ((opt = getopt(argc, argv, "o:")) != -1) {
		switch (opt) {
			case 'o':
				strcpy(cmd.output, optarg);
				cmd.outputFile = fopen(".tmpf9.c", "w+");
				break;
			default:
				usage(1);
		}
	}

	if (optind != argc - 1) usage(1);

	cmd.inputFile = fopen(argv[optind], "r");
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
	char	syscmd[1024];
	int		retcode;			/* Return code from 'system' */

	parseOpt(argc, argv);
	parser();

	fclose(cmd.outputFile);

	/* Run gcc to produce an object flie */

	sprintf(syscmd, "gcc -Wall -Waddress .tmpf9.c -o %s", cmd.output);
	retcode = system(syscmd);
	// unlink(".tmpf9.c");

	exit(0);
}

