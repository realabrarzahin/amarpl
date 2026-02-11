#include "object.h"
#include "bytecodes.h"
#include "memory.h"
#include "table.h"
#include "value.h"
#include "vm.h"

#include <stdio.h>
#include <string.h>

#define ALLOCATE_OBJ(type, object_type)                                        \
  (type *)allocate_object(sizeof(type), object_type)

static Obj *allocate_object(size_t size, ObjType type) {

  Obj *object = (Obj *)reallocate(NULL, 0, size);

  object->type = type;
  object->is_marked = false;

  object->next = vm.objects;
  vm.objects = object;

#ifdef DEBUG_LOG_GC
  printf("%p allocate %zu for %d\n", (void *)object, size, type);
#endif

  return object;
}

static ObjString *allocate_string(char *chars, size_t length, uint32_t hash) {

  ObjString *string = ALLOCATE_OBJ(ObjString, OBJ_STRING);

  string->length = length;
  string->chars = chars;
  string->hash = hash;

  push_vm_value_stack(OBJ_VAL(string));
  table_set(&vm.strings, string, NIL_VAL);
  pop_vm_value_stack();

  return string;
}

static uint32_t hash_string(const char *key, size_t length) {

  uint32_t hash = 2166136261u;

  for (size_t i = 0; i < length; i++) {
    hash ^= (uint8_t)key[i];
    hash *= 16777619;
  }

  return hash;
}

ObjString *copy_string(const char *chars, size_t length) {

  uint32_t hash = hash_string(chars, length);

  ObjString *interned = table_find_string(&vm.strings, chars, length, hash);

  if (interned != NULL) {
    return interned;
  }

  char *heap_chars = ALLOCATE(char, length + 1);

  memcpy(heap_chars, chars, length);
  heap_chars[length] = '\0';

  return allocate_string(heap_chars, length, hash);
}

ObjString *take_string(char *chars, size_t length) {
  uint32_t hash = hash_string(chars, length);

  ObjString *interned = table_find_string(&vm.strings, chars, length, hash);

  if (interned != NULL) {
    FREE_ARRAY(char, chars, length + 1);
    return interned;
  }

  return allocate_string(chars, length, hash);
}

static void print_function(ObjFunction *function) {

  if (function->name) {
    printf("fn %s", function->name->chars);
  }

  else {
    printf("script");
  }
}

void print_object(Value value) {

  switch (OBJ_TYPE(value)) {

  case OBJ_STRING: {
    printf("%s", AS_CSTRING(value));
    break;
  }

  case OBJ_NATIVE: {
    printf("native fn");
    break;
  }

  case OBJ_FUNCTION: {
    print_function(AS_FUNCTION(value));
    break;
  }
  }
}

ObjFunction *new_function() {

  ObjFunction *function = ALLOCATE_OBJ(ObjFunction, OBJ_FUNCTION);

  function->arity = 0;
  function->name = NULL;
  init_bytecodes(&function->bytecodes);

  return function;
}

ObjNative *new_native(NativeFn function) {

  ObjNative *native = ALLOCATE_OBJ(ObjNative, OBJ_NATIVE);
  native->function = function;

  return native;
}
