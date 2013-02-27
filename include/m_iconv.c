#include <stdio.h>
#include <error.h>
#include <errno.h>
#include <iconv.h>

int
m_iconv(const char *incode, const char *in, size_t inleft,
		const char *outcode, char *out, size_t outleft)
{
	char *inbuf, *outbuf;
	size_t nconv;
	iconv_t cd;

	// iconv_open
	cd = iconv_open(outcode, incode);
	if (cd == (iconv_t) -1) {
		if (errno == EINVAL)
			error(0, 0, "conversion from '%s' to '%s' not available", incode, outcode);
		else
			perror("iconv_open");

		return -1;
	}

	// Convert
	inbuf = (char *) in, outbuf = out;
	nconv = iconv(cd, &inbuf, &inleft, &outbuf, &outleft);
	if (nconv == (size_t) -1) {
		perror("iconv");
		return -1;
	}
	if (outleft > 0)
		*outbuf = '\0';

	// iconv_close
	if (iconv_close(cd) != 0)
		perror("iconv_close");
	
	return (int) (outbuf - out);
}
