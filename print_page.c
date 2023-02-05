#define _GNU_SOURCE

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <unistd.h>

const size_t BUFFER_SIZE = 32;

void print_addrinfo(struct addrinfo res)
{
	printf("--------\n");
	printf("ai_flags: %d\n", res.ai_flags);
	printf("ai_family: %d\n", res.ai_family);
	printf("ai_socktype: %d\n", res.ai_socktype);
	printf("ai_protocol: %d\n", res.ai_protocol);
	printf("ai_addrlen: %d\n", res.ai_addrlen);
	// printf("ai_canonname: %s\n", res.ai_canonname);
	// printf("ai_addr: %s\n", res.ai_addr);
}

void rewrite(int fd, const void *buf, size_t count)
{
	/*
	 The rewrite() function is similar to the write(2) function.
	 The differences are as follows:
	 - When an error occurs, rewrite() prints an error message
	   and exits the program (it does not return anything).
	 - The number of bytes sent by write()
	   may be smaller than the count parameter.
	   With rewrite(), all of the bytes are sent.

	 Tips:
	 - Obviously, you have to call the write() function.
	 - If an error occurs, exit the program with an error message.
	   You should use err(3) or errx(3).
	 - If the return value of write() is smaller than its third argument,
	   you must call write() again in order to have the rest of the data written.
	 - Repeat this until all the data has been sent,
	   but be careful not to send the same bytes more than once.
	*/
	size_t remaining = count;
	char *restart = buf + strlen(buf) - remaining;
	while (remaining > 0)
	{
		ssize_t sent = write(fd, restart, remaining);
		if (sent == -1)
		{
			err(EXIT_FAILURE, "Could not write to socket");
		}
		remaining -= sent;
		restart += sent;
	}
}

char *build_query(const char *host, size_t *len)
{
	/*
	 Arguments:
	 - host:
		 the host name.
	 - len:
		 a pointer to the length of the request.
		 This argument must return the length of the request that will be generated.

	 Return Value:
	 - the request that has been generated.
	   This string of characters must be dynamically generated
	   and will be freed by the caller.

	 Note:
	 - For instance, if the host name is "example.com",
	   the generated request should be:
		 "GET http://www.example.com/ HTTP/1.0\r\n\r\n"
	   If the request cannot be generated, the program should exit immediately
	   with an error message. See asprintf(3).
	*/
	// My Comment:
	//  We can use asprintf(3) like this:
	//  int size = asprintf(&request, "GET %s HTTP/1.0\r\n\r\n", host);
	//  if (size == -1) { // error }
	//  But, asprintf(3) is not C standard and is not available on all systems
	//  Use malloc(3) and strcat(3) instead.
	char prefix[] = "GET http://www.";
	char postfix[] = " HTTP/1.0\r\n\r\n";
	char *request = malloc(
		strlen(prefix) + strlen(host) + strlen(postfix) + 1);
	if (request == NULL)
	{
		printf("[ERROR] Could not allocate memory for request");
		exit(EXIT_FAILURE);
	}
	strcat(request, prefix);
	strcat(request, host);
	strcat(request, postfix);
	*len = strlen(request);
	return request;
}

void print_page(const char *host)
{
	/*
	 - In order to connect to the host, you will use the getaddrinfo(3) function.
	   The hints argument will point to an addrinfo structure.
		- Only two fields of this structure should not be null:
		- ai_family should be equal to AF_INET. ✅
		- ai_socktype should be equal to SOCK_STREAM. ✅
	 - To initialize the other fields to zero, you should use memset(3). ✅
	 - The addrinfo() function returns a linked list. You should iterate through this list
	   and try (for each element) to create a socket and connect it to the host. ✅
	 - When a connection for at least one element of the list is successful,
	   you can break the loop and free the list. ✅
	 - If no connection has succeeded,
	   your program must exit immediately with an error message. ✅
	 - To create and connect a socket, you should use the socket(2) and connect(2) functions.
	*/
	struct addrinfo hints;
	struct addrinfo *result;

	// Specifying criteria
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;		 // IPv4
	hints.ai_socktype = SOCK_STREAM; // TCP
	int err = getaddrinfo(host, "80", &hints, &result);

	// Error handling
	if (err != 0)
	{
		printf("[ERROR] %s\n", gai_strerror(err));
		exit(EXIT_FAILURE);
	}

	// Iterate over the linked list
	int socket_fd;
	struct addrinfo *p;
	for (p = result; p != NULL; p = p->ai_next)
	{
		// Try to create a socket
		socket_fd = socket(
			p->ai_family,
			p->ai_socktype,
			p->ai_protocol);
		// If error, continue to the next element
		if (socket_fd == -1)
		{
			continue;
		}
		// Try to connect to the socket
		err = connect(
			socket_fd,
			p->ai_addr,
			p->ai_addrlen);
		// If successful, break the loop
		if (err != -1)
		{
			break;
		}
		// If error, close the socket
		else
		{
			close(socket_fd);
		}
	}
	// Free the linked list
	freeaddrinfo(result);
	if (p == NULL)
	{
		// All attempts are unsuccessful
		printf("[ERROR] Could not connect to %s", host);
		exit(EXIT_FAILURE);
	}
	// Make the request
	size_t len;
	char *query = build_query(host, &len);
	// Send the request
	size_t written = write(socket_fd, query, len);
	rewrite(socket_fd, query, len - written);
	// Print the response
	char buffer[BUFFER_SIZE];
	while (read(socket_fd, buffer, BUFFER_SIZE) > 0)
	{
		for (int i = 0; i < (int)strlen(buffer); i++)
		{
			printf("%c", buffer[i]);
		}
		memset(buffer, '\0', BUFFER_SIZE);
	}
	close(socket_fd);
}