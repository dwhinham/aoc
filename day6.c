// ----------------------------------------------------------------------------
// Advent of Code 2022 - Day 6
// Dale Whinham
//
// $ gcc day6.c -Wall -Wextra -Wpedantic -Werror -o day6
// $ ./day6
// ----------------------------------------------------------------------------

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

#define INPUT_FILE "day6_input.txt"

int main()
{
    size_t size;
    char* message = read_file(INPUT_FILE, &size);

    char* p = message;
    while (p < message + size - MARKER_LEN)
    {
        for (uint8_t i = 0; i < MARKER_LEN; ++i)
        {
            for (uint8_t j = i + 1; j < MARKER_LEN; ++j)
            {
                if (p[i] == p[j])
                    goto next;
            }
        }

        // Found unique marker
        break;

    next:
        ++p;
    }

    size_t offset = p - message + MARKER_LEN;
    printf("Marker offset: %zu\n", offset);

    free(message);
    return EXIT_SUCCESS;
}
