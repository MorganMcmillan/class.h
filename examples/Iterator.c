#include "../class.h"
#include "Slice.c"
#include "Vec.c"
#include <stddef.h>
#include <stdlib.h>

interface(Iterator, {
    void *virtual0(next);
    size_t virtual0(item_size);
});

// I would recommend to create your own `impl_Interface` macros
#define impl_Iterator(Class)                                                   \
    impl(Class, Iterator,                                                      \
         {vcast0(Class##_next, void *), vcast0(Class##_item_size, size_t)});

void *method0(Iterator, next) { return vcall0(self, next); }

size_t method0(Iterator, item_size) { return vcall0(self, item_size); }

Vec method(Iterator, collect_vec, size_t size) {
    void *item;
    Vec vec = new(Vec);
    while ((item = Iterator_next(self)) != NULL) {
        Vec_push(&vec, item, size);
    }
    return vec;
}

// Calls a function repeatedly. The function takes in the mutable state, the
// current item, and the size of an item.
void method(Iterator, reduce, void *state,
            void *(*fn)(void *, void *, size_t)) {
    void *item;
    size_t size = Iterator_item_size(self);
    while ((item = Iterator_next(self)) != NULL) {
        fn(state, item, size);
    }
}

class(Range, {
    size_t start;
    size_t end;
    size_t step;
    size_t prev;
});

constructor(Range, size_t start, size_t end, size_t step) {
    return (Range){start, end, step, 0};
}

void *method0(Range, next) {
    if (self->start >= self->end) {
        return NULL;
    }
    self->prev = self->start;
    self->start += self->step;

    return &self->prev;
}

size_t method0(Range, item_size) { return sizeof(size_t); }

impl_Iterator(Range);

// Maps an item of one type to another. Works by writing the function's output
// to a buffer of `item_size` size.
class(IterMap, {
    Iterator super;
    void (*fn)(const void *item, void *output);
    size_t item_size;
    void *output;
});

constructor(IterMap, Iterator iterator, void (*fn)(const void *, void *),
            size_t item_size) {
    return (IterMap){iterator, fn, item_size, malloc(item_size)};
}

void *method0(IterMap, next) {
    void *item = Iterator_next(super());
    if (item == NULL) {
        return NULL;
    }

    self->fn(item, self->output);
    return self->output;
}

size_t method0(IterMap, item_size) { return self->item_size; }

impl_Iterator(IterMap);

class(IterFilter, {
    Iterator super;
    int (*predicate)(void *);
});

void *method0(IterFilter, next) {
    void *item = Iterator_next(super());
    if (item == NULL) {
        return NULL;
    }

    if (self->predicate(item)) {
        return item;
    } else {
        return IterFilter_next(self);
    }
}

size_t method0(IterFilter, item_size) { return Iterator_item_size(super()); }

impl_Iterator(IterFilter);

class(Take, {
    Iterator super;
    size_t limit;
});

void *method0(Take, next) {
    void *item = Iterator_next(super());
    if (self->limit > 0) {
        self->limit--;
        return Take_next(self);
    }

    return item;
}

size_t method0(Take, item_size) { return Iterator_item_size(super()); }

impl_Iterator(Take);

class(Drop, {
    Iterator super;
    size_t limit;
});

void *method0(Drop, next) {
    void *item = Iterator_next(super());
    if (self->limit > 0) {
        self->limit--;
        return item;
    }

    return NULL;
}

size_t method0(Drop, item_size) { return Iterator_item_size(super()); }

impl_Iterator(Drop);

class(IterSlice, {
    Slice *slice;
    size_t current_index;
    size_t size;
});

constructor(IterSlice, Slice *slice, size_t size) {
    return (IterSlice){slice, 0, size};
}

void *method0(IterSlice, next) {
    void *item = Slice_get(self->slice, self->current_index, self->size);
    self->current_index++;
    return item;
}

size_t method0(IterSlice, item_size) { return self->size; }

impl_Iterator(IterSlice);

#define foreach(item, iter) while ((item = Iterator_next(iter)) != NULL)
#define foreach_c(Class, item, iter) while ((item = Class##_next(iter)) != NULL)
