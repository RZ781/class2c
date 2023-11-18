#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "compiler.h"
#include "classfile.h"
#include "access.h"
#include "data.h"

void compile_class(class_file_t jclass, char* src_dir, char* inc_dir) {
	cp_info class_const = get_const(jclass, jclass.this_class);
	string name = utf8_const(jclass, class_const.info.u2[0]);

	// generate source and header file names
	int source_len = strlen(src_dir) + name.length + 2;
	int header_len = strlen(inc_dir) + name.length + 2;
	char* source_name = calloc(sizeof(char), source_len + 1);
	char* header_name = calloc(sizeof(char), header_len + 1);
	sprintf(source_name, "%s%s.c", src_dir, string_data(name));
	sprintf(header_name, "%s%s.h", inc_dir, string_data(name));
	make_full_path(source_name);
	make_full_path(header_name);

	FILE* source = fopen(source_name, "w");
	FILE* header = fopen(header_name, "w");
	free(source_name);
	free(header_name);
	if (!source) {
		fprintf(stderr, "error: failed to open file %s\n", source_name);
		exit(-1);
	}
	if (!header) {
		fprintf(stderr, "error: failed to open file %s\n", header_name);
		exit(-1);
	}

	// includes in source
	fprintf(source, "#include \"%s\".h", string_data(name));

	// include guard
	char* inc_guard = malloc(sizeof(char) * (name.length + 2));
	inc_guard[0] = '_';
	format_id(inc_guard+1, string_data(name), true);
	fprintf(header, "#ifndef %s\n", inc_guard);
	fprintf(header, "#define %s\n", inc_guard);
	free(inc_guard);

	// generate struct
	char* struct_name = malloc(sizeof(char) * (name.length + 1));
	format_id(struct_name, string_data(name), false);
	fprintf(header, "struct %s {", struct_name);
	fprintf(header, "}\n");
	free(struct_name);

	// compile methods
	for (int i=0; i<jclass.methods.count; i++)
		compile_method(jclass, i, source);

	// end files
	fprintf(header, "#endif\n");	
	fclose(source);
	fclose(header);
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
