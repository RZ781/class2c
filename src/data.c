#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "data.h"
#include "constpool.h"

cp_info get_const(class_file_t jclass, uint16_t i) {
	if (i >= jclass.const_pool.count) {
		fprintf(stderr, "error: constant pool index out of range\n");
		exit(-1);
	}
	return jclass.const_pool.cp[i];
}

string utf8_const(class_file_t jclass, uint16_t i) {
	string s;
	cp_info utf8_const = get_const(jclass, i);
	s.length = utf8_const.info.u2[0];
	s.data = (char*) utf8_const.ptr;
	return s;
}
char* string_data(string s) {
	if (s.data[s.length]) {
		fprintf(stderr, "error: string does not end in null");
		exit(-1);
	}
	return s.data;
}
char* concat(int length, int n, ...) {
	char* data = malloc(length);
	data[0] = 0;

	va_list args;
	va_start(args, n);

	for (int i=0; i<n; i++) {
		strcat(data, va_arg(args, char*));
	}
	va_end(args);
	return data;
}

