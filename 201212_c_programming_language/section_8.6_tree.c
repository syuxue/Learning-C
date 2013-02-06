#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "section_8.6_directory.h"

static int depth = -1;

char *m_basename(const char *name)
{
	const char *p;

	for (p = name + strlen(name) - 1; p > name && *p != '/'; p--)
		;
	return (char *) (p == name ? name : p + 1);
}

void fsize(char *fname)
{
	int i;
	struct stat stbuf;

	/*
	 * http://www.kernel.org/doc/man-pages/online/pages/man2/stat.2.html
	 * lstat() is identical to stat(), except that if path is a symbolic link, then
     * the link itself is stat-ed, not the file that it refers to. 
	 */
	if (lstat(fname, &stbuf) == -1) {
		fprintf(stderr, "fsize: can't access %s\n", fname);
		return;
	}

	// Display
	if (depth > -1) {
		printf("|");
		for (i = 0; i < depth; i++)
			printf("   |");
		printf("-- ");
	}
	printf("%s", m_basename(fname));
	putchar('\n');

	// Walk
	if ((stbuf.st_mode & S_IFMT) == S_IFDIR) { // is dir
		depth++;
		dirwalk(fname, fsize);
		depth--;
	}
}

void dirwalk(char *dir, void (*fcn)(char *)) /* dirwalk: apply fcn to all files in dir */
{
	char name[MAX_PATH];
	Dirent *de;
	DIR *dp;

	if ((dp = opendir(dir)) == NULL) {
		fprintf(stderr, "dirwalk: can't open %s\n", dir);
		return;
	}

	while ((de = readdir(dp)) != NULL) {
		if (strcmp(de->name, ".") == 0 || strcmp(de->name, "..") == 0 || strncmp(de->name, ".", 1) == 0)
			continue; /* skip self and parent and hidden */
		if (strlen(dir) + strlen(de->name) + 2 > sizeof(name))
			fprintf(stderr, "dirwalk: name %s %s too long\n", dir, de->name);
		else {
			sprintf(name, "%s/%s", dir, de->name);
			(*fcn)(name);
		}
	}
	
	closedir(dp);
}

/* ****************************** Main ****************************** */
int main(int argc, char *argv[])
{
	if (argc == 1)
		fsize(".");
	else
		while (--argc > 0)
			fsize(*++argv);

	return 0;
}
