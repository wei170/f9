#include <f9cc.h>

/* Declare the global symbol variables */
int	nsyms;
struct symentry	symtab[MAXSYM];	/* The symbol table	*/

/**
 * Add a new symbol into the symtab
 * @return: 0 if it exists | 1 if successfully added
 */
int addsym(
		char *name,
		int	type,
		int	size
		)
{
	/* The name has already existed in the table */
	if (symlookup(name) != -1) return 0;

	struct symentry *nextsym = &symtab[nsyms++];
	strcpy(nextsym->sym_name, name);
	nextsym->sym_type = type;
	nextsym->sym_size = size;
	return 1;
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
	else return SYMERR;
}
