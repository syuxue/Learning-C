#include <stdio.h>
#include "include/m_function.h"

/* ****************************** Definition & Declaration ****************************** */
#define integer int
typedef int inte;

/* ****************************** Function ****************************** */


/* ****************************** Main ****************************** */
int main(int argc, char *argv[])
{
	// Section 6.7 Typedef
	{
		int i;
		integer i2;
		inte i3;

		i = 10;
		i2 = i;
		i3 = i;

		M_printd(i);
		M_printd(i2);
		M_printd(i3);
	}

	// Section 6.8 Union
	{
		struct {
			char *name;
			int flags;
			int utype;
			union {
				int ival;
				float fval;
				char *sval;
			} u;
		} sym;
		M_printd(sizeof sym);

		sym.u.ival = 10;
		sym.u.fval = 10.2;
		M_printd(sym.u.ival);
		M_printf(sym.u.fval);
		
	}

	// Section 6.9 Bie-fields
	{
		struct {
			unsigned int sign : 1;
			// Fields need not be names; unnamed fields (a colon
			// and width only) are used for padding. The special width 0 may be used to force alignment at
			// the next word boundary.
			unsigned int : 0; 
			unsigned int val : 31;
		} ivalue;

		//M_printd(flags.is_static);
		M_printd(sizeof ivalue);
	}

	return 0;
}
