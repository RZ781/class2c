#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>
#include <errno.h>
#include "compiler.h"
#include "classfile.h"
#include "access.h"
#include "data.h"

void make_full_path(char* path) {
	int length = strlen(path);
	for (int i=0; i<length; i++) {
		if (path[i] != '/')
			continue;
		char c = path[i];
		path[i] = 0;
		if (mkdir(path, 0777) && errno != EEXIST) {
			fprintf(stderr, "error: failed to create folder %s (code: %i)\n", path, errno);
			exit(-1);
		}
		path[i] = c;
	}
}

void compile_class(class_file_t jclass, char* src_dir, char* inc_dir) {
	cp_info class_const = get_const(jclass, jclass.this_class);
	string name = utf8_const(jclass, class_const.info.u2[0]);
	// generate source and header file names
	char* source_name = concat(strlen(src_dir) + name.length + 3, 3, src_dir, string_data(name), ".c");
	char* header_name = concat(strlen(inc_dir) + name.length + 3, 3, inc_dir, string_data(name), ".h");
	make_full_path(source_name);
	make_full_path(header_name);
	FILE* source = fopen(source_name, "w");
	FILE* header = fopen(header_name, "w");
	if (!source) {
		fprintf(stderr, "error: failed to open file %s\n", source_name);
		exit(-1);
	}
	if (!header) {
		fprintf(stderr, "error: failed to open file %s\n", source_name);
	}
	// include guard
	fprintf(header, "#ifndef TEST_H\n");
	fprintf(header, "#define TEST_H\n");
	// generate struct
	fprintf(header, "struct %s {", source_name);
	fprintf(header, "}\n");
	for (int i=0; i<jclass.methods.count; i++) // compile methods
		compile_method(jclass, i, source);
	// end include guard
	fprintf(header, "#endif\n");
}
void compile_method(class_file_t jclass, uint16_t method_n, FILE* file) {
	const method_info method = jclass.methods.methods[method_n];
	if (method.access_flags & ACC_ABSTRACT || method.access_flags & ACC_NATIVE) // abstract and native methods don't have code
		return;
	int code_i = -1;
	for (int i=0; i<method.attrs.count; i++) { // find code attribute
		attr_info attr = method.attrs.attrs[i];
		string name = utf8_const(jclass, attr.name_index);
		if (name.length == 4 && memcmp(name.data, "Code", 4) == 0)
			code_i = i;
	}
	if (code_i == -1) {
		fprintf(stderr, "error: concrete method has no code attribute\n");
		exit(-1);
	}
	fprintf(file, "int %s();\n", string_data(utf8_const(jclass, method.name_index)));
}
