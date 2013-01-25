#include <stdio.h>
#include "m_function.h"
int day_of_year(int year, int month, int day);
void month_day(int year, int yearday, int *pmonth, int *pday);
char *month_name(int month);
int year_isleap(int year);
char daytab[2][13] = {
	{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
	{0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
};

int day_of_year(int year, int month, int day)
{
	char *p;
	
	p = daytab[year_isleap(year)];
	while (month > 0)
		day += *(p + --month);
	
	return day;
}

void month_day(int year, int yearday, int *pmonth, int *pday)
{
	char *p;
	
	p = daytab[year_isleap(year)];
	for (*pmonth = 1; yearday > p[*pmonth]; (*pmonth)++) 
		yearday -= p[*pmonth];
	*pday = yearday;
}

char *month_name(int month)
{
	static char *mname[] = {"Illegal month", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
	return (month >= 1 && month <= 12) ? mname[month] : mname[0];
}

int year_isleap(int year)
{
	return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
}

/* ****************************** Main ****************************** */
int main(int argc, char *argv[])
{
	int month, day;

	month_day(1987, 50, &month, &day);
	printf("%s,%d\n", month_name(month), day);
	printf("%d\n", day_of_year(2001, 12, 31));

	return 0;
}
