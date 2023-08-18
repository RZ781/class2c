#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "compiler.h"

int main(int argc, char** argv) {
	if (argc < 2) {
		fprintf(stderr, "error: no input files provided\n");
		exit(-1);
	}
	FILE* f = fopen(argv[1], "r");
	if (f == NULL) {
		fprintf(stderr, "error: could not open file %s\n", argv[0]);
		exit(-1);
	}
	class_file_t main_class = parse_class(f);
	fclose(f);
	compile_class(main_class, "output/src/", "output/include/");
}
