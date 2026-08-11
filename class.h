#ifndef CLASS_H
#define CLASS_H

// Note: this library makes liberal use of an implicit self.

// --------------------
// Classes
// --------------------

// Semantic wrapper for defining a class

#define class(Class, fields) typedef struct fields Class;

// Defines a class constructor
// Expected to be followed by a {} function body

#define constructor(Class, ...) Class Class##_create(__VA_ARGS__)

// Defines a class destructor.
// This should free all allocated memory and resources associated with the instance.

#define destructor(Class) void Class##_destroy(Class *self)

// Creates a new instance of `Class` from a constructor on the stack.

#define create(Class, ...) Class##_create(__VA_ARGS__)

// Creates a new instance of `Class` from a constructor on the stack,
// without any extra arguments.

#define create0(Class) Class##_create()

// Calls the class destructor.
// Also ensures that the value cannot be used after.

#define delete(Class, self) Class##_destroy(self); (void)self

// Calls the class destructor and then free.
// Also ensures that the value cannot be used after.
// Used with classes that are allocated on the heap.

#define delete_and_free(Class, self) Class##_destroy(self); free(self); (void)self

// Defines a class method
// Expected to be preceded by the return type, and followed by a {} function
// body.

#define method(Class, name, ...) Class##_##name(Class *self, __VA_ARGS__)

// Defines a class method without any extra arguments.

#define method0(Class, name) Class##_##name(Class *self)

// Explicitly downcasts this instance to its superclass.

#define downcast(name) &((name)->super)

// Explicitly downcasts this instance to its nth superclass.
// For use with multiple inheritance, where superclasses are named `super1`, `super2`, etc.

#define downcast_n(name, n) &(name->super##n)

// Explicitly downcasts one pointer type to another.

#define downcast_ptr(Class, name) (Class *)name

// Explicitly downcasts one pointer type to another, and assigns it to a new
// variable.

#define downcast_declare(Class, name, new_name) Class *new_name = (Class *)name

// --------------------
// Getters and setters
// --------------------

// Defines a getter method.

#define getter(Class, name, type) type Class##_get_##name(Class *self)

// Defines a setter method.

#define setter(Class, name, type) void Class##_set_##name(Class *self, type value)

// Defines a default getter method.
// Do not add a semicolon, this is a function definition.

#define default_getter(Class, name, type) type Class##_get_##name(Class *self) { return self->name; }

// Defines a default setter method.
// Do not add a semicolon, this is a function definition.

#define default_setter(Class, name, type) void Class##_set_##name(Class *self, type value) { self->name = value; }

// --------------------
// Interfaces and virtual methods
// --------------------

// Defines an interface as a class-vtable pointer pair.

#define interface(Interface, vtable_def)                                       \
    typedef struct vtable_def Interface##_vtable;                              \
    typedef struct {                                                           \
        void *super;                                                           \
        Interface##_vtable *vtable;                                            \
    } Interface;

// Declares/defines a virtual method for a class-vtable pair.

#define virtual(name, ...) (*name)(void *self, __VA_ARGS__)

// Declares/defines a virtual method for a class-vtable pair, without any
// arguments.

#define virtual0(name) (*name)(void *self)

// Casts a function to a virtual table entry.

#define vcast(name, ret_type, ...) (ret_type (*)(void *, __VA_ARGS__)) name

// Casts a function to a virtual table entry, without extra arguments.

#define vcast0(name, ret_type) (ret_type (*)(void *)) name

// Calls a virtual method.
// This is meant only to be called inside an interface method.

#define vcall(self, name, ...)                                                 \
    (*self->vtable->name)((void *)self->super, __VA_ARGS__)

// Calls a virtual method without any extra arguments.
// This is meant only to be called inside an interface method.

#define vcall0(self, name) (*self->vtable->name)((void *)self->super)

// Gets a virtual constant from the vtable.

#define vget(self, name) self->vtable->name

// Defines an implementation of an interface.

#define impl(Class, Interface, vtable_impl)                                      \
    Interface##_vtable Class##_##Interface##_vtable =                            \
        (Interface##_vtable)vtable_impl

// Creates an instance of an interface.
// This is similar to Rust's `dyn` keyword, which creates a trait object from a concrete type.

#define dyn(Interface, Class, value)                                             \
    (Interface) { .super = value, .vtable = &Class##_##Interface##_vtable }

#endif /* CLASS_H */
