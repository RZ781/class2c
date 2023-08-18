// https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-4.html#jvms-4.4

#ifndef CONSTPOOL_H
#define CONSTPOOL_H

#include <stdint.h>
#define CONSTANT_Class              7
#define CONSTANT_Fieldref           9
#define CONSTANT_Methodref          10
#define CONSTANT_InterfaceMethodref 11
#define CONSTANT_String             8
#define CONSTANT_Integer            3
#define CONSTANT_Float              4
#define CONSTANT_Long               5
#define CONSTANT_Double             6
#define CONSTANT_NameAndType        12
#define CONSTANT_Utf8               1
#define CONSTANT_MethodHandle       15
#define CONSTANT_MethodType         16
#define CONSTANT_InvokeDynamic      18

typedef struct {
	uint8_t tag;
	union {
		uint16_t u2[4];
		uint32_t u4[2];
	} info;
	uint8_t* ptr;
} cp_info;

typedef struct {
	uint16_t count;
	cp_info* cp;
} cp_t;

#endif
