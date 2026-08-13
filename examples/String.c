#include "String.h"
#include "../class.h"
#include "Vec.h"
#include <string.h>

constructor(String) { return (String){Vec_new()}; }

destructor(String) { delete(Vec, super()); }

String String_with_capacity(size_t capacity) {
    return (String){Vec_with_capacity(capacity)};
}

String method0(String, clone) { return (String){Vec_clone(super())}; }

void method(String, push_char, char c) { Vec_push(super(), &c, sizeof(char)); }

void method(String, push_str, const char *str) {
    while (*str) {
        String_push_char(self, *str++);
    }
}

char method0(String, pop_char) {
    char c;
    if (Vec_pop(super(), &c, sizeof(char)) == 0) {
        return c;
    } else {
        return '\0';
    }
}

String method(String, concatenate, String *other) {
    return (String){Vec_concatenate(super(), downcast(other))};
}

size_t method0(String, len) { return Vec_get_length(super()); }

size_t method0(String, capacity) { return Vec_get_capacity(super()); }

int method(String, compare, String *other) {
    size_t len1 = String_len(self);
    size_t len2 = String_len(other);
    size_t min_len = (len1 < len2) ? len1 : len2;
    return memcmp(super()->super.data, downcast(other)->super.data, min_len);
}

char *method(String, first_char, char c) {
    return memchr(super()->super.data, c, String_len(self));
}

/// @brief Converts the String into a C string with a null terminator. This
/// string is still valid, just remember to call `pop_char` to remove it.
char *method0(String, into_c_string) {
    String_push_char(self, '\0');
    return super()->super.data;
}
