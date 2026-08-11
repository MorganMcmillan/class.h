#ifndef CLASS_H
#define CLASS_H

// Note: this library makes liberal use of an implicit self.

// --------------------
// Classes
// --------------------

// Semantic wrapper for defining a class
#define class(Cls, fields) typedef struct fields Cls;

// Defines a class constructor
// Expected to be followed by a {} function body
#define constructor(Cls, ...) Cls Cls##_create(__VA_ARGS__)

// Defines a class method
// Expected to be preceded by the return type, and followed by a {} function
// body.
#define method(name, Self, ...) Self##_##name(Self *self, __VA_ARGS__)

// Defines a class method without any extra arguments.
#define method0(name, Self) Self##_##name(Self *self)

// Defines a class destructor.
// This should free all allocated memory and resources associated with the instance.
#define destructor(Cls) void Cls##_destroy(Cls *self)

// Calls the class destructor and then free.
// Intended for `malloc`'d instances.
#define delete(Cls, self) Cls##_destroy(self); free(self);

// Explicitly downcasts this instance to its superclass.
#define downcast(name) &(name->super)

// Explicitly downcasts one pointer type to another.
#define downcast_ptr(Cls, name) (Cls *)name

// Explicitly downcasts one pointer type to another, and assigns it to a new
// variable.
#define downcast_declare(Cls, name, new_name) Cls *new_name = (Cls *)name

// --------------------
// Getters and setters
// --------------------

// Defines a getter method.
#define getter(Cls, name, type) type Cls##_get_##name(Cls *self)

// Defines a setter method.
#define setter(Cls, name, type) void Cls##_set_##name(Cls *self, type value)

// Defines a default getter method.
// Do not add a semicolon, this is a function definition.
#define default_getter(Cls, name, type) type Cls##_get_##name(Cls *self) { return self->name; }

// Defines a default setter method.
// Do not add a semicolon, this is a function definition.
#define default_setter(Cls, name, type) void Cls##_set_##name(Cls *self, type value) { self->name = value; }

// --------------------
// Interfaces and virtual methods
// --------------------

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

// Creates an instance of an interface.
// This is similar to Rust's `dyn` keyword, which creates a trait object from a concrete type.
#define dyn(Interface, Cls, value)                                             \
    (Interface) { .super = value, .vtable = &Cls##_##Interface##_vtable }

#endif /* CLASS_H */
