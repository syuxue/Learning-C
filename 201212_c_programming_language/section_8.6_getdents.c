#define _GNU_SOURCE
#include <dirent.h>     /* Defines DT_* constants */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <syscall.h>
#include "include/m_function.c"
#define BUFF_SIZE 100

struct linux_dirent {
	unsigned long	d_ino;		/* Inode number */
	unsigned long	d_off;		/* Offset to next linux_dirent */
	unsigned short	d_reclen;	/* Length of this linux_dirent */
	char			d_name[];	/* Filename (null-terminated) */
								/* length is actually (d_reclen - 2 -
								offsetof(struct linux_dirent, d_name) */
	/*
	char			pad;		// Zero padding byte
	char			d_type;		// File type (only since Linux 2.6.4;
								// offset is (d_reclen - 1))
	*/
};

/*
ref: http://www.kernel.org/doc/man-pages/online/pages/man2/getdents.2.html
*/
int main(int argc, char *argv[])
{
	int fd, nread, bpos;
	char d_type, buf_dir[BUFF_SIZE];
	struct linux_dirent *dirp;

	fd = open(argc > 1 ? argv[1] : ".", O_RDONLY | O_DIRECTORY, 0);
	
	// Read Dir
	while ((nread = syscall(SYS_getdents, fd, buf_dir, BUFF_SIZE)) > 0) {

		for (bpos = 0; bpos < nread; ) {
			dirp = (struct linux_dirent *) (buf_dir + bpos);

			printf("%8ld  ", dirp->d_ino);
			d_type = *(buf_dir + bpos + dirp->d_reclen - 1);
			printf("%-10s ", (d_type == DT_REG) ?  "regular" :
							(d_type == DT_DIR) ?  "directory" :
							(d_type == DT_FIFO) ? "FIFO" :
							(d_type == DT_SOCK) ? "socket" :
							(d_type == DT_LNK) ?  "symlink" :
							(d_type == DT_BLK) ?  "block dev" :
							(d_type == DT_CHR) ?  "char dev" : "???");
			printf("%4d %10lld  %s\n", dirp->d_reclen,
			(long long) dirp->d_off, (char *) dirp->d_name);

			bpos += dirp->d_reclen;
		}
	}

	close(fd);

	return nread;
}
