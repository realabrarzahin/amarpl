#ifndef VM_H
#define VM_H

#include "object.h"
#include "table.h"

#define FRAMES_MAX 256
#define STACK_MAX (FRAMES_MAX * UINT8_COUNT)

#include "bytecodes.h"

typedef struct {

  ObjFunction *function;
  uint8_t *ip;
  Value *slots;

} CallFrame;

typedef struct {

  CallFrame frames[FRAMES_MAX];
  size_t frame_count;

  Value stack[STACK_MAX];
  Value *stack_top;

  Obj *objects;
  int gray_count;
  int gray_capacity;
  Obj **gray_stack;

  Table strings;
  Table globals;

  size_t bytes_allocated;
  size_t next_gc;

} VM;

extern VM vm;

typedef enum {

  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR

} InterpretResult;

void init_vm();
void free_vm();

void push_vm_value_stack(Value value);
Value pop_vm_value_stack();

InterpretResult interpret(const char *source);

#endif
