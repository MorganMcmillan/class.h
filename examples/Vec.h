#ifndef VEC_H
#define VEC_H

#include "../class.h"
#include "Slice.h"
#include <stdlib.h>
#include <string.h>

/// @brief A vector class that can hold arbitrary data. It is a dynamic array
/// that can grow and shrink as needed. It is generic in that it looks at data
/// as only bytes, and does not care about the type of data it holds.
class(Vec, {
    Slice super;
    // In bytes
    size_t capacity;
});

constructor(Vec);

destructor(Vec);

getter(Vec, data, char *);
getter(Vec, length, size_t);
getter(Vec, capacity, size_t);

Vec Vec_with_capacity(size_t capacity);

Vec method0(Vec, clone);

/// @brief Pushes a value of `size` bytes from `data` onto the end of the
/// vector.
/// @param self
/// @param data
/// @param size
void method(Vec, push, void *data, size_t size);

/// @brief Pops a value of `size` bytes from the end and into `buf`. Returns 0
/// on success, -1 if the vector is too small to pop the value.
/// @param self
/// @param buf
/// @param size
/// @return error code
int method(Vec, pop, void *buf, size_t size);

/// @brief Gets a pointer to the value at `index` of `size` bytes. The index is
/// the same as an array index
/// @param self
/// @param index
/// @param size
/// @return the memory pointed to by index, or NULL if the index is out of
/// bounds.
char *method(Vec, get, size_t index, size_t size);

void method(Vec, extend, Vec *other);

Vec method(Vec, concatenate, Vec *other);

void method0(Vec, clear);

#define Vec_foreach(Type, it, vec)                                             \
    for (Type *it = vec->data; it < vec->data + (vec->length * sizeof(Type));  \
         it++)

#endif
