#include <f9cc.h>

/* Declare the global symbol variables */
int	nsyms;
struct symentry	symtab[MAXSYM];	/* The symbol table	*/

/**
 * Add a new symbol into the symtab
 * @return: -1 if it exists | index if successfully added
 */
int addsym(
		char *name,
		int	type,
		int	size
		)
{
	/* The name has already existed in the table */
	if (symlookup(name) != -1) return -1;

	struct symentry *nextsym = &symtab[nsyms++];
	strcpy(nextsym->sym_name, name);
	nextsym->sym_type = type;
	nextsym->sym_size = size;
	return (nsyms - 1);
}

/**
 * Find the symbol structure in the table based on a name
 *	@return: index of the found symbol | -1 if not found
 */
int symlookup(
		char *name
		)
{
	for (int i = nsyms - 1; i >= 0; i--) {
		if (strcmp(name, symtab[i].sym_name) == 0) {
			return i;
		}
	}
	return -1;
}

int symtypelookup(
		char *type
		)
{
	if (strcmp(type, "int") == 0) return SYMINT;
	else if (strcmp(type, "str") == 0) return SYMSTR;
	else if (strcmp(type, "bool") == 0) return SYMBOOL;
	else return SYMERR;
}
