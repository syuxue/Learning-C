#include <stdio.h>
#include <stdlib.h>
#include "include/m_function.h"

/*
Linus：利用二级指针删除单向链表@http://coolshell.cn/articles/8990.html
原文：http://meta.slashdot.org/story/12/10/11/0030249/linus-torvalds-answers-your-questions
*/

typedef struct node {
	int val;
	struct node *next;
} node;
typedef int (*remove_fn)(node *);

int is_remove(node *cur)
{
	return cur->val % 3 == 1;
}

int is_remove2(node *cur)
{
	return cur->val % 3 == 2;
}

void show(node *head)
{
	node *cur;

	printf(M_bash_YELLOW "HEAD " M_bash_default);
	for (cur = head; cur; ) {
		printf(M_bash_CYAN "%d" M_bash_YELLOW " -> " M_bash_default, cur->val);
		if (!(cur = cur->next))
			printf(M_bash_RED "NULL" M_bash_default);
	}
	putchar('\n');
}

node *add_tradition(node *head, int val)
{
	node *cur, *prev;

	for (prev = NULL, cur = head; cur; cur = cur->next)
		prev = cur;
	cur = (node *) malloc(sizeof(node));
	cur->val = val;
	cur->next = NULL;
	if (prev)
		prev->next = cur;

	return cur;
}

node *del_tradition(node *head, remove_fn is_rm)
{
	node *cur, *prev, *next;

	for (prev = NULL, cur = head; cur; ) {
		next = cur->next;
		if (is_rm(cur)) {
			if (cur == head)
				head = next;
			else
				prev->next = next;
			free(cur);
		} else {
			prev = cur;
		}
		cur = next;
	}

	return head;
}

void add_linus(node **head, int val)
{
	node **cur;

	for (cur = head; *cur; cur = &(*cur)->next)
		;
	*cur = (node *) malloc(sizeof(node));
	(*cur)->val = val;
	(*cur)->next = NULL;
}

void del_linus(node **head, remove_fn is_rm)
{
	node **cur, *entry;

	for (cur = head; *cur; ) {
		entry = *cur;
		if (is_rm(entry)) {
			*cur = entry->next;
			free(entry);
		} else
			cur = &entry->next;
	}
}

int main(void)
{
	int i;
	node *head = NULL;

	if (1) {
		add_linus(&head, 0);
		for (i = 1; i < 10; i++) {
			add_linus(&head, i + 10);
		}
	} else {
		head = add_tradition(head, 0);
		for (i = 1; i < 10; i++) {
			add_tradition(head, i + 10);
		}
	}
	show(head);

	head = del_tradition(head, is_remove);
	show(head);

	del_linus(&head, is_remove2);
	show(head);

	return 0;
}
