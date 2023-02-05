CC = gcc
CFLAGS = -Wall -Wextra -g

all: main

main: main.o print_page.o
main.o: main.c
print_page.o: print_page.c print_page.h

.PHONY: clean

clean:
	${RM} main main.o print_page.o
