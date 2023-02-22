#include "print_page.h"
#include <err.h>

int main(int argc, char **argv) {
	if (argc != 2) {
		errx(-1, "argc, argv");
	}
	print_page(argv[1]);
	return 0;
}
