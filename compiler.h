#ifndef COMPILER_H
#define COMPILER_H

#include "bytecodes.h"
#include "common.h"
#include "object.h"

ObjFunction *compile(const char *source);
void mark_compiler_roots();

#endif
