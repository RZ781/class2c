#ifndef DATA_H
#define DATA_H
#include <stdbool.h>
#include "classfile.h"
#include "constpool.h"

typedef struct {
	uint16_t length;
	char* data;
} string;

void make_full_path(char* path);
cp_info get_const(class_file_t jclass, uint16_t i);
string utf8_const(class_file_t jclass, uint16_t i);
char* string_data(string s);
void format_id(char* dest, const char* s, bool to_upper);
void free_class(class_file_t jclass);

#endif
