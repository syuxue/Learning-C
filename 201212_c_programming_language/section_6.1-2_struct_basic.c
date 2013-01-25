#include <stdio.h>
#include <math.h>
#include "m_function.h"

/* ****************************** Definition & Declaration ****************************** */
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
struct point {
	int x;
	int y;
};
struct rect {
	struct point lt;
	struct point rb;
};
double dist(struct point pt, struct point origin);
struct point makepoint(int x, int y);

/* ****************************** Function ****************************** */
void showcoord(struct point pt)
{
	printf("%3d * %3d dist: %g\n", pt.x, pt.y, dist((struct point){0, 0}, pt));
}

double dist(struct point pt, struct point origin)
{
	return sqrt(pow(pt.x - origin.x, 2) + pow(pt.y - origin.y, 2));
}

struct point makepoint(int x, int y)
{
	struct point tmp;

	tmp.x = x, tmp.y = y;
	return tmp;
}

struct point addpoint(struct point p1, struct point p2)
{
	p1.x += p2.x, p1.y += p2.y;
	return p1;
}

int ptinrect(struct point p, struct rect r)
{
	return p.x >= r.lt.x && p.y >= r.lt.y
		&& p.x <= r.rb.x && p.y <= r.rb.y;
}

struct rect canonrect(struct rect r)
{
	struct rect tmp;

	tmp.lt.x = min(r.lt.x, r.rb.x);
	tmp.lt.y = min(r.lt.y, r.rb.y);
	tmp.rb.x = max(r.lt.x, r.rb.x);
	tmp.rb.y = max(r.lt.y, r.rb.y);
	return tmp;
}

void showPinRect(struct point p, struct rect r)
{
	int x, y;
	
	for (y = r.lt.y - 1; y <= r.rb.y + 1; y++) {
		if (y == r.lt.y - 1 || y == r.rb.y + 1)
			for (x = r.lt.x - 1; x <= r.rb.x + 1; x++)
				printf("-");
		else
			for (x = r.lt.x - 1; x <= r.rb.x + 1; x++)
				if (x == r.lt.x - 1 || x == r.rb.x + 1)
					printf("|");
				else if (y == p.y && x == p.x)
					printf("X");
				else
					printf(" ");
		printf("\n");
	}
}

/* ****************************** Main ****************************** */
int main(void)
{
	struct rect screen;
	struct point origin = { 0, 0 }, max, middle, *pp;
	
	max = makepoint(80, 40); // max = { 100, 40 }; 这样是非法的
	screen = canonrect((struct rect){origin, max});

	pp = &middle;
	*pp = makepoint((max.x + origin.y) / 2,
					   (max.y + origin.y) / 2);
	
	// 各种输出
	printf("origin: "), showcoord(origin);
	printf("middle: "), showcoord(middle);
	printf("mid*2:  "), showcoord(addpoint(middle, middle));
	printf("max:    "), showcoord(max);
	M_printd(ptinrect(middle, screen));
	M_printd(ptinrect(addpoint(middle, max), screen));

	showPinRect(middle, screen);
	showPinRect((struct point){12, 13}, screen);

	return 0;
}
