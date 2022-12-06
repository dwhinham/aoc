// ----------------------------------------------------------------------------
// Advent of Code 2022 - Day 4
// Dale Whinham
//
// $ gcc day4.c -Wall -Wextra -Wpedantic -Werror -o day4
// $ ./day4
// ----------------------------------------------------------------------------

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "day4_input.txt"

#define PART_TWO

int main()
{
    FILE* f = fopen(INPUT_FILE, "r");
    if (f == NULL)
        return EXIT_FAILURE;

    uint32_t overlaps = 0;
    uint32_t a_min, a_max, b_min, b_max;
    while (fscanf(f, "%u-%u,%u-%u\n", &a_min, &a_max, &b_min, &b_max) == 4)
    {
#ifdef PART_TWO
        if (a_min <= b_max && b_min <= a_max)
#else
        if ((a_min >= b_min && a_max <= b_max) ||
            (b_min >= a_min && b_max <= a_max))
#endif
            ++overlaps;
    }

    printf("Overlapping ranges: %d\n", overlaps);

    fclose(f);
    return EXIT_SUCCESS;
}
