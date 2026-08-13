#include "Slice.h"
#include "../class.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

constructor(Slice, char *data, size_t length) { return (Slice){data, length}; }

// Use only when the slice's data is heap-allocated.
destructor(Slice) { free(self->data); }

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
