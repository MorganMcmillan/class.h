#ifndef FILE_H
#define FILE_H

#include "../class.h"
#include "Slice.h"
#include "String.h"
#include <stdio.h>
#include <stdlib.h>

class(File, { FILE *file; });

constructor(File, const char *filename, const char *mode);

destructor(File);

void method(File, write, char *data, size_t length);

void method(File, write_slice, Slice *slice);

void method(File, write_string, String *str);

size_t method(File, read, char *buffer, size_t length);

String method0(File, read_to_string);

int method(File, seek, long offset, int whence);

void method0(File, flush);

#endif