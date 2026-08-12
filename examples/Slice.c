#ifndef SLICE_C
#define SLICE_C

#include "../class.h"
#include "Vec.c"
#include <stddef.h>
#include <string.h>

class(Slice, {
    char *data;
    // In bytes
    size_t length;
});

constructor(Slice, char *data, size_t length) { return (Slice){data, length}; }

void *method(Slice, get, size_t index, size_t size) {
    size_t offset = index * size;
    if (offset >= self->length) {
        return NULL;
    }

    return self->data + offset;
}

Slice method(Slice, subslice, size_t start_index, size_t end_index,
             size_t size) {
    size_t start_offset = start_index * size;
    size_t end_offset = end_index * size;

    if (start_offset >= self->length || end_offset >= self->length ||
        end_offset < start_offset) {
        return new(Slice, NULL, 0);
    }

    size_t length = end_offset - start_offset;
    return new(Slice, self->data + start_offset, length);
}

Slice method(Slice, subslice_start, size_t start_index, size_t size) {
    size_t start_offset = start_index * size;
    if (start_offset >= self->length) {
        return new(Slice, NULL, 0);
    }

    size_t length = self->length - start_offset;
    return new(Slice, self->data + start_offset, length);
}

Slice method(Slice, from_str, char *str) {
    return new(Slice, str, strlen(str));
}

Vec method0(Slice, to_vec) {
    Vec result = Vec_with_capacity(self->length);
    Vec_push(&result, self->data, self->length);
    return result;
}

#endif
