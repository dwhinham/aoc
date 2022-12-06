#include <stdio.h>
#include <stdlib.h>

#ifndef UTIL_H
#define UTIL_H

#define ARRAY_SIZE(X) (sizeof(X) / sizeof(*X))

static long file_size(FILE* f)
{
    long current_pos = ftell(f);
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, current_pos, SEEK_SET);
    return size;
}

static char* read_file(const char* path, size_t* out_size)
{
    FILE* f = fopen(path, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Couldn't open %s for reading\n", path);
        return NULL;
    }

    *out_size = file_size(f);
    char* text = malloc(*out_size);
    fread(text, 1, *out_size, f);
    fclose(f);

    return text;
}

#endif
