declaration:
	declaration-specifiers init-declarator-list(opt)
	type-specifiers declarator

declaration-specifiers:
	/*storage-class-specifier declaration-specifiers(opt)*/
	type-specifier declaration-specifiers(opt)
	/*type-qualifier declaration-specifiers(opt)*/

	type-specifier:
		void
		char
		short
		int
		long
		float
		double
		signed
		unsigned
		/*struct-or-union-specifier
		enum-specifier
		typedef-name*/

init-declarator-list:
	init-declarator
	init-declarator-list , init-declarator

	init-declarator:
		declarator
		/*declarator = initializer*/

		declarator:
			pointer(opt) direct-declarator

			pointer:
				* /*type-qualifier-list(opt)*/
				* /*type-qualifier-list(opt)*/ pointer

			direct-declarator:
				identifier
				(declarator)
				direct-declarator [ constant-expression(opt) ]
				direct-declarator ( parameter-type-list )
				direct-declarator ( identifier-list(opt) )

					parameter-type-list:
						parameter-list
						parameter-list , ...

					parameter-list:
						parameter-declaration
						parameter-list , parameter-declaration

					parameter-declaration:
						declaration-specifiers declarator
						declaration-specifiers abstract-declarator(opt)