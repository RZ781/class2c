// https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-4.html#jvms-4.5

#ifndef FIELD_INFO_H
#define FIELD_INFO_H

#include <stdint.h>
#include "attrinfo.h"

typedef struct {
	uint16_t access_flags;
	uint16_t name_index;
	uint16_t descriptor;
	attrs_t attrs;
} field_info;

typedef struct {
	uint16_t count;
	field_info* fields;
} fields_t;

#endif
