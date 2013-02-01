#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/m_function.h"

/* ****************************** Definition ****************************** */

#define TERMINAL_ROW 40
#define GIFT_NUM 100
#define GIFT_NAME_LENGTH 10
#define GIFT_ORIGIN_LENGTH 4
#define GIFT_ID_START 10001

/* ****************************** Struct ****************************** */

struct date {
    unsigned short year;
    unsigned short month;
    unsigned short day;
};
struct gift {
	unsigned int id;
	char name[GIFT_NAME_LENGTH + 1];
	unsigned int price;
	unsigned int cost;
	unsigned int qtt;
	unsigned short kind;
	char origin[GIFT_ORIGIN_LENGTH + 1];
	struct date date;
};

/* ****************************** Function UI ****************************** */

void ui_menu(void)
{
	m_bash_clear();
	printf("\n********Gift Manager*******\n");
	printf("\n---------MENU---------");
	printf("\n1. View    Gift Info");
	printf("\n2. Add     Gift Info");
	printf("\n3. Modify  Gift Info");
	printf("\n4. Search  Gift Info");
	printf("\n5. Delete  Gift Info");
	printf("\n\n---------STAT---------");
	printf("\n6. All   Goods Value");
	printf("\n7. Kind  Goods Value");
	printf("\n8. View Info sort ascending");
	printf("\n9. View Info sort descending");
	printf("\n\n0. Quit");
	printf("\n\n*****copyright by WYc*****\n");
	printf("\nselect:");
}

void ui_gift_title(void)
{
	m_bash_clear();
	printf("                * * * * * * * G I F T   L I S T * * * * * * * \n");
	printf("    NUMBER          NAME    KIND    PRICE    COST    QUANTITY    ORIGIN          DATE    \n");
	printf("-----------------------------------------------------------------------------------------\n");
}

void ui_gift_display(struct gift *gift)
{
	printf("    %6u    %10s    %4u    %5u    %4u    %8u    %6s    %4u-%02u-%02u\n",
		gift->id,
		gift->name,
		gift->kind,
		gift->price,
		gift->cost,
		gift->qtt,
		gift->origin,
		gift->date.year,
		gift->date.month,
		gift->date.day
	);
}

/* ****************************** Function Data ****************************** */

void gift_random_text(char *text, int len)
{
	char *p;

	srand((unsigned long) text);
	len = (unsigned int)rand() % (len / 2) + len / 2;
	p = text;
	*p++ = (unsigned int)rand() % 26 + 65;
	while (p < text + len)
		*p++ = (unsigned int)rand() % 26 + 97;
	*p = '\0';
}

void gift_random(struct gift *gift)
{
	srand((unsigned long) gift);
	gift_random_text(gift->name, GIFT_NAME_LENGTH);
	gift_random_text(gift->origin, GIFT_ORIGIN_LENGTH);
	gift->price = (unsigned int)rand() % 1000 + 1;
	gift->cost = (unsigned int)rand() % gift->price + 1;
	gift->kind = (unsigned int)rand() % 9;
	gift->qtt = (unsigned int)rand() % 100;
	gift->date.year = (unsigned int)rand() % 20 + 1990;
	gift->date.month = (unsigned int)rand() % 12 + 1;
	gift->date.day = (unsigned int)rand() % 31 + 1;
}

struct gift *gift_init(unsigned num, int do_random)
{
	struct gift *giftlist, *gift;

	giftlist = malloc(sizeof(struct gift) * num);
	for (gift = giftlist; gift < giftlist + num; gift++) {
		gift->id = gift - giftlist + GIFT_ID_START;
		if (do_random)
			gift_random(gift);
		else {
			gift->name[0] = gift->origin[0] = '\0';
			gift->price = gift->cost = gift->kind = gift->qtt = 0;
			gift->date.year = gift->date.month = gift->date.day = 0;
		}
	}

	return giftlist;
}

void gift_showlist(struct gift *giftlist, unsigned num)
{
	struct gift *gift;

	ui_gift_title();
	for (gift = giftlist; gift < giftlist + GIFT_NUM; gift++) {
		ui_gift_display(gift);
		if ((gift - giftlist + 1) % TERMINAL_ROW == 0 && gift != giftlist + GIFT_NUM - 1) {
			m_bash_pause(NULL);
		}
	}
}

/* ****************************** Main ****************************** */
int main(void)
{
	struct gift *giftlist;

	giftlist = gift_init(GIFT_NUM, 1);
	gift_showlist(giftlist, GIFT_NUM);

	return 0;
}
