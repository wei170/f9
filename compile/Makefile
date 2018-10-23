PROGRAM		=		f9cc
FLAGS		=		-Wall -Waddress
BINDIR		=		./binaries
CODEDIR		=		../code
INCDIR		= 		../includes
GENDIR      =       ../gen

$(PROGRAM): genfsm main.o parser.o lex.o file.o symbol.o
	    gcc $(FLAGS) $(BINDIR)/*.o -o $(PROGRAM)

genfsm:     $(GENDIR)/genfsm
		$(GENDIR)/genfsm $(CODEDIR)/fsm-specification

main.o:     $(CODEDIR)/main.c
	    gcc $(FLAGS) -I$(INCDIR) -c $(CODEDIR)/main.c -o $(BINDIR)/main.o

parser.o:	$(CODEDIR)/parser.c
		gcc $(FLAGS) -I$(INCDIR) -c $(CODEDIR)/parser.c -o $(BINDIR)/parser.o

lex.o:      $(CODEDIR)/lex.c
	    gcc $(FLAGS) -I$(INCDIR) -c $(CODEDIR)/lex.c -o $(BINDIR)/lex.o

file.o:     $(CODEDIR)/file.c
	    gcc $(FLAGS) -I$(INCDIR) -c $(CODEDIR)/file.c -o $(BINDIR)/file.o

symbol.o:	$(CODEDIR)/symbol.c
		gcc $(FLAGS) -I$(INCDIR) -c $(CODEDIR)/symbol.c -o $(BINDIR)/symbol.o

clean:
	    rm -f $(BINDIR)/*.o $(PROGRAM)
