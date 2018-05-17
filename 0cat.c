/* Copyright by Christian Jaeger, ch@christianjaeger.ch
   Published under the terms of the GNU GPL */

#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/sendfile.h>

void usage () {
    fprintf(stderr,
	    "usage: 0cat filename\n"
	    "  Opens filename then sendfile's its filehandle to stdout.\n"
	    "  Afterwards exits with exit code 0 if successful.\n");
    exit(1);
}


int main (int argn, char**argv) {
    if (argn!= 2) {
	usage ();
    }
    int fd= open(argv[1], O_RDONLY);
    if (fd < 0) {
	fprintf(stderr, "0cat: open: %s: %s\n",
		argv[1],
		strerror(errno));
	exit(1);
    }
    struct stat st;
    if (fstat(fd, &st) < 0) {
	fprintf(stderr, "0cat: fstat for: %s: %s\n",
		argv[1],
		strerror(errno));
	exit(1);
    }
    size_t len= st.st_size;
    while (len) {
	ssize_t nsent= sendfile(1, fd, NULL, len);
	if (nsent < 0) {
	    fprintf(stderr, "0cat: sendfile for: %s: %s\n",
		    argv[1],
		    strerror(errno));
	    exit(1);
	}
	len-= nsent;
	if (len) { fprintf(stderr,"0cat: %i bytes to go\n", len); }
    }
    if (close(1)<0) {
	fprintf(stderr, "0cat: close stdout: %s\n",
		strerror(errno));
	exit(1);
    }
    if (close(fd)<0) {
	fprintf(stderr, "0cat: close: %s: %s\n",
		argv[1],
		strerror(errno));
	exit(1);
    }
    return 0;
}
