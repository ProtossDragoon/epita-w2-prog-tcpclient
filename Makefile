# Compiler
CC = gcc

# Compiler flags
# -std=c99: C standard version
# -g: make debuggable
# -fsanitize=address: help to find memory leak
# -Wall: show all warnings
# -Wextra: show extra warnings
CFLAGS = -std=c99 -g -fsanitize=address -Wall -Wextra 

# Linker flags
LDFLAGS = -fsanitize=address

# File variables
EXC = main 
OBJ = print_page.o main.o
DEP = ${OBJ:.o=.d}

# Instructions
all: main

main: main.o print_page.o

print_page: print_page.o

# Not related to files
.PHONY: clean
clean:
	${RM} ${OBJ} ${DEP} ${EXC}	
