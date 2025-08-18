#ifndef XELTU_VALUE_H
#define XELTU_VALUE_H

typedef enum { VALUE_CONST, VALUE_VAR } ValueType;
typedef struct Value {
	ValueType type;

	bool defined;

	union {
		float constant;
		char* name;
	};
} Value;


#endif
