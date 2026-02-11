#ifndef OBJECT_H
#define OBJECT_H

#include "bytecodes.h"
#include "common.h"
#include "value.h"

typedef enum {

  OBJ_STRING,
  OBJ_FUNCTION,
  OBJ_NATIVE,

} ObjType;

#define OBJ_TYPE(value) (AS_OBJ(value)->type)
#define IS_STRING(value) is_obj_type(value, OBJ_STRING)
#define IS_FUNCTION(value) is_obj_type(value, OBJ_FUNCTION)
#define IS_NATIVE(value) is_obj_type(value, OBJ_NATIVE)

#define AS_STRING(value) ((ObjString *)AS_OBJ(value))
#define AS_FUNCTION(value) ((ObjFunction *)AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString *)AS_OBJ(value))->chars)
#define AS_NATIVE(value) (((ObjNative *)AS_OBJ(value))->function)

struct Obj {
  ObjType type;
  struct Obj *next;
  bool is_marked;
};

struct ObjString {
  Obj obj;
  size_t length;
  char *chars;
  uint32_t hash;
};

typedef struct {

  Obj obj;
  size_t arity;
  ByteCodes bytecodes;
  ObjString *name;

} ObjFunction;

typedef Value (*NativeFn)(int arg_count, Value *args);

typedef struct {

  Obj obj;
  NativeFn function;

} ObjNative;

static inline bool is_obj_type(Value value, ObjType type) {
  return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

ObjString *copy_string(const char *chars, size_t length);
ObjString *take_string(char *chars, size_t length);

ObjFunction *new_function();
ObjNative *new_native(NativeFn function);

void print_object(Value value);

#endif
