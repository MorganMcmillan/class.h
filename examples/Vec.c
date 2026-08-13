#include "Vec.h"
#include <stdlib.h>
#include <string.h>

constructor(Vec) { return (Vec){malloc(0), 0, 0}; }

destructor(Vec) { free(super()->data); }

getter(Vec, data, char *) { return super()->data; }
getter(Vec, length, size_t) { return super()->length; }
default_getter(Vec, capacity, size_t);

Vec Vec_with_capacity(size_t capacity) {
    return (Vec){malloc(capacity), 0, capacity};
}

Vec method0(Vec, clone) {
    Vec result = Vec_with_capacity(self->capacity);
    result.super.length = super()->length;
    memcpy(result.super.data, super()->data, super()->length);
    return result;
}

void method(Vec, push, void *data, size_t size) {
    if (super()->length + size >= self->capacity) {
        self->capacity = (self->capacity == 0) ? size : self->capacity * 2;
        super()->data = realloc(super()->data, self->capacity);
    }
    memcpy(super()->data + super()->length, data, size);
    super()->length += size;
}

int method(Vec, pop, void *buf, size_t size) {
    if (super()->length < size) {
        return -1;
    } else {
        super()->length -= size;
        memcpy(buf, super()->data + super()->length, size);
        return 0;
    }
}

char *method(Vec, get, size_t index, size_t size) {
    index *= size;
    if (index + size > super()->length) {
        return NULL;
    } else {
        return super()->data + index;
    }
}

void method(Vec, extend, Vec *other) {
    Vec_push(self, downcast(other)->data, downcast(other)->length);
}

Vec method(Vec, concatenate, Vec *other) {
    Vec result = Vec_with_capacity(super()->length + other->super.length);
    Vec_extend(&result, self);
    Vec_extend(&result, other);
    return result;
}

void method0(Vec, clear) { super()->length = 0; }
