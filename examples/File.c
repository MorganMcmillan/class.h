#include "../class.h"
#include <stdlib.h>
#include <stdio.h>
#include "Vec.c"
#include "String.c"

class(File, {
    FILE *file;
});

constructor(File, const char *filename, const char *mode) {
    return (File) {fopen(filename, mode)};
}

destructor(File) {
    fclose(self->file);
}

void method(File, write, char *data, size_t length) {
    fwrite(data, sizeof(char), length, self->file);
}

void method(File, write_vec, Vec *vec) {
    File_write(self, vec->data, vec->length);
}

void method(File, write_string, String *str) {
    File_write_vec(self, downcast(str));
}

size_t method(File, read, char *buffer, size_t length) {
    return fread(buffer, 1, length, self->file);
}

String method0(File, read_to_string) {
    // Get length of file
    fseek(self->file, 0, SEEK_END);
    size_t length = ftell(self->file);
    fseek(self->file, 0, SEEK_SET);

    String result = String_with_capacity(length);

    char buffer[length];
    File_read(self, buffer, length);
    Vec_push(downcast(&result), buffer, length);

    return result;
}

int method(File, seek, long offset, int whence) {
    return fseek(self->file, offset, whence);
}

void method0(File, flush) {
    fflush(self->file);
}