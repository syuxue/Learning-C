/* ****************************** Linux ****************************** */
#define _LEARN_IN_LINUX
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

/* ****************************** TCPL ****************************** */
#define NAME_MAX 255
#define MAX_PATH 1024
#define GETDENTS_BUF 1024

typedef struct { // DIR Entry
	long ino;
	char name[NAME_MAX + 1];
} Dirent;

typedef struct { // DIR
	int fd;
	// l_* 为兼容linux指令getdents
	int l_bpos;
	int l_nread;
	char l_buf[BUFSIZ];
	Dirent d;
} DIR;

/*struct stat {			// inode information returned by stat
	dev_t st_dev;		// device of inode
	ino_t st_ino;		// inode number
	short st_mode;		// mode bits
	short st_nlink;		// number of links to file
	short st_uid;		// owners user id
	short st_gid;		// owners group id
	dev_t st_rdev;		// for special files
	off_t st_size;		// file size in characters
	time_t st_atime;	// time last accessed
	time_t st_mtime;	// time last modified
	time_t st_ctime;	// time originally created
};*/

DIR *opendir(char *dirname);
Dirent *readdir(DIR *dfd);
void closedir(DIR *dfd);

void dirwalk(char *dirname, void (*fcn)(char *));
