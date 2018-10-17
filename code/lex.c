/**
 * Tokenizer
 *
 */

#include <f9cc.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

/************************************************************************/
/*																		*/
/*																		*/
/*						Lexical analyzer								*/
/*																		*/
/*																		*/
/*																		*/
/************************************************************************/

int			state;				/* Current state in the lexical FSM		*/

struct tok	nexttok;			/* The next input token. Set by lexan	*/
								/*	and used by the parser				*/
int			toklen;				/* Length of the token					*/

struct	keyentry ktab[] = {     /* Declaration of the keyword table     */
	{ "int",     LEXTYPE },
	{ "if",      LEXIF   },
	{ "else",    LEXELSE },
	{ "while",   LEXWHILE},
	{ "program", LEXPGM  },
	{ "read",    LEXREAD },
	{ "print",   LEXFCN  },
	{ "exit",    LEXFCN  }
};

struct	fsmentry fsm[STATES][CHAR_RANGE]; /* One entry in the FSM array */

void        lookup(void);

/************************************************************************/
/*																		*/
/*	lexinit  --  initialize the lexical analyzer						*/
/*																		*/
/************************************************************************/

void	lexinit(void) {
	int	s, c;

	/* Initialize the FSM */

	for (s=0; s<STATES; s++) {
		for (c=0; c<CHAR_RANGE; c++) {
			fsm[s][c].fsm_nextst = 0;
			fsm[s][c].fsm_action = FSM_ACT_ERR;
			fsm[s][c].fsm_lextyp = LEXUNUSED;
		}
	}

	/* Fill in  FSM entries */

	#include "fsm-code"

	/* Initialize nextchar */
	getch();
	if (nextchar == EOF_CHAR) {
		fprintf(stderr, "Input file is empty\n");
		exit(1);
	}

	state = FSM_STATE_INIT;

}



/************************************************************************/
/*																		*/
/* gettok  --  perform lexical analysis to set nexttok to the next		*/
/*				token													*/
/*																		*/
/************************************************************************/

void	gettoken(void) {

	struct		fsmentry *fsmptr;	/* Ptr to current FSM entry		*/
	int			ch;					/* Current input character		*/
	char		*pch;				/* Walks through token string	*/
	int			dig;				/* Next digit					*/
	unsigned	int tmp;			/* Used to accumulate a number	*/

	while (1) {
		ch = nextchar;
		fsmptr = &fsm[state][ch];
		state = fsmptr->fsm_nextst;

		switch (fsmptr->fsm_action) {
			case FSM_ACT_SKIP:
				if (ch != EOF_CHAR) {
					getch();
				}
				break;

			case FSM_ACT_SAVE:
				nexttok.tok_str[0] = ch;
				toklen = 1;
				getch();
				break;

			case FSM_ACT_APP:
				if (toklen >= MAXTOK-1) {
					errmsg("Token is too long");
				}
				nexttok.tok_str[toklen++] = ch;
				getch();
				break;

			case FSM_ACT_RET:
				nexttok.tok_str[toklen] = '\0';
				nexttok.tok_typ = fsmptr->fsm_lextyp;

				/* Convert numeric values and look up names */

				if (fsmptr->fsm_lextyp == LEXNUM) {
					/* Convert to numeric value and check */
					if (nexttok.tok_str[1] == 'x') { /* Hex */
						pch = &nexttok.tok_str[2];
						for (tmp=0; *pch != '\0'; pch++) {
							if ((*pch>='0')&&(*pch<='9')) {
								dig = *pch-'0';
							} else if ((*pch>='A')&&(*pch<='F')) {
								dig = 10 + (*pch - 'A');
							} else {
								dig = 10 + (*pch - 'a');
							}
							tmp = (tmp<<4) | dig;
							if (tmp > MAXINT) {
								errmsg("Invalid hex constant");
							}
						}
						nexttok.tok_val = tmp;
					} else { /* Decimal */
						pch = &nexttok.tok_str[0];
						for (tmp=0; *pch != '\0'; pch++) {
							dig = *pch-'0';
							tmp = 10*tmp + dig;
							if (tmp > MAXINT) {
								errmsg("Invalid decimal constant");
							}
						}
						nexttok.tok_val = tmp;
					}
				} else if (fsmptr->fsm_lextyp == LEXNAME) {
					lookup();
				}
				return;

			case FSM_ACT_SRET:
				nexttok.tok_str[0] = nextchar;
				toklen = 1;
				nexttok.tok_str[toklen] = '\0';
				nexttok.tok_typ = fsmptr->fsm_lextyp;
				getch();
				return;

			case FSM_ACT_ERR:
				errmsg("Invalid input (not a valid token)");
		}

	}
	return;
}

/************************************************************************/
/*																		*/
/* lookup  --  lookup the name inthe current token to see if it is a	*/
/*				keyword, and if som], replace the token type			*/
/*																		*/
/************************************************************************/

void	lookup(void) {

	int	k;		/* Index in the keyword table		*/
	int	nkeys;	/* Number of keywords in the table	*/

	nkeys = sizeof(ktab)/sizeof(struct keyentry);

	for (k=0; k < nkeys; k++) {
		if (strcmp(ktab[k].keyword, nexttok.tok_str) == 0) {
			/* Match */
			nexttok.tok_typ = ktab[k].lextype;
			break;
		}
	}
	return;
}
