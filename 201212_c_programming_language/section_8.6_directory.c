#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syscall.h>
#include <unistd.h>
#include "section_8.6_directory.h"

/* ****************************** Dir ****************************** */

/* opendir: open a directory for readdir calls */
DIR *opendir(char *dirname)
{
	int fd;
	struct stat stbuf;
	DIR *dp;

	if ((fd = open(dirname, (O_RDONLY | O_DIRECTORY), 0)) == -1
		|| fstat(fd, &stbuf) == -1
		|| (stbuf.st_mode & S_IFMT) != S_IFDIR
		|| (dp = (DIR *) malloc(sizeof(DIR))) == NULL) {
		return NULL;
	}

	dp->fd = fd;
	dp->l_bpos = dp->l_nread = 0;
	return dp;
}

/* closedir: close directory opened by opendir */
void closedir(DIR *dp)
{
	if (dp) {
		close(dp->fd);
		free(dp);
	}
}

#ifdef _LEARN_IN_LINUX // IN Linux

/* 
 * 此处因为例子中是Unix，使用read读取目录
 * Linux中是使用系统指令getdents
 */
Dirent *readdir(DIR *dp)
{
	static Dirent d; /* return: portable structure */
	struct linux_dirent *ld;

	if (dp->l_bpos >= dp->l_nread) {
		dp->l_nread = syscall(SYS_getdents, dp->fd, dp->l_buf, GETDENTS_BUF);
		dp->l_bpos = 0;
		if (dp->l_nread == 0)
			return NULL;
		else if (dp->l_nread == -1)
			return NULL;
	}

	ld = (struct linux_dirent *) (dp->l_buf + dp->l_bpos);
	d.ino = ld->d_ino;
	strcpy(d.name, ld->d_name);

	dp->l_bpos += ld->d_reclen;

	return &d;
}

#else // TCPL Oringinal

#ifndef DIRSIZ
#define DIRSIZ 14
#endif
struct direct { /* directory entry */
	ino_t d_ino; /* inode number */
	char d_name[DIRSIZ]; /* long name does not have '\0' */
};

//#include <sys/dir.h> /* local directory structure */
/* readdir: read directory entries in sequence */
Dirent *readdir(DIR *dp)
{
	struct direct dirbuf; /* local directory structure */
	static Dirent d; /* return: portable structure */

	while (read(dp->fd, (char *) &dirbuf, sizeof(dirbuf)) == sizeof(dirbuf)) {
		if (dirbuf.d_ino == 0) /* slot not in use */
			continue;
		d.ino = dirbuf.d_ino;
		strncpy(d.name, dirbuf.d_name, DIRSIZ);
		d.name[DIRSIZ] = '\0'; /* ensure termination */
		return &d;
	}
	return NULL;
}

#endif
