#ifndef VALUE_H
#define VALUE_H

#include "common.h"

typedef struct Obj Obj;
typedef struct ObjString ObjString;

typedef enum {

  VAL_BOOL,
  VAL_NIL,
  VAL_NUMBER,
  VAL_OBJ

} ValueType;

typedef struct {

  ValueType type;

  union {
    bool boolean;
    double number;
    Obj *object;
  } as;

} Value;

#define AS_BOOL(value) ((value).as.boolean)
#define AS_NUMBER(value) ((value).as.number)
#define AS_OBJ(value) ((value).as.object)

#define BOOL_VAL(value) ((Value){VAL_BOOL, {.boolean = value}})
#define NIL_VAL ((Value){VAL_NIL, {.number = 0}})
#define NUMBER_VAL(value) ((Value){VAL_NUMBER, {.number = value}})
#define OBJ_VAL(value) ((Value){VAL_OBJ, {.object = (Obj *)value}})

#define IS_BOOL(value) (value.type == VAL_BOOL)
#define IS_NIL(value) (value.type == VAL_NIL)
#define IS_NUMBER(value) (value.type == VAL_NUMBER)
#define IS_OBJ(value) (value.type == VAL_OBJ)

typedef struct {

  size_t capacity;
  size_t count;
  Value *value_array;

} Values;

void init_values(Values *values);
void write_values(Values *values, Value value);
void free_values(Values *values);
void print_value(Value value);
bool values_equal(Value a, Value b);

#endif
