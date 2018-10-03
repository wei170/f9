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
#define	LEXCMPOP	17		/* Camparison operation							*/
#define	LEXARIOP	18		/* Arithmetic operation							*/
#define	LEXLOG		19		/* Logic										*/
#define	LEXSEMI		20		/* Semicolon									*/
#define	LEXCOMMA	21		/* Comma										*/
#define	LEXSTR		22		/* Strings										*/
#define	LEXEOF		23		/* End of file									 */

/* Globals shared by lexical analyzer and parser */
#define	MAXTOK		256

typedef	struct	tok {		/* A token as returned by the lexical analyzer	*/
	int		tok_typ;		/* One of the types defined above				*/
	char	tok_str[MAXTOK];/* The actual string							*/
	int		tok_val;		/* Value of the token is a number				*/
} tok_f9;

typedef	struct	Command {
	FILE	*inputFile;		/* Input file									*/
	FILE	*outputFile; 	/* Output file									*/
} command_f9;

extern	tok_f9 nexttok;		/* The next input token							*/

extern command_f9 cmd;		/* The command									*/


// lex.c
extern	void	lexinit(void);

extern	void	gettoken(void);

extern	void	getch(void);

extern	char	nextchar;

// extern	int	symlookup(char *);
#endif

