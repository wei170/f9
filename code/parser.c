/**
 *	Parser
 *	TODO: parsestmts may have a recursion, handle that
 */
#include <f9cc.h>

/************************************************************************/
/*																		*/
/*																		*/
/*						Lexical analyzer								*/
/*																		*/
/*																		*/
/*																		*/
/************************************************************************/

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

/************************************************************************/
/*																		*/
/*	parse -- initialize and then parse a program						*/
/*																		*/
/************************************************************************/

void parser() {
	lexinit();
	gettoken();
	program();
	printf("------------ Parser Finished !------------\n");
}

/************************************************************************/
/*																		*/
/*	match -- check the type of the nexttoken							*/
/*																		*/
/************************************************************************/

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

	/* Check the program header */

	match(LEXPGM, "expecting \'program\'");

	fprintf(cmd.outputFile, "#include <stdio.h>\n");		/* CODE */
	fprintf(cmd.outputFile, "#include <stdlib.h>\n");		/* CODE */

	gettoken();
	match(LEXLB, "expecting left brace");

	fprintf(cmd.outputFile, "int\tmain(void) {\n");		/* CODE */

	/* Parse declarations */

	gettoken();
	parsedecls();

	/* Parse statements */

	parsestmts();

	/* Check the end of the program */

	match(LEXRB, "expecting right brace");

	fprintf(cmd.outputFile, "\texit(0);\n}\n");					/* CODE */

	gettoken();
}

