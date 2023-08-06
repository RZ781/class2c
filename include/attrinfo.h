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

#endif
