#include <stdio.h>

#include "bytecodes.h"
#include "debug.h"

void disassemble_bytecodes(ByteCodes *bytecodes, const char *name) {

  printf("----++++++++==< %s >==++++++++----\n", name);

  for (size_t offset = 0; offset < bytecodes->count;) {
    offset = disassemble_instruction(bytecodes, offset);
  }
}

static size_t simple_instruction(const char *name, size_t offset) {

  printf("%s\n", name);
  return offset + 1;
}

static size_t constant_instruction(const char *name, ByteCodes *bytecodes,
                                   size_t offset) {

  uint8_t constant = bytecodes->code[offset + 1];

  printf("%-16s %4d\t", name, constant);

  print_value(bytecodes->values.value_array[constant]);

  printf("\n");

  return offset + 2;
}

static size_t byte_instruction(const char *name, ByteCodes *bytecodes,
                               size_t offset) {
  uint8_t slot = bytecodes->code[offset + 1];
  printf("%-16s %4d\t\n", name, slot);

  return offset + 2;
}

static size_t jump_instruction(const char *name, int sign, ByteCodes *bytecodes,
                               size_t offset) {

  uint16_t jump = (uint16_t)(bytecodes->code[offset + 1] << 8);
  jump |= (bytecodes->code[offset + 2]);

  printf("%-16s %4d -> %d\t\n", name, (int)offset,
         (int)offset + 3 + sign * jump);

  return offset + 3;
}

size_t disassemble_instruction(ByteCodes *bytecodes, size_t offset) {

  printf("[Line: %04d] %04d\t", (int)bytecodes->lines[offset], (int)offset);

  uint8_t instruction = bytecodes->code[offset];

  switch (instruction) {

  case OP_CONSTANT: {
    return constant_instruction("OP_CONSTANT", bytecodes, offset);
  }

  case OP_NEGATE: {
    return simple_instruction("OP_NEGATE", offset);
  }

  case OP_ADD: {
    return simple_instruction("OP_ADD", offset);
  }

  case OP_SUBTRACT: {
    return simple_instruction("OP_SUBTRACT", offset);
  }

  case OP_MULTIPLY: {
    return simple_instruction("OP_MULTIPLY", offset);
  }

  case OP_DIVIDE: {
    return simple_instruction("OP_DIVIDE", offset);
  }

  case OP_NIL: {
    return simple_instruction("OP_NIL", offset);
  }

  case OP_TRUE: {
    return simple_instruction("OP_TRUE", offset);
  }

  case OP_FALSE: {
    return simple_instruction("OP_FALSE", offset);
  }

  case OP_NOT: {
    return simple_instruction("OP_INSTRUCTION", offset);
  }

  case OP_EQUAL: {
    return simple_instruction("OP_EQUAL", offset);
  }

  case OP_GREATER: {
    return simple_instruction("OP_GREATER", offset);
  }

  case OP_LESS: {
    return simple_instruction("OP_LESS", offset);
  }

  case OP_DISPLAY: {
    return simple_instruction("OP_DISPLAY", offset);
  }

  case OP_POP: {
    return simple_instruction("OP_POP", offset);
  }

  case OP_DEFINE_GLOBAL: {
    return constant_instruction("OP_DEFINE_GLOBAL", bytecodes, offset);
  }

  case OP_GET_GLOBAL: {
    return constant_instruction("OP_GET_GLOBAL", bytecodes, offset);
  }

  case OP_SET_GLOBAL: {
    return constant_instruction("OP_SET_GLOBAL", bytecodes, offset);
  }

  case OP_SET_LOCAL: {
    return byte_instruction("OP_SET_LOCAL", bytecodes, offset);
  }

  case OP_GET_LOCAL: {
    return byte_instruction("OP_GET_LOCAL", bytecodes, offset);
  }

  case OP_CALL: {
    return byte_instruction("OP_CALL", bytecodes, offset);
  }

  case OP_JUMP: {
    return jump_instruction("OP_JUMP", 1, bytecodes, offset);
  }

  case OP_JUMP_IF_FALSE: {
    return jump_instruction("OP_JUMP_IF_FALSE", 1, bytecodes, offset);
  }

  case OP_LOOP: {
    return jump_instruction("OP_LOOP", -1, bytecodes, offset);
  }

  case OP_RETURN: {
    return simple_instruction("OP_RETURN", offset);
  }

  default: {
    printf("Unknown opcode %d\n", instruction);
    return offset + 1;
  }
  }
}
