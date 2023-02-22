#define _GNU_SOURCE

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <unistd.h>

const size_t BUFFER_SIZE = 32;

void rewrite(int fd, const void *buf, size_t count)
{
	int left = count;
	void *p_buf = buf;
	do {
		int n_written = write(fd, p_buf, left);
		if (n_written == -1) {
			errx(-1, "write()\n");
		}
		p_buf += n_written;	
		left -= n_written;
	} while (left > 0);
}

char *build_query(const char *host, size_t *len)
{
	char *ret = NULL;
	*len = asprintf(&ret, "GET http://www.%s HTTP/1.0\r\n\r\n", host);
	if (ret == NULL) {
		errx(-1, "asprintf()");
	}
	return ret;
}

void print_page(const char *host)
{
	char buffer[BUFFER_SIZE];
 	size_t len;

	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	
	struct addrinfo *addr = NULL;
	int err = getaddrinfo(host, "80", &hints, &addr);
	if (err != 0) {
		errx(err, "addrinfo(): %s\n", gai_strerror(err));
	}
	
	int sfd;
	for (struct addrinfo *p = addr; p != NULL; p = p->ai_next) {
		// Create a socket
		sfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (sfd == -1) {
			continue;
		}
		// Try to connect the socket
		int err = connect(sfd, p->ai_addr, p->ai_addrlen);
		if (err == 0) {
			break;
		} else {
			close(sfd);
		}
	}
	freeaddrinfo(addr);
	 
	char* query = build_query(host, &len);
	rewrite(sfd, query, strlen(query));
	free(query);

	int n_read;
	do {
		n_read = read(sfd, buffer, BUFFER_SIZE);
		for (int i = 0; i < n_read; i++) {
			printf("%c", buffer[i]);
		}
	} while (n_read > 0);
	close(sfd);
}
