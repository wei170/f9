/**
 *	Parser
 */
#include <f9cc.h>

static	void	program		(void);
static	void	parsepgm	(void);
static	void	parsedecls	(void);
static	void	parsestmts	(void);
static	void	parsestmt	(void);
static	void	parseif		(void);
static	void	parseelse	(void);
static	void	parsewhile	(void);
static	void	parsefnc	(void);
static	void	parseidop	(void);
static	void	parseexpr	(void);
static	void	parsecmp	(void);
static	void	parseari	(void);
static	void	parseterm	(void);

void parser() {
	lexinit();
	gettoken();
	program();
}

void program() {
	parsepgm();
	if (nexttok.tok_typ != LEXEOF) {
		errmsg("expected EOF");
		exit(1);
	}
}

void parsepgm() {
	if (nexttok.tok_typ != LEXPGM) {
		errmsg("expecting \'program\'");
		exit(1);
	}
	gettoken();
	parsedecls();
	parsestmts();
	if (nexttok.tok_typ != LEXRB) {
		errmsg("expecting right brace");
		exit(1);
	}
}

void parsedecls() {
	while (nexttok.tok_typ == LEXTYPE) {
		gettoken();
		if (nexttok.tok_typ != LEXID) {
			errmsg("expecting identifier");
			exit(1);
		}
		gettoken();
		if (nexttok.tok_typ != LEXSEMI) {
			errmsg("expecting semicolon");
			exit(1);
		}
		gettoken();
	}
}

void parsestmts() {
	gettoken();
	parsestmt();

	gettoken();
	while (nexttok.tok_typ != LEXRB) {
		parsestmt();
		gettoken();
	}
}

void parsestmt() {
	if (nexttok.tok_typ == LEXIF) {
		parseif();
	} else if (nexttok.tok_typ == LEXWHILE) {
		parsewhile();
	} else if (nexttok.tok_typ == LEXFCN) {
		parsefnc();
	} else if (nexttok.tok_typ == LEXID) {
		parseidop();
	} else {
		errmsg("expecting \'if\', \'while\', \'function\', or identifier");
		exit(1);
	}
}

void parseif() {
	if (nexttok.tok_typ != LEXIF) {
			errmsg("expecting \'if\'");
			exit(1);
	}

	gettoken();
	parseexpr();

	gettoken();
	if (nexttok.tok_typ != LEXLB) {
			errmsg("expecting \'{\'");
			exit(1);
	}

	gettoken();
	parsestmts();

	gettoken();
	if (nexttok.tok_typ != LEXRB) {
			errmsg("expecting \'}\'");
			exit(1);
	}

	gettoken();
	if (nexttok.tok_typ == LEXELSE) {
		parseelse();
	}
}

void parseelse() {
	if (nexttok.tok_typ != LEXELSE) {
			errmsg("expecting \'else\'");
			exit(1);
	}

	gettoken();
	if (nexttok.tok_typ != LEXLB) {
			errmsg("expecting \'{\'");
			exit(1);
	}

	gettoken();
	parsestmts();

	gettoken();
	if (nexttok.tok_typ != LEXRB) {
			errmsg("expecting \'}\'");
			exit(1);
	}
}

void parsewhile() {
	if (nexttok.tok_typ != LEXWHILE) {
			errmsg("expecting \'while\'");
			exit(1);
	}

	gettoken();
	parseexpr();

	gettoken();
	if (nexttok.tok_typ != LEXLB) {
			errmsg("expecting \'{\'");
			exit(1);
	}

	gettoken();
	parsestmts();

	gettoken();
	if (nexttok.tok_typ != LEXRB) {
			errmsg("expecting \'}\'");
			exit(1);
	}
}

void parsefnc() {
	if (nexttok.tok_typ != LEXFCN) {
			errmsg("expecting a name");
			exit(1);
	}

	gettoken();
	parseexpr();
}

void parseidop() {
	if (nexttok.tok_typ != LEXID) {
			errmsg("expecting a identifier");
			exit(1);
	}

	gettoken();
	parseari();
}

void parseexpr() {
	parsecmp();

	gettoken();
	while (nexttok.tok_typ == LEXLOGOP) {
		gettoken();
		parsecmp();
	}
}

void parsecmp() {
	parseari();

	gettoken();
	while (nexttok.tok_typ == LEXCMPOP) {
		gettoken();
		parseari();
	}
}

void parseari() {
	parseterm();

	gettoken();
	while (nexttok.tok_typ == LEXARIOP) {
		gettoken();
		parseterm();
	}
}

void parseterm() {
	if (nexttok.tok_typ == LEXNAME) {
	} else if (nexttok.tok_typ == LEXTYPE) {
	} else if (nexttok.tok_typ == LEXLP) {
		gettoken();
		parseexpr();
		gettoken();
		if (nexttok.tok_typ != LEXRP) {
			errmsg("expecting \')\'");
			exit(1);
		}
	} else {
		errmsg("expectin lexname, type, or \'(\' expr \')\'");
		exit(1);
	}
}

