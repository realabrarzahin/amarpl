#ifndef BYTECODES_H
#define BYTECODES_H

#include "common.h"
#include "value.h"

typedef enum {

  OP_CONSTANT,
  OP_NEGATE,
  OP_RETURN,
  OP_ADD,
  OP_SUBTRACT,
  OP_MULTIPLY,
  OP_DIVIDE,
  OP_NIL,
  OP_TRUE,
  OP_FALSE,
  OP_NOT,
  OP_EQUAL,
  OP_GREATER,
  OP_LESS,
  OP_DISPLAY,
  OP_POP,
  OP_DEFINE_GLOBAL,
  OP_GET_GLOBAL,
  OP_SET_GLOBAL,
  OP_GET_LOCAL,
  OP_SET_LOCAL,
  OP_JUMP_IF_FALSE,
  OP_JUMP,
  OP_LOOP,
  OP_CALL,

} OpCode;

typedef struct {

  size_t count;
  size_t capacity;
  uint8_t *code;
  Values values;
  size_t *lines;

} ByteCodes;

void init_bytecodes(ByteCodes *bytecodes);
void write_bytecodes(ByteCodes *bytecodes, uint8_t byte, size_t line);
void free_bytecodes(ByteCodes *bytecodes);
size_t add_constant(ByteCodes *bytecodes, Value value);

#endif
