#include <stdio.h>
#include <stdlib.h>
#include "print_page.h"

int main(int argc, char *argv[])
{
    // - Your main function must accept only one command-line argument,
    //   which is the host name.
    // - If the number of arguments is different from one,
    //   your program must exit immediately with an error message.
    if (argc != 2)
    {
        printf("[ERROR] usage: ./main <hostname>\n");
        exit(EXIT_FAILURE);
    }
    // Your main function should call the print_page() function
    // with the host name.
    print_page(argv[1]);
    return 0;
}