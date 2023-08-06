#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

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
	printf("version: %d %d\n", main_class.major_version, main_class.minor_version);
	printf("access: %x\n", main_class.access_flags);
	fclose(f);
}
