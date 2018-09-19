/****************************************************************************/
/*																			*/
/*																			*/
/*				Basic definitions used throughout the compiler				*/
/*																			*/
/*																			*/
/****************************************************************************/

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
#define	LEXFCN		11      /* functions, ie print exit						*/
#define	LEXLP		12		/* Left parenthesis								*/
#define	LEXRP		13		/* Right parenthesis							*/
#define	LEXLB		14      /* Left Brace									*/
#define	LEXRB		15      /* Right Brace									*/
#define	LEXCMPOP	16		/* Camparison operation							*/
#define	LEXARIOP	17		/* Arithmetic operation							*/
#define	LEXLOG		18		/* Logic										*/
#define	LEXSEMI		19		/* Semicolon									*/
#define	LEXCOMMA	20		/* Comma										*/

/* Globals shared by lexical analyzer and parser */

struct	tok {				/* A token as returned by the lexical analyzer	*/
	int		tok_typ;		/* One of the types defined above				*/
	char	tok_str[MAXTOK];/* The actual string							*/
	int		tok_val;		/* Value of the token is a number				*/
};

extern	struct	tok nexttok;/* The next input token							*/

struct	Command {
	FILE	*inputFile;		/* Input file									*/
	FILE	*outputFile; 	/* Output file									*/
}

extern struct Command cmd;	/* The command									*/



// lex.c
extern	void	lexinit(void);

extern	void	gettoken(void);

extern	void	getch(void);

extern	char	*nextchar;

// extern	int	symlookup(char *);

// vector.c
// #define EMPTY_VECTOR ((Vector){})
// Vector	init_vector(int size);
// void	push_vector(Vector *vec, void *item);
// void *	pop_vector(Vector *vec);
// void *	head_vector(Vector *vec);
// void *	tail_vector(Vector *vec);

