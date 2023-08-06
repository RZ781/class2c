// https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-4.html#jvms-4.6

#ifndef METHOD_INFO
#define METHOD_INFO

#include <stdint.h>
#include "attrinfo.h"

typedef struct {
	uint16_t access_flags;
	uint16_t name_index;
	uint16_t descriptor;
	attrs_t attrs;
} method_info;

typedef struct {
	uint16_t count;
	method_info* methods;
} methods_t;

#endif
