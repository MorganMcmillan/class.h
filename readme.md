# class.h: header only OOP library in C

`class.h` is a single file which adds OOP features to C, such as classes, inheritence, getters/setters, interfaces, and polymorphism.

`class.h` is entirely macro-based, as it has to effectively add new syntax to C.

## Guide

To use `class.h`, simply copy it to your project and include it in your files.

```c
#include "class.h"
```

**Classes** are defined using the `class` macro. These are just a convenience wrapper around `typedef struct`.

```c
class(Person, {
    char *name;
    char *address;
    unsigned int phone_number;
});
```

**Constructors** are used to create new instances of a class, however unlike in C++, these *return* the class, rather than initializing its fields.

```c
constructor(Person, char *name, char *address, unsigned int phone_number) {
    return (Person) {
        name,
        address,
        phone_number
    };
}
```

The reason constructors return a new instance is because it helps avoid leaving fields uninitialized.

**Methods** are defined with `method` or `method0`. They way these work is they declare a function whose name is the format of the class name followed by an `_` and the method name, and define the first argument to be `Class *self` as a convenience.

Note that the "0" on `method0` means "this macro takes in zero extra arguments, which is used to avoid a syntax error. If you're using a method and it complains that it expects a list, try the "0" variant.

```c
bool method(lives_nearby, Person, char *address, unsigned int area_code) {
    return in_area(self->address, address) || has_area_code(self->phone_number, area_code);
}

void method0(print_info, Person) {
    printf("Name: %s, Address: %s, Phone Number: %d\n", self->name, self->address, self->phone_number);
}
```

While C provides little control on whether class fields are visible, you can define **getters and setters** to encourage people away from raw field access.

```c
default_getter(Person, name, char *)

default_getter(Person, address, char *)
default_setter(Person, address, char *)

// Getters can return any type
getter(Person, phone_number, char *) {
    return format_phone_number(self->phone_number);
}
```

Note: use `getter` and `setter` for forward-declaration.