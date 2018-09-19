/**
 * Tokenizer
 *
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

/************************************************************************/
/*																		*/
/*																		*/
/*						Lexical analyzer								*/
/*																		*/
/*																		*/
/*																		*/
/************************************************************************/

#include "f9cc.h"

int			state;				/* Current state in the lexical FSM		*/

struct tok	nexttok;			/* The next input token. Set by lexan	*/
								/*	and used by the parser				*/
int			toklen;				/* Length of the token					*/

/* Declaration of the keyword table */

struct	keyentry {
	char	*keyword;			/* A keyword in the language			*/
	int		lextype;			/* The lexical type for the keyword		*/
};

struct	keyentry ktab[] = {
	{ "int",     LEXTYPE },
	{ "if",      LEXIF   },
	{ "else",    LEXELSE },
	{ "while",   LEXWHILE},
	{ "program", LEXPGM  },
	{ "read",    LEXREAD },
	{ "print",   LEXFCN  },
	{ "exit",    LEXFCN  }
};


/* Declarations for the FSM */

#include "fsm-defs"

/* Definition of character range */

#define	CHAR_RANGE		257		/* Range of character values: 0 to 255	*/
								/* 	plus one for "EOF"					*/
#define	EOF_CHAR		256

/* FSM actions */

#define	FSM_ACT_SKIP	0	/* Skip this character (no action)			*/
#define	FSM_ACT_SAVE	1	/* Save the current character				*/
#define	FSM_ACT_APP		2	/* Append the current character to the		*/
							/*	saved characters						*/
#define	FSM_ACT_RET		3	/* Have reached the end of a token, so		*/
							/*	return (convert string to				*/
							/*	integer value if LEXNUM)				*/
#define	FSM_ACT_SRET	4	/* Save and return.  Used with single-		*/
							/*	character token, such as comma			*/
#define	FSM_ACT_ERR		5	/* Invalid combination of state and			*/
							/*	input character							*/

/* One entry in the FSM array */

struct	fsmentry {
	int	fsm_nextst;			/* Next state								*/
	int	fsm_action;			/* Action for this transition				*/
	int	fsm_lextyp;			/* For FSM_RET, the token type to return	*/
};

struct	fsmentry fsm[STATES][CHAR_RANGE];

/************************************************************************/
/*																		*/
/*	lexinit  --  initialize the lexical analyzer						*/
/*																		*/
/************************************************************************/

void	lexinit(void) {
	int	s, c, k;

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

	struct	fsmentry *fsmptr;		/* Ptr to current FSM entry			*/
	int		ch;						/* Current input character			*/
	char	*pch;					/* Walks through token string		*/
	int		dig;					/* Next digit						*/
	unsigned int tmp;				/* Used to accumulate a number		*/

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
		}
	}
}
