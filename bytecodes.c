#include <stdlib.h>

#include "bytecodes.h"
#include "memory.h"
#include "value.h"
#include "vm.h"

void init_bytecodes(ByteCodes *bytecodes) {

  bytecodes->count = 0;
  bytecodes->capacity = 0;
  bytecodes->code = NULL;
  bytecodes->lines = NULL;

  init_values(&bytecodes->values);
}

void write_bytecodes(ByteCodes *bytecodes, uint8_t byte, size_t line) {

  if ((bytecodes->capacity) < (bytecodes->count + 1)) {

    size_t old_capacity = bytecodes->capacity;
    bytecodes->capacity = GROW_CAPACITY(old_capacity);
    bytecodes->code =
        GROW_ARRAY(uint8_t, bytecodes->code, old_capacity, bytecodes->capacity);
    bytecodes->lines =
        GROW_ARRAY(size_t, bytecodes->lines, old_capacity, bytecodes->capacity);
  }

  bytecodes->code[bytecodes->count] = byte;
  bytecodes->lines[bytecodes->count] = line;
  bytecodes->count++;
}

void free_bytecodes(ByteCodes *bytecodes) {

  FREE_ARRAY(uint8_t, bytecodes->code, bytecodes->capacity);
  FREE_ARRAY(uint8_t, bytecodes->lines, bytecodes->capacity);
  free_values(&bytecodes->values);
  init_bytecodes(bytecodes);
}

size_t add_constant(ByteCodes *bytecodes, Value value) {

  push_vm_value_stack(value);
  write_values(&bytecodes->values, value);
  pop_vm_value_stack();

  return bytecodes->values.count - 1;
}
