/* ****************************** Include ****************************** */
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include "m_function.h"
#include "section_4.3_rpn.h"

/* ****************************** Stack ****************************** */
double	rpn_stack[RPN_STACK_MAXSIZE];
int		rpn_stack_p;
void	rpn_stack_push(double val);
double	rpn_stack_pop(void);
double	rpn_stack_show(int offset);
void	rpn_stack_clear(void);


/* ****************************** Reverse Polish Notation ****************************** */
double rpn_calculate(char notation[])
{
	char operator[RPN_OPERATOR_MAXSIZE];

	rpn_stack_clear();
	while (rpn_getop(notation, operator)) {
		if (RPN_DEBUG) printf("debug:  [parse] %s\t%s\n",operator,notation);
		rpn_operate(operator);
	}

	if (rpn_stack_p == 1)
		return rpn_stack_pop();
	else {
		printf("error: notation error, %d number(s) in stack\n",rpn_stack_p);
		return 0.0;
	}
}

int rpn_getop(char *notation, char *operator)
{
	static int i = 0;

	//Comment,\0
	if (*notation == '#' || *(notation + i) == '\0') return i = 0;

	//Skip space
	for (; isspace(*(notation + i)); i++) ;

	//Get op
	for (; *(notation + i) != '\0' && i < RPN_NOTATION_MAXSIZE; i++) {
		if (isspace(*(notation + i)))
			break;
		*(operator++) = *(notation + i);
	}
	*operator = '\0';
	return i;
}

void rpn_operate(char operator[])
{
	double tmp,tmp2,m_str2float();

	if (
		isdigit(operator[0])
		|| (operator[0] == '-' && isdigit(operator[1]))
	) {
		if (RPN_DEBUG) printf("debug:    [num] %g\n",m_str2float(operator,10));
		rpn_stack_push(m_str2float(operator,10));
	} else if (m_strcmp(operator,"+") == 0) {
		if (RPN_DEBUG) printf("debug:   [op:+] %g + %g = %g\n",rpn_stack_show(1),rpn_stack_show(0),rpn_stack_show(1) + rpn_stack_show(0));
		rpn_stack_push(rpn_stack_pop() + rpn_stack_pop());
	} else if (m_strcmp(operator,"-") == 0) {
		if (RPN_DEBUG) printf("debug:   [op:-] %g - %g = %g\n",rpn_stack_show(1),rpn_stack_show(0),rpn_stack_show(1) - rpn_stack_show(0));
		tmp = rpn_stack_pop();
		rpn_stack_push(rpn_stack_pop() - tmp);
	} else if (m_strcmp(operator,"*") == 0) {
		if (RPN_DEBUG) printf("debug:   [op:*] %g * %g = %g\n",rpn_stack_show(1),rpn_stack_show(0),rpn_stack_show(1) * rpn_stack_show(0));
		rpn_stack_push(rpn_stack_pop() * rpn_stack_pop());
	} else if (m_strcmp(operator,"/") == 0) {
		if (RPN_DEBUG) printf("debug:   [op:/] %g / %g = %g\n",rpn_stack_show(1),rpn_stack_show(0),rpn_stack_show(1) / rpn_stack_show(0));
		tmp = rpn_stack_pop();
		if (tmp != 0.0)
			rpn_stack_push(rpn_stack_pop() / tmp);
		else
			printf("error: zero divisor\n");
	} else if (m_strcmp(operator,"%") == 0) {
		if (RPN_DEBUG) printf("debug:   [op:%%] %g %% %g = %d\n",rpn_stack_show(1),rpn_stack_show(0),(int)rpn_stack_show(1) % (int)rpn_stack_show(0));
		tmp = rpn_stack_pop();
		if ((double)(long)rpn_stack_show(0) == rpn_stack_show(0) && (double)(long)rpn_stack_show(1) == rpn_stack_show(1))
			rpn_stack_push((int)rpn_stack_pop() % (int)tmp);
		else
			printf("error: mod float\n");
	} else if (m_strcmp(operator,"$_d") == 0) {
		if (RPN_DEBUG) printf("debug: [op:$_d] copy last number %g\n",rpn_stack_show(0));
		rpn_stack_push(rpn_stack_show(0));
	} else if (m_strcmp(operator,"&sw") == 0) {
		if (RPN_DEBUG) printf("debug: [op:&sw] swap last 2 number %g,%g -> %g,%g\n",rpn_stack_show(1),rpn_stack_show(0),rpn_stack_show(0),rpn_stack_show(1));
		tmp2 = rpn_stack_pop();
		tmp = rpn_stack_pop();
		rpn_stack_push(tmp2);
		rpn_stack_push(tmp);
	} else if (m_strcmp(operator,"&sh") == 0) {
		printf("debug: [op:&sh] show top number = %g\n",rpn_stack_show(0));
	} else if (m_strcmp(operator,"pow") == 0) {
		if (RPN_DEBUG) printf("debug: [op:pow] pow(%g,%g) = %g\n",rpn_stack_show(1),rpn_stack_show(0),pow(rpn_stack_show(1),rpn_stack_show(0)));
		tmp = rpn_stack_pop();
		rpn_stack_push(pow(rpn_stack_pop(),tmp));
	} else {
		printf("error: unsupport operator \"%s\", ignore\n",operator);
	}
}

/* ****************************** Stack ****************************** */
void rpn_stack_push(double val)
{
	if (rpn_stack_p < RPN_STACK_MAXSIZE)
		rpn_stack[rpn_stack_p++] = val;
	else
		printf("error: stack full, can't push %g\n",val);
}

double rpn_stack_pop(void)
{
	if (rpn_stack_p > 0)
		return rpn_stack[--rpn_stack_p];
	else {
		printf("error: stack empty\n");
		return 0.0;
	}
}

double rpn_stack_show(int offset)
{
	return rpn_stack[rpn_stack_p - (offset + 1)];
}

void rpn_stack_clear(void)
{
	rpn_stack_p = 0;
}
