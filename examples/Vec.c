#ifndef VEC_C
#define VEC_C

#include "../class.h"
#include <stdlib.h>
#include <string.h>

/// @brief A vector class that can hold arbitrary data. It is a dynamic array
/// that can grow and shrink as needed. It is generic in that it looks at data
/// as only bytes, and does not care about the type of data it holds.
class(Vec, {
    char *data;
    // In bytes
    size_t length;
    // In bytes
    size_t capacity;
});

constructor(Vec) { return (Vec){malloc(0), 0, 0}; }

destructor(Vec) { free(self->data); }

default_getter(Vec, data, char *);
default_getter(Vec, length, size_t);
default_getter(Vec, capacity, size_t);

Vec Vec_with_capacity(size_t capacity) {
    return (Vec){malloc(capacity), 0, capacity};
}

Vec method0(Vec, clone) {
    Vec result = Vec_with_capacity(self->capacity);
    result.length = self->length;
    memcpy(result.data, self->data, self->length);
    return result;
}

/// @brief Pushes a value of `size` bytes from `data` onto the end of the
/// vector.
/// @param self
/// @param data
/// @param size
void method(Vec, push, void *data, size_t size) {
    if (self->length + size >= self->capacity) {
        self->capacity = (self->capacity == 0) ? size : self->capacity * 2;
        self->data = realloc(self->data, self->capacity);
    }
    memcpy(self->data + self->length, data, size);
    self->length += size;
}

/// @brief Pops a value of `size` bytes from the end and into `buf`. Returns 0
/// on success, -1 if the vector is too small to pop the value.
/// @param self
/// @param buf
/// @param size
/// @return error code
int method(Vec, pop, void *buf, size_t size) {
    if (self->length < size) {
        return -1;
    } else {
        self->length -= size;
        memcpy(buf, self->data + self->length, size);
        return 0;
    }
}

/// @brief Gets a pointer to the value at `index` of `size` bytes. The index is
/// the same as an array index
/// @param self
/// @param index
/// @param size
/// @return the memory pointed to by index, or NULL if the index is out of
/// bounds.
char *method(Vec, get, size_t index, size_t size) {
    index *= size;
    if (index + size > self->length) {
        return NULL;
    } else {
        return self->data + index;
    }
}

void method(Vec, extend, Vec *other) {
    Vec_push(self, other->data, other->length);
}

Vec method(Vec, concatenate, Vec *other) {
    Vec result = Vec_with_capacity(self->length + other->length);
    Vec_extend(&result, self);
    Vec_extend(&result, other);
    return result;
}

void method0(Vec, clear) { self->length = 0; }

#define Vec_foreach(Type, it, vec)                                             \
    for (Type *it = vec->data; it < vec->data + (vec->length * sizeof(Type));  \
         it++)

#endif
