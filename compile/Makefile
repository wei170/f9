PROGRAM		=		f9cc
FLAGS		=		-Wall -Waddress
BINDIR		=		./binaries
CODEDIR		=		../code
INCDIR		= 		../includes
GENDIR      =       ../gen

$(PROGRAM): genfsm main.o parser.o lex.o file.o
	    gcc $(FLAGS) $(BINDIR)/*.o -o $(PROGRAM)

genfsm:     $(GENDIR)/genfsm
		$(GENDIR)/genfsm $(CODEDIR)/fsm-specification

main.o:     $(INCDIR)/f9cc.h $(CODEDIR)/main.c
	    gcc $(FLAGS) -I$(INCDIR) -c $(CODEDIR)/main.c -o $(BINDIR)/main.o

parser.o:	$(INCDIR)/f9cc.h $(CODEDIR)/parser.c
		gcc	$(FLAGS) -I$(INCDIR) -c $(CODEDIR)/parser.c -o $(BINDIR)/parser.o

lex.o:      $(INCDIR)/f9cc.h $(CODEDIR)/lex.c
	    gcc $(FLAGS) -I$(INCDIR) -c $(CODEDIR)/lex.c -o $(BINDIR)/lex.o

file.o:     $(INCDIR)/f9cc.h $(CODEDIR)/file.c
	    gcc $(FLAGS) -I$(INCDIR) -c $(CODEDIR)/file.c -o $(BINDIR)/file.o

clean:
	    rm -f $(BINDIR)/*.o $(PROGRAM)
