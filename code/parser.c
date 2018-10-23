/**
 *	Parser
 *
 */
#include <f9cc.h>



static	void	match		(int, char *);
static	void	program		(void);
static	void	parsepgm	(void);
static	void	parsedecls	(void);
static	void	parsestmts	(void);
static	void	parsestmt	(void);
static	void	parseif		(void);
static	void	parseelse	(void);
static	void	parsewhile	(void);
static	void	parsefnc	(char *);
static	void	parseread	(void);
static	void	parseasn	(void);
static	void	parseexpr	(void);
static	void	parsecmp	(void);
static	void	parseari	(void);
static	void	parseterm	(void);

void parser() {
	lexinit();
	gettoken();
	program();
	printf("------------ Parser Finished !------------\n");
}

void match(int lextype, char *msg) {
	if (nexttok.tok_typ != lextype) {
		errmsg(msg);
		exit(1);
	}
}

void program() {
	parsepgm();
	match(LEXEOF, "expected EOF");
}

void parsepgm() {
	match(LEXPGM, "expecting \'program\'");
	gettoken();
	match(LEXLB, "expecting left brace");
	gettoken();
	parsedecls();
	parsestmts();
	match(LEXRB, "expecting right brace");
	gettoken();
}

void parsedecls() {
	while (nexttok.tok_typ == LEXTYPE) {
		int symtype = symtypelookup(nexttok.tok_str);
		if (symtype == SYMERR) {
			errmsg("expecting a valid type, i.e int");
			exit(1);
		}
		// TODO: support ,
		gettoken();
		match(LEXID, "expecting identifier");
		/* Add the identifier into the table */
		if (!addsym(nexttok.tok_str, symtype, sizeof(nexttok.tok_str))) {
			errmsg("identifier already exists");
		}
		gettoken();
		match(LEXSEMI, "expecting semicolon");
		gettoken();
	}
}

void parsestmts() {
	parsestmt();

	while (nexttok.tok_typ != LEXRB) {
		parsestmt();
	}
}

void parsestmt() {
	char fname[MAXTOK];
	switch(nexttok.tok_typ) {
		case LEXIF:
			parseif();
			break;
		case LEXWHILE:
			parsewhile();
			break;
		case LEXFCN:
			strcpy(fname, nexttok.tok_str);
			parsefnc(fname);
			break;
		case LEXREAD:
			parseread();
			break;
		case LEXID:
			parseasn();
			break;
		case LEXSEMI:
			errmsg("Did not expect \';\' here");
			exit(1);
		default:
			errmsg("expecting \'if\', \'while\', \'function\', or identifier");
			exit(1);
	}
}

void parseif() {
	match(LEXIF, "expecting \'if\'");

	gettoken();
	match(LEXLP, "expecting \'(\'");

	gettoken();
	parseexpr();

	match(LEXRP, "expecting \')\'");

	gettoken();
	match(LEXLB, "expecting \'{\'");

	gettoken();
	parsestmts();

	match(LEXRB, "expecting \'}\'");

	gettoken();
	if (nexttok.tok_typ == LEXELSE) {
		parseelse();
	}
}

void parseelse() {
	match(LEXELSE, "expecting \'else\'");

	gettoken();
	match(LEXLB, "expecting \'{\'");

	gettoken();
	parsestmts();

	match(LEXRB, "expecting \'}\'");
	gettoken();
}

void parsewhile() {
	match(LEXWHILE, "expecting \'while\'");

	gettoken();
	match(LEXLP, "expecting \'(\'");

	gettoken();
	parseexpr();

	match(LEXRP, "expecting \')\'");

	gettoken();
	match(LEXLB, "expecting \'{\'");

	gettoken();
	parsestmts();

	match(LEXRB, "expecting \'}\'");
	gettoken();
}

void parsefnc(char *fname) {
	// TODO: check if the name exist
	match(LEXFCN, "expecting a name");

	gettoken();
	match(LEXLP, "expecting \'(\'");

	gettoken();
	parseexpr();

	while (nexttok.tok_typ == LEXCOMMA) {
		gettoken();
		parseexpr();
	}

	match(LEXRP, "expecting \')\'");
	gettoken();
}

void parseread() {
	match(LEXREAD, "expecting read()");

	gettoken();
	match(LEXLP, "expecting \'(\'");

	gettoken();
	parseexpr();

	while (nexttok.tok_typ == LEXCOMMA) {
		gettoken();
		parseexpr();
	}

	match(LEXRP, "expecting \')\'");
	gettoken();
}

void parseasn() {
	match(LEXID, "expecting a identifier");
	if (symlookup(nexttok.tok_str) == -1) {
		errmsg("the identifer has not been declared");
		exit(1);
	}

	gettoken();
	match(LEXARIOP, "expecting a ari operation");

	gettoken();
	parseexpr();

	// TODO: if this is correct
	if (nexttok.tok_typ == LEXSEMI) {
		gettoken();
	}
	// match(LEXSEMI, "expecting a semicolon");
	// gettoken();
}

void parseexpr() {
	parsecmp();

	while (nexttok.tok_typ == LEXLOGOP) {
		gettoken();
		parsecmp();
	}
}

void parsecmp() {
	parseari();

	while (nexttok.tok_typ == LEXCMPOP) {
		gettoken();
		parseari();
	}
}

void parseari() {
	parseterm();

	while (nexttok.tok_typ == LEXARIOP) {
		gettoken();
		parseterm();
	}
}

void parseterm() {
	if (nexttok.tok_typ == LEXID) {
		gettoken();
	} else if (nexttok.tok_typ == LEXTYPE) {
		gettoken();
	} else if (nexttok.tok_typ == LEXNUM) {
		gettoken();
	} else if (nexttok.tok_typ == LEXSTR) {
		gettoken();
	} else if (nexttok.tok_typ == LEXFCN) {
		char fname[MAXTOK];
		strcpy(fname, nexttok.tok_str);
		parsefnc(fname);
	} else if (nexttok.tok_typ == LEXREAD) {
		parseread();
	} else if (nexttok.tok_typ == LEXLP) {
		gettoken();
		parseexpr();
		match(LEXRP, "expecting \')\'");
		gettoken();
	} else {
		printf("tok: %d\n", nexttok.tok_typ);
		errmsg("expecting lexname, type, number, or \'(\' expr \')\'");
		exit(1);
	}
}

