#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>
#include "data.h"
#include "constpool.h"

void make_full_path(char* path) {
	int length = strlen(path);
	for (int i=0; i<length; i++) {
		if (path[i] != '/')
			continue;
		path[i] = 0;
		if (mkdir(path, 0777) != 0 && errno != EEXIST) {
			fprintf(stderr, "error: failed to create folder %s (code: %i)\n", path, errno);
			exit(-1);
		}
		path[i] = '/';
	}
}

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

void format_id(char* dest, const char* s, bool to_upper) {
	int i;
	for (i=0; s[i]; i++) {
		if (isalnum(s[i])) {
			if (to_upper)
				dest[i] = toupper(s[i]);
			else
				dest[i] = s[i];
		} else {
			dest[i] = '_';
		}
	}
	dest[i] = 0;
}

void free_attrs(attrs_t attrs) {
	for (int i=0; i<attrs.count; i++) {
		free(attrs.attrs[i].info);
	}
	free(attrs.attrs);
}

void free_class(class_file_t jclass) {
	for (int i=0; i<jclass.const_pool.count; i++) {
		cp_info* info = &jclass.const_pool.cp[i];
		if (info->tag == CONSTANT_Utf8)
			free(info->ptr);
	}
	for (int i=0; i<jclass.fields.count; i++)
		free_attrs(jclass.fields.fields[i].attrs);
	for (int i=0; i<jclass.methods.count; i++)
		free_attrs(jclass.methods.methods[i].attrs);
	free_attrs(jclass.attrs);
	free(jclass.interfaces);
	free(jclass.const_pool.cp);
	free(jclass.fields.fields);
	free(jclass.methods.methods);
}
