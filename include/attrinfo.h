// https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-4.html#jvms-4.7
#ifndef ATTR_INFO_H
#define ATTR_INFO_H
#include <stdint.h>

typedef struct {
	uint16_t name_index;
	uint32_t length;
	uint8_t* info;
} attr_info;

typedef struct {
	uint16_t count;
	attr_info* attrs;
} attrs_t;

typedef struct {
	uint16_t start_pc;
	uint16_t end_pc;
	uint16_t handler_pc;
	uint16_t catch_type;
} exception_info;

typedef struct {
	uint16_t stack_size;
	uint16_t locals;
	uint32_t code_length;
	uint8_t* bytecode;
	uint16_t exception_count;
	exception_info* exception_table;
	attrs_t attrs;
} code_attr;

#endif
