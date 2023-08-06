// https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-4.html#jvms-4.1

#ifndef CLASS_FILE_H
#define CLASS_FILE_H

#include <stdint.h>
#include "constpool.h"
#include "fieldinfo.h"
#include "methodinfo.h"
#include "attrinfo.h"

typedef struct {
	uint16_t major_version;
	uint16_t minor_version;

	uint16_t access_flags;
	uint16_t this_class;
	uint16_t super_class;

	uint16_t interface_count;
	uint16_t* interfaces;
	cp_t const_pool;
	fields_t fields;
	methods_t methods;
	attrs_t attrs;
} class_file_t;

#endif
