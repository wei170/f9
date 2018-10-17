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

	if (fread(&nextchar, 1, 1, cmd.inputFile) == EOF) {
        nextchar = EOF_CHAR;
    } else {
        pos.col++;
    }

}

