#include "Iterator.h"
#include "../class.h"
#include "Slice.c"
#include "Vec.c"
#include <stddef.h>
#include <stdlib.h>

void *method0(Iterator, next) { return vcall0(self, next); }

size_t method0(Iterator, item_size) { return vcall0(self, item_size); }

Vec method(Iterator, collect_vec, size_t size) {
    void *item;
    Vec vec = Vec_new();
    while ((item = Iterator_next(self)) != NULL) {
        Vec_push(&vec, item, size);
    }
    return vec;
}

void method(Iterator, foreach, void (*fn)(void *)) {
    void *item;
    while ((item = Iterator_next(self)) != NULL) {
        fn(item);
    }
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

constructor(IterFilter, Iterator iterator, int (*predicate)(void *)) {
    return (IterFilter){iterator, predicate};
}

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

constructor(Take, Iterator iterator, size_t limit) {
    return (Take){iterator, limit};
}

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

constructor(Drop, Iterator iterator, size_t limit) {
    return (Drop){iterator, limit};
}

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

constructor(SliceIter, Slice *slice, size_t size) {
    return (SliceIter){slice, 0, size};
}

void *method0(SliceIter, next) {
    void *item = Slice_get(self->slice, self->current_index, self->size);
    self->current_index++;
    return item;
}

size_t method0(SliceIter, item_size) { return self->size; }

impl_Iterator(SliceIter);
