#!/bin/zsh
case $# in
	0)	FILE=
		;;
	1)	FILE="$1"
		if test ! -r $FILE; then
			echo "Cannot read file $FILE" >&2
			exit 1
		fi
		;;
	*)	echo "Use is genfsm [file] [output]" >&2
		exit 1
		;;
esac

FSM_DEFS=fsm-defs
FSM_CODE=fsm-code

Q="'"
# sed 's/	*/	/g' |
cat $FILE | gawk '

BEGIN {
	states	= 0;					# Count of states
	quote	= "'$Q'"				# Single quote
}

/^[^	]/ {						# State name line starts with non-black
									# and non-tab
	if (NF != 1) {
		printf("Error line %d: state name mush have on field\n", NR);
		exit(1);
	}

	state_name = $1;
	tmp = sprintf("#define FSM_STATE_%s", state_name);
	printf("%s", tmp);
	for (ll = length(tmp); ll < 32; ll++) {
		printf(" ");
	}
	printf("%d\n", states);
	states++;
	nentries = 0;
	printf("/**************************************************************************/\n");
	printf("/*                                                                        */\n");
	x = sprintf(" Transitions for %s (%d)", state_name, states);
	printf("/*%s", x);
	for (ll = length(x); ll < 72; ll++) { printf(" "); } printf("*/\n");
	printf("/*                                                                        */\n");
	printf("/**************************************************************************/\n");
	next;
}


# Empty like

/^$/ {
	print;
	next;
}

# Blank like

/^[		]*$/ {
	print;
	next;
}

# Line starting with a comment

/^	*\// {
	print;
	next;
}

# Other lines
#
{
	if (states == 0) {
		printf("Error line %d: specification precedes state name\n", NR);
		exit(1);
	} else if (NF != 4) {
		printf("Error line %d: specification needs four fields.\n", NR)
		exit(1);
	}

	# Specification with four fields
	
	if ($1 == "any") {
		if (nentries++ != 0) {
			printf("Error line %d: \"any\" is not the first specification\n", NR);
			exit(1);
		}
		printf("	for (c = 0; c <= CHAR_RANGE; c++) {\n");
		printf("		fsm[FSM_STATE_%s][c].fsm_nextst = FSM_STATE_%s;\n", state_name, $2);
		printf("		fsm[FSM_STATE_%s][c].fsm_action = FSM_ACT_%s;\n", state_name, $3);
		printf("		fsm[FSM_STATE_%s][c].fsm_lextyp = %s;\n", state_name, $4);
		printf("	}\n")
	} else if ((substr($1, 1, 1) != quote) && ($1 ~/\-/)) {
		if (length($1) != 3) {
			printf("Error line %d: incorrect range specification\n", NR);
			exit(1);
		}

		nentries++;
		char1 = substr($1, 1, 1);
		char2 = substr($1, 3, 1);

		printf("	for (c = %s%s%s; c <= %s%s%s; c++) {\n", \
							quote, char1, quote, \
							quote, char2, quote);
		printf("		fsm[FSM_STATE_%s][c].fsm_nextst = FSM_STATE_%s;\n", state_name, $2);
		printf("		fsm[FSM_STATE_%s][c].fsm_action = FSM_ACT_%s;\n", state_name, $3);
		printf("		fsm[FSM_STATE_%s][c].fsm_lextyp = %s;\n", state_name, $4);
		printf("	}\n")
	} else {
		nentries++;
		printf("	fsm[FSM_STATE_%s][%s].fsm_nextst = FSM_STATE_%s;\n", state_name, $1, $2);
		printf("	fsm[FSM_STATE_%s][%s].fsm_action = FSM_ACT_%s;\n", state_name, $1, $3);
		printf("	fsm[FSM_STATE_%s][%s].fsm_lextyp = %s;\n", state_name, $1, $4);
	}
}

END {
	printf("#define STATES\t\t\t\t\t%d\n", states);
}
'

grep "#define" $FSM_CODE > $FSM_DEFS
grep -v "#define" $FSM_CODE > fsm-code.bak && mv fsm-code.bak $FSM_CODE
