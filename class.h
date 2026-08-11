#ifndef CLASS_H
#define CLASS_H

// Note: this library makes liberal use of an implicit self.

// Semantic wrapper for defining a class
#define class(Cls, fields) typedef struct fields Cls;

// Defines a class method
// Expected to be preceded by the return type, and followed by a {} function
// body.
#define method(name, Self, ...) Self##_##name(Self *self, __VA_ARGS__)

// Defines a class method without any extra arguments.
#define method0(name, Self) Self##_##name(Self *self)

// Defines a class constructor
// Expected to be followed by a {} function body
#define constructor(Cls, ...) Cls Cls##_create(__VA_ARGS__)

// Declares/defines a virtual method for a class-vtable pair.
#define virtual(name, ...) (*name)(void *self, __VA_ARGS__)

// Declares/defines a virtual method for a class-vtable pair, without any
// arguments.
#define virtual0(name) (*name)(void *self)

// Casts a function to a virtual table entry.
#define vcast(name, ret_type, ...) (ret_type (*)(void *, __VA_ARGS__)) name

// Casts a function to a virtual table entry, without extra arguments.
#define vcast0(name, ret_type) (ret_type (*)(void *)) name

#define vcall(self, name, ...)                                                 \
    (*self->vtable->name)((void *)self->super, __VA_ARGS__)

#define vcall0(self, name) (*self->vtable->name)((void *)self->super)

// Defines an interface as a class-vtable pointer pair.
#define interface(Interface, vtable_def)                                       \
    typedef struct vtable_def Interface##_vtable;                              \
    typedef struct {                                                           \
        void *super;                                                           \
        Interface##_vtable *vtable;                                            \
    } Interface;

// Defines an implementation of an interface.
#define impl(Cls, Interface, vtable_impl)                                      \
    Interface##_vtable Cls##_##Interface##_vtable =                            \
        (Interface##_vtable)vtable_impl

#define dyn(Interface, Cls, value)                                             \
    (Interface) { .super = value, .vtable = &Cls##_##Interface##_vtable }

// Explicitly downcasts this instance to its superclass.
#define downcast(name) &(name->super)

// Explicitly downcasts one pointer type to another.
#define downcast_ptr(Cls, name) (Cls *)name

// Explicitly downcasts one pointer type to another, and assigns it to a new
// variable.
#define downcast_declare(Cls, name, new_name) Cls *new_name = (Cls *)name

#endif /* CLASS_H */
