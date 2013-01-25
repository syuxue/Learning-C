/* ****************************** Constant ****************************** */
#define RPN_DEBUG				1
#define	RPN_NOTATION_MAXSIZE	1024
#define	RPN_OPERATOR_MAXSIZE	64
#define	RPN_STACK_MAXSIZE		100

/* ****************************** Function ****************************** */
double	rpn_calculate(char notation[]);
int		rpn_getop(char notation[], char operator[]);
void	rpn_operate(char operator[]);
