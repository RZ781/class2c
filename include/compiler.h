#ifndef COMPILER_H
#define COMPILER_H
#include <stdint.h>
#include "classfile.h"

void compile_class(class_file_t jclass, char* src_dir, char* include_dir);
void compile_method(class_file_t jclass, uint16_t method, FILE* file);

#endif
