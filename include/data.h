#ifndef DATA_H
#define DATA_H
#include "classfile.h"
#include "constpool.h"

typedef struct {
	uint16_t length;
	char* data;
} string;

cp_info get_const(class_file_t jclass, uint16_t i);
string utf8_const(class_file_t jclass, uint16_t i);
char* string_data(string s);
char* concat(int length, int n, ...);

#endif