void parsedecls() {
	int symtype;

	/* While declarations exist, parse them */

	while (nexttok.tok_typ == LEXTYPE) {

		/* Lookup the declaration type */

		symtype = symtypelookup(nexttok.tok_str);
		if (symtype == SYMERR) {
			errmsg("expecting a valid type, i.e int");
			exit(1);
		}
		fprintf(cmd.outputFile, "\t%s\t", nexttok.tok_str);		/* CODE */

		/* Check the identifier of the declaration */

		gettoken();
		match(LEXID, "expecting identifier");

		/* Add the identifier into the table */

		if (!addsym(nexttok.tok_str, symtype, sizeof(nexttok.tok_str))) {
			errmsg("identifier already exists");
			exit(1);
		}
		fprintf(cmd.outputFile, "%s", nexttok.tok_str);			/* CODE */

		/* While comma exists, parse the following declarations */

		gettoken();
		while (nexttok.tok_typ == LEXCOMMA) {

			fprintf(cmd.outputFile, ", ");						/* CODE */

			/* Check the identifier of the declaration */

			gettoken();
			match(LEXID, "expecting identifier");

			/* Add the identifier into the table */

			if (!addsym(nexttok.tok_str, symtype, sizeof(nexttok.tok_str))) {
				errmsg("identifier already exists");
				exit(1);
			}
			fprintf(cmd.outputFile, "%s", nexttok.tok_str);		/* CODE */

			gettoken();
		}

		/* Check the semicolon at the end */
		match(LEXSEMI, "expecting semicolon");
		fprintf(cmd.outputFile, ";\n");							/* CODE */

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
	fprintf(cmd.outputFile, "\tif ");							/* CODE */

	gettoken();
	match(LEXLP, "expecting \'(\'");
	fprintf(cmd.outputFile, "( ");								/* CODE */

	/* Parse expression in the parenthesis */

	gettoken();
	parseexpr();

	match(LEXRP, "expecting \')\'");
	fprintf(cmd.outputFile, ") ");								/* CODE */

	gettoken();
	match(LEXLB, "expecting \'{\'");
	fprintf(cmd.outputFile, "{\n");								/* CODE */

	/* Parse the statements inside of the if */

	gettoken();
	parsestmts();

	match(LEXRB, "expecting \'}\'");
	fprintf(cmd.outputFile, "\t}\n");							/* CODE */

	/* Check if there is 'else' following */

	gettoken();
	if (nexttok.tok_typ == LEXELSE) {

		/* Parse the else statement */

		parseelse();
	}
}

void parseelse() {
	match(LEXELSE, "expecting \'else\'");
	fprintf(cmd.outputFile, "\telse ");							/* CODE */

	gettoken();
	match(LEXLB, "expecting \'{\'");
	fprintf(cmd.outputFile, "{\n");								/* CODE */

	gettoken();
	parsestmts();

	match(LEXRB, "expecting \'}\'");
	fprintf(cmd.outputFile, "\t}\n");							/* CODE */

	gettoken();
}

void parsewhile() {
	match(LEXWHILE, "expecting \'while\'");
	fprintf(cmd.outputFile, "\twhile ");						/* CODE */

	gettoken();
	match(LEXLP, "expecting \'(\'");
	fprintf(cmd.outputFile, "( ");								/* CODE */

	/* Parse the expression in the parenthessi */

	gettoken();
	parseexpr();

	match(LEXRP, "expecting \')\'");
	fprintf(cmd.outputFile, ") ");								/* CODE */

	gettoken();
	match(LEXLB, "expecting \'{\'");
	fprintf(cmd.outputFile, "{\n");								/* CODE */

	/* Parse the statements in the while */

	gettoken();
	parsestmts();

	match(LEXRB, "expecting \'}\'");
	fprintf(cmd.outputFile, "\t}\n");							/* CODE */

	gettoken();
}

void parsefnc(char *fname) {
	match(LEXFCN, "expecting a name");

	int moreargs, argcnt;

	gettoken();
	match(LEXLP, "expecting \'(\'");

	if (strcmp(fname, "exit") == 0) {
		/* Handle exit */
		fprintf(cmd.outputFile, "\texit(");						/* CODE */
		gettoken();
		parseexpr();
	} else {
		/* Handle print */
		gettoken();
		if (nexttok.tok_typ == LEXRP) {
			errmsg("empty list");
		}

		moreargs = 1;
		argcnt = 0;
		while (moreargs > 0) {
			argcnt++;
			if (argcnt >= MAXARGS) {
				errmsg("too many args to print");
			}

			if (nexttok.tok_typ == LEXSTR) {
				fprintf(cmd.outputFile, "\tprintf(");			/* CODE */
				fprintf(cmd.outputFile, "%s", nexttok.tok_str);	/* CODE */
				gettoken();
			} else {
				fprintf(cmd.outputFile, "\tprintf(\"%%d\",");	/* CODE */
				parseexpr();
			}

			if (nexttok.tok_typ == LEXCOMMA) {
				fprintf(cmd.outputFile, ");\n");				/* CODE */
				gettoken();
			} else {
				moreargs = 0;
			}
		}
	}

	match(LEXRP, "expecting \')\'");
	fprintf(cmd.outputFile, ");\n");							/* CODE */

	gettoken();
}

void parseread() {
	match(LEXREAD, "expecting read()");

	int moreargs, argcnt, varindex;

	gettoken();
	match(LEXLP, "expecting \'(\'");

	moreargs = 1;
	argcnt = 0;
	gettoken();
	while (moreargs > 0) {
		fprintf(cmd.outputFile, "\tscanf(\"%%d\", ");			/* CODE */

		argcnt++;
		match(LEXID, "expecting a identifier in the first argument of read");

		varindex = symlookup(nexttok.tok_str);
		if (varindex == -1) {
			errmsg("the identifier is not declared");
			exit(1);
		}
		fprintf(cmd.outputFile, "&%s", nexttok.tok_str);		/* CODE */

		gettoken();
		if (nexttok.tok_typ == LEXCOMMA) {
			fprintf(cmd.outputFile, ") +\n");					/* CODE */
			gettoken();
		} else {
			moreargs = 0;
		}
	}

	match(LEXRP, "expecting \')\'");
	fprintf(cmd.outputFile, ");\n");							/* CODE */

	gettoken();
}

void parseasn() {
	match(LEXID, "expecting a identifier");

	int symindex;
	char tmptok[MAXTOK];

	symindex = symlookup(nexttok.tok_str);
	if (symindex == -1) {
		errmsg("the identifer has not been declared");
		exit(1);
	}
	fprintf(cmd.outputFile, "\t%s ", nexttok.tok_str);			/* CODE */

	strcpy(tmptok, nexttok.tok_str);

	gettoken();
	match(LEXARIOP, "expecting a arithmetic operation");
	fprintf(cmd.outputFile, "%s ", nexttok.tok_str);			/* CODE */

	gettoken();
	if (nexttok.tok_typ == LEXREAD) {
		parseread();
	} else {
		parseexpr();
		match(LEXSEMI, "expecting a semicolon");
		gettoken();
	}

	fprintf(cmd.outputFile, ";\n");								/* CODE */
}

void parseexpr() {
	parsecmp();

	while (nexttok.tok_typ == LEXLOGOP) {
		fprintf(cmd.outputFile, "%s ", nexttok.tok_str);		/* CODE */

		gettoken();
		parsecmp();
	}
}

void parsecmp() {
	parseari();

	while (nexttok.tok_typ == LEXCMPOP) {
		fprintf(cmd.outputFile, "%s ", nexttok.tok_str);		/* CODE */

		gettoken();
		parseari();
	}
}

void parseari() {
	parseterm();

	while (nexttok.tok_typ == LEXARIOP) {
		fprintf(cmd.outputFile, "%s ", nexttok.tok_str);		/* CODE */

		gettoken();
		parseterm();
	}
}

void parseterm() {
	if (nexttok.tok_typ == LEXID) {
		fprintf(cmd.outputFile, "%s ", nexttok.tok_str);		/* CODE */
		gettoken();
	} else if (nexttok.tok_typ == LEXTYPE) {
		fprintf(cmd.outputFile, "%s ", nexttok.tok_str);		/* CODE */
		gettoken();
	} else if (nexttok.tok_typ == LEXNUM) {
		fprintf(cmd.outputFile, "%s ", nexttok.tok_str);		/* CODE */
		gettoken();
	} else if (nexttok.tok_typ == LEXLP) {
		fprintf(cmd.outputFile, "( ");							/* CODE */

		gettoken();
		parseexpr();

		match(LEXRP, "expecting \')\'");
		fprintf(cmd.outputFile, ") ");							/* CODE */

		gettoken();
	} else {
		printf("tok: %d\n", nexttok.tok_typ);
		errmsg("expecting lexname, type, number, or \'(\' expr \')\'");
		exit(1);
	}
}

