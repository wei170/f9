#include <f9cc.h>

struct position	pos;

void getch(void) {

	/* Check opened input file */

	if (cmd.inputFile == NULL) return;

    /* Update the position if hit a newline */

	if (nextchar == NEWLINE) {
		pos.row++;
		pos.col = 0;
	}

    /* Read the next character */

	nextchar = fgetc(cmd.inputFile);
	if (nextchar == EOF) {
        nextchar = EOF_CHAR;
		fclose(cmd.inputFile);
    } else {
        pos.col++;
    }
}

