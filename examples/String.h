#ifndef STRING_H
#define STRING_H

#include "../class.h"
#include "Vec.h"
#include <string.h>

class(String, { Vec super; });

constructor(String);

destructor(String);

String String_with_capacity(size_t capacity);

String method0(String, clone);

void method(String, push_char, char c);

void method(String, push_str, const char *str);

char method0(String, pop_char);

String method(String, concatenate, String *other);

size_t method0(String, len);

size_t method0(String, capacity);

int method(String, compare, String *other);

char *method(String, first_char, char c);

/// @brief Converts the String into a C string with a null terminator. This
/// string is still valid, just remember to call `pop_char` to remove it.
char *method0(String, into_c_string);

#endif // !STRING_H
