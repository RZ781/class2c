#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "classfile.h"
#include "attrinfo.h"
#include "methodinfo.h"
#include "fieldinfo.h"

uint8_t parse_u1(FILE* f) {
	int c = fgetc(f);
	if (c == EOF) {
		fprintf(stderr, "error: unexpected eof\n");
		exit(-1);
	}
	return c;
}
uint16_t parse_u2(FILE* f) {
	int a = parse_u1(f);
	int b = parse_u1(f);
	return (a << 8) + b;
}
uint32_t parse_u4(FILE* f) {
	int a = parse_u2(f);
	int b = parse_u2(f);
	return (a << 16) + b;
}

cp_t parse_cp(FILE* f) {
	cp_t const_pool;
	const_pool.count = parse_u2(f);
	const_pool.cp = calloc(sizeof(cp_info), const_pool.count);
	for (int i=1; i < const_pool.count; i++) {
		cp_info entry;
		entry.tag = parse_u1(f);
		switch (entry.tag) {
			case CONSTANT_Class:
			case CONSTANT_String:
			case CONSTANT_MethodType:
				entry.info.u2[0] = parse_u2(f);
				break;
			case CONSTANT_Fieldref:
			case CONSTANT_Methodref:
			case CONSTANT_InterfaceMethodref:
			case CONSTANT_NameAndType:
				entry.info.u2[0] = parse_u2(f);
				entry.info.u2[1] = parse_u2(f);
				break;
			case CONSTANT_Integer:
			case CONSTANT_Float:
				entry.info.u4[0] = parse_u4(f);
				break;
			case CONSTANT_Long:
			case CONSTANT_Double:
				entry.info.u4[0] = parse_u4(f);
				entry.info.u4[1] = parse_u4(f);
				break;
			case CONSTANT_MethodHandle:
				entry.info.u2[0] = parse_u1(f);
				entry.info.u2[0] = parse_u2(f);
				break;
			case CONSTANT_Utf8:
				entry.info.u2[0] = parse_u2(f);
				entry.ptr = malloc(entry.info.u2[0]);
				for (int i=0; i<entry.info.u2[0]; i++)
					entry.ptr[i] = parse_u1(f);
				break;
			default:
				fprintf(stderr, "unknown const tag %i\n", entry.tag);
				exit(-1);
		}
		const_pool.cp[i] = entry;
	}
	return const_pool;
}

attrs_t parse_attrs(FILE* f) {
	attrs_t attrs;
	attrs.count = parse_u2(f);
	attrs.attrs = calloc(sizeof(attr_info), attrs.count);
	for (int i=0; i<attrs.count; i++) {
		attr_info attr;
		attr.name_index = parse_u2(f);
		attr.length = parse_u4(f);
		attr.info = malloc(attr.length);
		fread(attr.info, attr.length, 1, f);
		attrs.attrs[i] = attr;
	}
	return attrs;
}

fields_t parse_fields(FILE* f) {
	fields_t fields;
	fields.count = parse_u2(f);
	fields.fields = calloc(sizeof(field_info), fields.count);
	for (int i=0; i<fields.count; i++) {
		field_info field;
		field.access_flags = parse_u2(f);
		field.name_index = parse_u2(f);
		field.descriptor = parse_u2(f);
		field.attrs = parse_attrs(f);
		fields.fields[i] = field;
	}
	return fields;
}
methods_t parse_methods(FILE* f) {
	methods_t methods;
	methods.count = parse_u2(f);
	methods.methods = calloc(sizeof(method_info), methods.count);
	for (int i=0; i<methods.count; i++) {
		method_info method;
		method.access_flags = parse_u2(f);
		method.name_index = parse_u2(f);
		method.descriptor = parse_u2(f);
		method.attrs = parse_attrs(f);
		methods.methods[i] = method;
	}
	return methods;
}

class_file_t parse_class(FILE* f) {
	class_file_t jclass;
	uint32_t magic = parse_u4(f);
	if (magic != 0xcafebabe) {
		fprintf(stderr, "error: invalid magic number %x\n", magic);
		exit(-1);
	}
	jclass.minor_version   = parse_u2(f);
	jclass.major_version   = parse_u2(f);
	jclass.const_pool      = parse_cp(f);
	jclass.access_flags    = parse_u2(f);
	jclass.this_class      = parse_u2(f);
	jclass.super_class     = parse_u2(f);
	jclass.interface_count = parse_u2(f);
	jclass.interfaces = calloc(sizeof(uint16_t), jclass.interface_count);
	for (int i=0; i<jclass.interface_count; i++)
		jclass.interfaces[i] = parse_u2(f);
	jclass.fields  = parse_fields(f);
	jclass.methods = parse_methods(f);
	jclass.attrs   = parse_attrs(f);
	return jclass;
}
