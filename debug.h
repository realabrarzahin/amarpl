#ifndef DEBUG_H
#define DEBUG_H

#include "bytecodes.h"

void disassemble_bytecodes(ByteCodes *bytecodes, const char *name);
size_t disassemble_instruction(ByteCodes *bytecodes, size_t offset);

#endif
