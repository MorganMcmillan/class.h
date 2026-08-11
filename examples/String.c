#include "../class.h"
#include "Vec.c"
#include <string.h>

class(String, {
    Vec super;
});

constructor(String) {
    return (String) {create(Vec)};
}

destructor(String) {
    delete(Vec, downcast(self));
}

String String_with_capacity(size_t capacity) {
    return (String) {Vec_with_capacity(capacity)};
}

String method0(String, clone) {
    return (String) {Vec_clone(downcast(self))};
}

void method(String, push_char, char c) {
    Vec_push(downcast(self), &c, sizeof(char));
}

void method(String, push_str, const char *str) {
    while (*str) {
        String_push_char(self, *str++);
    }
}

char method0(String, pop_char) {
    char c;
    if (Vec_pop(downcast(self), &c, sizeof(char)) == 0) {
        return c;
    } else {
        return '\0';
    }
}

String method(String, concatenate, String *other) {
    Vec_concatenate(downcast(self), downcast(other));
}

size_t method0(String, len) {
    return Vec_len(downcast(self), sizeof(char));
}

size_t method0(String, capacity) {
    return Vec_capacity(downcast(self), sizeof(char));
}

int method(String, compare, String *other) {
    size_t len1 = String_len(self);
    size_t len2 = String_len(other);
    size_t min_len = (len1 < len2) ? len1 : len2;
    return memcmp(self->super.data, other->super.data, min_len);
}

char *method(String, first_substring, const char *str) {
    return strnstr(self->super.data, str, String_len(self));
}

char *method(String, first_char, char c) {
    return memchr(self->super.data, c, String_len(self));
}

/// @brief Converts the String into a C string with a null terminator. This string is still valid, just remember to call `pop_char` to remove it.
char *method0(String, into_c_string) {
    String_push_char(self, '\0');
    return self->super.data;
}