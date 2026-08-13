#ifndef ITERATOR_H
#define ITERATOR_H

#include "../class.h"
#include "Slice.h"
#include "Vec.h"
#include <stddef.h>
#include <stdlib.h>

interface(Iterator, {
    void *virtual0(next);
    size_t virtual0(item_size);
});

// I would recommend to create your own `impl_Interface` macros.
// Usage of these belongs in the header, since the vtable needs to be made
// global
#define impl_Iterator(Class)                                                   \
    impl(Class, Iterator,                                                      \
         {vcast0(Class##_next, void *), vcast0(Class##_item_size, size_t)});

void *method0(Iterator, next);

size_t method0(Iterator, item_size);

Vec method(Iterator, collect_vec, size_t size);

// Calls a function repeatedly. The function takes in the mutable state, the
// current item, and the size of an item.
void method(Iterator, reduce, void *state, void *(*fn)(void *, void *, size_t));

class(Range, {
    size_t start;
    size_t end;
    size_t step;
    size_t prev;
});

constructor(Range, size_t start, size_t end, size_t step);

void *method0(Range, next);

size_t method0(Range, item_size);

declare_impl(Range, Iterator);

// Maps an item of one type to another. Works by writing the function's output
// to a buffer of `item_size` size.
class(IterMap, {
    Iterator super;
    void (*fn)(const void *item, void *output);
    size_t item_size;
    void *output;
});

constructor(IterMap, Iterator iterator, void (*fn)(const void *, void *),
            size_t item_size);

void *method0(IterMap, next);

size_t method0(IterMap, item_size);

declare_impl(IterMap, Iterator);

class(IterFilter, {
    Iterator super;
    int (*predicate)(void *);
});

constructor(IterFilter, Iterator iterator, int (*predicate)(void *));

void *method0(IterFilter, next);

size_t method0(IterFilter, item_size);

declare_impl(IterFilter, Iterator);

class(Take, {
    Iterator super;
    size_t limit;
});

constructor(Take, Iterator iterator, size_t limit);

void *method0(Take, next);

size_t method0(Take, item_size);

declare_impl(Take, Iterator);

class(Drop, {
    Iterator super;
    size_t limit;
});

constructor(Drop, Iterator iterator, size_t limit);

void *method0(Drop, next);

size_t method0(Drop, item_size);

declare_impl(Drop, Iterator);

class(SliceIter, {
    Slice *slice;
    size_t current_index;
    size_t size;
});

constructor(SliceIter, Slice *slice, size_t size);

void *method0(SliceIter, next);

size_t method0(SliceIter, item_size);

declare_impl(SliceIter, Iterator);

#define foreach(item, iter) while ((item = Iterator_next(iter)) != NULL)
#define foreach_c(Class, item, iter) while ((item = Class##_next(iter)) != NULL)

void method(Iterator, foreach, void (*fn)(void *));

#endif
