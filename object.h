#ifndef clox_object_h
#define clox_object_h

#include "common.h"
#include "value.h"
#include "chunk.h"

typedef enum {
	OBJ_FUNCTION,
	OBJ_NATIVE,
	OBJ_STRING,
	OBJ_CLOSURE,
	OBJ_UPVALUE,
	OBJ_ARRAY
} ObjType;

struct Obj {
	ObjType type;
	bool isMarked;
	struct Obj* next;
};

struct ObjString {
	Obj obj;
	int length;
	char* chars;
	uint32_t hash;
};

typedef struct {
	Obj obj;
	int upvalueCount;
	int arity;
	Chunk chunk;
	ObjString* name;
} ObjFunction;

typedef Value* (*NativeFn)(int argCount, Value* args);

typedef struct {
	Obj obj;
	NativeFn function;
} ObjNative;

typedef struct ObjUpvalue {
	Obj obj;
	Value* location;
	Value closed;
	struct ObjUpvalue* next;
} ObjUpvalue;

typedef struct {
	Obj obj;
	ObjFunction* function;
	ObjUpvalue** upvalues;
	int upvalueCount;
} ObjClosure;

typedef struct {
	Obj obj;
	int capacity;
	int count;
	Value* values;
} ObjArray;




static inline bool isObjType(Value value, ObjType type) {
	return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#define OBJ_TYPE(value)	(AS_OBJ(value)->type)

#define IS_STRING(value)	isObjType(value, OBJ_STRING)
#define IS_FUNCTION(value)  isObjType(value, OBJ_FUNCTION)
#define IS_NATIVE(value)    isObjType(value, OBJ_NATIVE)
#define IS_CLOSURE(value)   isObjType(value, OBJ_CLOSURE)
#define IS_ARRAY(value)		isObjType(value, OBJ_ARRAY)

#define AS_FUNCTION(value)     ((ObjFunction*)AS_OBJ(value))
#define AS_STRING(value)       ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value)      (((ObjString*)AS_OBJ(value))->chars)
#define AS_NATIVE(value)	   (((ObjNative*)AS_OBJ(value))->function)
#define AS_CLOSURE(value)      ((ObjClosure*)AS_OBJ(value))
#define AS_ARRAY(value)		   ((ObjArray*)AS_OBJ(value))

ObjString* copyString(const char* chars, int length);
void printObject(Value value);
ObjString* takeString(char* chars, int length);

ObjFunction* newFunction();
ObjNative* newNative(NativeFn function);
ObjClosure* newClosure(ObjFunction* function);
ObjUpvalue* newUpvalue(Value* slot);
ObjArray* newArray(uint8_t);
void writeToArray(ObjArray* array, Value value);


#endif
