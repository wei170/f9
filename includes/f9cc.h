#ifndef _F9CCH_
#define _F9CCH_

/****************************************************************************/
/*																			*/
/*																			*/
/*				Basic definitions used throughout the compiler				*/
/*																			*/
/*																			*/
/****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define	DEBUG		0

/* Lexical token types */

#define	LEXUNUSED	1       /* An unused value (should never be returned)	*/
#define	LEXNAME		2       /* Identifier or keyword						*/
#define	LEXID		3       /* Identifier									*/
#define	LEXNUM		4       /* A number										*/
#define	LEXTYPE		5       /* 'int'										*/
#define	LEXPRG		6       /* 'program'									*/
#define	LEXIF		7       /* 'if'											*/
#define	LEXELSE		8       /* 'else'										*/
#define	LEXWHILE	9       /* 'while'										*/
#define	LEXREAD		10      /* 'read'										*/
#define	LEXPGM		11		/* 'program'									*/
#define	LEXFCN		12      /* functions, ie print exit						*/
#define	LEXLP		13		/* Left parenthesis								*/
#define	LEXRP		14		/* Right parenthesis							*/
#define	LEXLB		15      /* Left Brace									*/
#define	LEXRB		16      /* Right Brace									*/
#define	LEXLBK		17		/* Left Bracket									*/
#define	LEXRBK		18		/* Right Bracket								*/
#define	LEXCMPOP	19		/* Camparison operation							*/
#define	LEXARIOP	20		/* Arithmetic operation							*/
#define	LEXLOGOP	21		/* Logic operation								*/
#define	LEXSEMI		22		/* Semicolon									*/
#define	LEXCOMMA	23		/* Comma										*/
#define	LEXSTR		24		/* Strings										*/
#define	LEXBOOL		25		/* Boolean macros								*/
#define	LEXEOF		26		/* End of file									*/

/* Declarations for the FSM */

#include "fsm-defs"

/* Definition of character range */

#define	CHAR_RANGE		257		/* Range of character values: 0 to 255	*/
								/* 	plus one for "EOF"					*/
#define	EOF_CHAR		'\0'
#define	TAB				'\t'
#define	NEWLINE			'\n'
#define	BLANK			' '

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
#define FSM_ACT_ARET    5   /* Append and return,  Used with multi-		*/
							/*  character token, such as +=				*/
#define	FSM_ACT_ERR		6	/* Invalid combination of state and			*/
							/*	input character							*/

/* Symbol types */
#define	SYMERR			0	/* Not recoganized symbol type				*/
#define	SYMINT			1	/* 'int'									*/
#define	SYMSTR			2	/* 'string'									*/
#define	SYMBOOL			3	/* 'boolean'									*/

/* Globals shared by lexical analyzer and parser */

#define	MAXTOK		1024	/* Maximum size of a token (possibly a string)	*/
#define	MAXNAME		24		/* Maximum variable name length					*/
#define	MAXINT		0x7fffffff  /* Maximum positive integer					*/
#define	MAXARGS		200		/* Maximum arguments to a function				*/
#define	MAXSYM		1024	/* Maximum symbols in the entire program		*/
#define	MAXFRAME	512

/****************************/
/*							*/
/*		Structures			*/
/*							*/
/****************************/

struct	tok {				/* A token as returned by the lexical analyzer	*/
	int		tok_typ;		/* One of the types defined above				*/
	char	tok_str[MAXTOK];/* The actual string							*/
	int		tok_val;		/* Value of the token is a number				*/
};

struct	command {
	FILE	*inputFile;		/* Input file									*/
	FILE	*outputFile; 	/* Output file									*/
	char	output[MAXFRAME];	/* Output file name							*/
};

struct	position {			/* Position of the next character				*/
	int		row;
	int		col;
};

struct	keyentry {          /* Keyword table                                */
	char	*keyword;		/* A keyword in the language			        */
	int		lextype;		/* The lexical type for the keyword		        */
};

struct	fsmentry {          /* FSM Entry                                    */
	int		fsm_nextst;		/* Next state								    */
	int		fsm_action;		/* Action for this transition				    */
	int		fsm_lextyp;		/* For FSM_RET, the token type to return	    */
};

struct symentry {			/* Symbol Entry									*/
	char	sym_name[MAXNAME]; /* Name of the symbol						*/
	int		sym_type;		/* Type of the symbol, ie. int					*/
	int		sym_size;		/* Size											*/
};

/****************************/
/*							*/
/*		Variables			*/
/*							*/
/****************************/

extern	struct tok		nexttok;		/* The next input token				*/

extern	struct command	cmd;			/* Data from command input			*/

extern	char			nextchar;		/* The next read character			*/

extern	struct position	pos;			/* Postion for the nextchar			*/

extern  struct fsmentry fsm[STATES][CHAR_RANGE];

extern  struct keyentry ktab[12];

extern	struct symentry	symtab[MAXSYM];	/* The symbol table					*/

extern	int				nsyms;			/* Numbers of symbol in the table	*/

/****************************/
/*							*/
/*		Functions			*/
/*							*/
/****************************/

/* in file main.c */
extern	void	errmsg(char *);

/* in file lex.c */
extern	void	lexinit(void);

extern	void	gettoken(void);

/* in file file.c */
extern	void	getch(void);

/* in file parser.c */
extern	void	parser(void);

/* in file symbol.c */
extern	int		addsym(char *, int, int);
extern	int		symlookup(char *);
extern	int		symtypelookup(char *);

#endif

