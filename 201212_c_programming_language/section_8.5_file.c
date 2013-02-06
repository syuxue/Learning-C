#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "section_8.5_file.h"

#define PERMS 0666

/* ****************************** Global Variable ****************************** */

FILE _iob[OPEN_MAX] = {
	{ 0, (char *) 0, (char *) 0, _READ, 0 }, // stdin
	{ 0, (char *) 0, (char *) 0, _WRITE, 1 }, // stdout
	{ 0, (char *) 0, (char *) 0, _WRITE | _UNBUF, 2 } // stderr
};

/* ****************************** Function ****************************** */

FILE *fopen(char *name, char *mode)
{
	int fd;
	FILE *fp;

	// Check mode
	if (*mode != 'r' && *mode != 'w' && *mode != 'a')
		return NULL;

	// Find a slot
	for (fp = _iob; fp < _iob + OPEN_MAX; fp++)
		if ((fp->flag & (_READ | _WRITE)) == 0)
			break; /* found free slot */
	if (fp >= _iob + OPEN_MAX) /* no free slots */
		return NULL;

	// Open file descriptor
	if (*mode == 'w') {
		fd = creat(name, PERMS);
	} else if (*mode == 'a') {
		if ((fd = open(name, O_WRONLY, 0)) == -1)
			fd = creat(name, PERMS);
		lseek(fd, 0L, 2);
	} else {
		fd = open(name, O_RDONLY, 0);
	}
	if (fd == -1) /* couldn't access name */
		return NULL;
	
	// Make file pointer
	fp->cnt = 0;
	fp->ptr = fp->base = NULL;
	fp->flag = (*mode == 'r') ? _READ : _WRITE;
	fp->fd = fd;

	return fp;
}

int fclose(FILE *fp)
{
	if (fp->cnt > -1)
		fflush(fp);
	if (fp->base != NULL)
		free(fp->base);
	if (!close(fp->fd))
		return EOF;

	// Clear file pointer
	fp->cnt = fp->flag = 0;
	fp->ptr = fp->base = NULL;
	fp->fd = EOF;

	return 0;
}

int fflush(FILE *fp)
{
	// Check flag
	if (fp->flag & (_EOF | _ERR))
		return EOF;

	// Flush write buff
	if ((fp->flag & (_WRITE | _UNBUF)) == _WRITE) {
		_flushbuf(EOF, fp);
	} else if ((fp->flag & (_READ | _UNBUF)) == _READ) {
		fp->ptr = fp->base;
		fp->cnt = 0;
	}

	return 0;
}

int fseek(FILE *fp, long offset, int origin)
{
	// Check flag
	if (fp->flag & (_EOF | _ERR))
		return EOF;

	if (!(fp->flag & _UNBUF)) {
		if (fp->flag & _READ && origin == SEEK_CUR) // getc时，由于先读入buf，所以fd的位置已经是CUR + BUFSIZ了
			offset -= fp->cnt;
		fflush(fp);
	}
	if (lseek(fp->fd, offset, origin) == EOF)
		return EOF;

	return 0;
}

int _fillbuf(FILE *fp)
{
	int bufsize;

	// Check flag
	if ((fp->flag & (_READ | _EOF | _ERR)) != _READ)
		return EOF;

	// Alloc buffer
	bufsize = (fp->flag & _UNBUF) ? 1 : BUFSIZ;
	if (fp->base == NULL) /* no buffer yet */
		if ((fp->base = (char *) malloc(bufsize)) == NULL)
			return EOF; /* can't get buffer */

	// Read
	fp->ptr = fp->base;
	fp->cnt = read(fp->fd, fp->ptr, bufsize);

	// Check EOF
	if (--fp->cnt < 0) {
		if (fp->cnt == -1)
			fp->flag |= _EOF;
		else
			fp->flag |= _ERR;
		fp->cnt = 0;
		return EOF;
	}

	return (unsigned char) *fp->ptr++;
}

int _flushbuf(int c, FILE *fp)
{
	unsigned char uc;

	// Check flag
	if ((fp->flag & (_WRITE | _EOF | _ERR)) != _WRITE)
		return EOF;

	if (fp->flag & _UNBUF) { // Unbuffered
		uc = c;
		write(fp->fd, &uc, 1);
	} else { // Buffered
		if (fp->base == NULL && !(fp->flag & _UNBUF)) { // Alloc buffer
			if ((fp->ptr = fp->base = (char *) malloc(BUFSIZ)) == NULL)
				return EOF;
			fp->cnt = BUFSIZ - 1;
			*(fp->ptr++) = c;
		} else {
			if (c != EOF)
				*(fp->ptr++) = c;
			write(fp->fd, fp->base, fp->ptr - fp->base);
			fp->cnt = BUFSIZ - 1;
			fp->ptr = fp->base;
		}
	}

	return (unsigned char) c;
}
