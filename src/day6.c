// ----------------------------------------------------------------------------
// Advent of Code 2022 - Day 6
// Dale Whinham
//
// $ gcc day6.c -Wall -Wextra -Wpedantic -Werror -o day6
// $ ./day6
// ----------------------------------------------------------------------------

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

#define PART_TWO

#ifdef PART_TWO
#define MARKER_LEN 14
#else
#define MARKER_LEN 4
#endif

#define INPUT_FILE "input/day6.txt"

static inline bool all_unique(const char* string, size_t len)
{
    for (size_t i = 0; i < len; ++i)
    {
        for (size_t j = i + 1; j < len; ++j)
        {
            if (string[i] == string[j])
                return false;
        }
    }

    return true;
}

int main()
{
    size_t size, offset = 0;
    char* message = read_file(INPUT_FILE, &size);

    // Find unique marker
    while (!all_unique(&message[offset], MARKER_LEN))
        ++offset;

    offset += MARKER_LEN;
    printf("Marker offset: %zu\n", offset);

    free(message);
    return EXIT_SUCCESS;
}
