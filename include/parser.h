#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "classfile.h"
#include "attrinfo.h"

uint16_t parse_u2(FILE*);
attrs_t parse_attrs(FILE*);
class_file_t parse_class(FILE*);

#endif
