#include <stdio.h>
#include <string.h>

#include "common.h"
#include "memory.h"
#include "object.h"
#include "value.h"

void init_values(Values *values) {
  values->count = 0;
  values->capacity = 0;
  values->value_array = NULL;
}

void write_values(Values *values, Value value) {

  if (values->capacity < values->count + 1) {

    size_t old_capacity = values->capacity;
    values->capacity = GROW_CAPACITY(old_capacity);
    values->value_array =
        GROW_ARRAY(Value, values->value_array, old_capacity, values->capacity);
  }

  values->value_array[values->count] = value;
  values->count++;
}

void free_values(Values *values) {

  FREE_ARRAY(Value, values->value_array, values->capacity);
  init_values(values);
}

void print_value(Value value) {
  if (value.type == VAL_NUMBER) {
    printf("%g", AS_NUMBER(value));
  }

  else if (value.type == VAL_BOOL) {
    printf("%s", AS_BOOL(value) ? "true" : "false");
  }

  else if (value.type == VAL_NIL) {
    printf("nil");
  }

  else if (value.type == VAL_OBJ) {
    print_object(value);
  }
}

bool values_equal(Value a, Value b) {

  if (a.type != b.type) {
    return false;
  }

  switch (a.type) {
  case VAL_BOOL: {
    return AS_BOOL(a) == AS_BOOL(b);
  }
  case VAL_NIL: {
    return true;
  }
  case VAL_NUMBER: {
    return AS_NUMBER(a) == AS_NUMBER(b);
  }
  case VAL_OBJ: {
    return AS_OBJ(a) == AS_OBJ(b);
  }
  default: {
    return false;
  }
  }
}
