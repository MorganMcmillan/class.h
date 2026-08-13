#ifndef SLICE_H
#define SLICE_H

#include "../class.h"
#include <stddef.h>
#include <string.h>

class(Slice, {
    char *data;
    // In bytes
    size_t length;
});

constructor(Slice, char *data, size_t length);

// Use only when the slice's data is heap-allocated.
destructor(Slice);

void *method(Slice, get, size_t index, size_t size);

Slice method(Slice, subslice, size_t start_index, size_t end_index,
             size_t size);

Slice method(Slice, subslice_start, size_t start_index, size_t size);

Slice method(Slice, from_str, char *str);

#endif
