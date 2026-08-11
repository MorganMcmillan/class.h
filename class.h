#ifndef CLASS_H
#define CLASS_H

#define class(Cls, fields) typedef struct fields Cls;

// Defines a class method
// Expected to be preceded by the return type, and followed by a {} function
// body
#define method(Cls, name, ...) Cls##_##name(self *Cls, __VA_ARGS__)

// Defines a class constructor
// Expected to be followed by a {} function body
#define constructor(Cls, ...) Cls Cls##_create(__VA_ARGS__)

#define ivirtual(name, Interface, ...) (*name)(Interface * self, ...)

#define virtual(name, ...) (*name)(void *self, ...)

#define vcall(self, name, ...) (*self->vtable->name)(self, __VA_ARGS__)

#define impl(Cls, Interface, vtable)                                           \
  Interface Cls##_##Interface##_vtable = vtable

// Creates a class-vtable pair to represent a polymorphic instance of an
// interface.
#define def_dyn(Cls, Interface)                                                \
  typedef struct {                                                             \
    Cls value;                                                                 \
    Interface vtable                                                           \
  } Cls##_##Interface

#define dyn(Cls, Interface, name, val)                                         \
  Cls##_##Interface name = (Cls##_##Interface) {                               \
    .value = val, .vtable = Cls##_##Interface##_vtable                         \
  }

// Explicitly downcasts one pointer type to another
#define downcast(name, Cls, new_name) Cls *new_name = (Cls *)name

#endif /* CLASS_H */
