// ----------------------------------------------------------------------------
// Advent of Code 2022 - Day 1
// Dale Whinham
//
// $ gcc day1.c -o day1
// $ ./day1
// ----------------------------------------------------------------------------

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

#define INPUT_FILE "day1_input.txt"

typedef struct
{
    uint32_t index;
    uint32_t calories;
} elf_t;

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    size_t size;
    char* text = read_file(INPUT_FILE, &size);
    if (text == NULL)
        return EXIT_FAILURE;

    elf_t elves[3] = {0};
    uint32_t elf_current = 1;
    uint32_t cal_current = 0;
    char* p = text, *p2 = text;

    while (p < text + size)
    {
        // Find next newline and terminate it
        while (p2 < text + size && *p2 != '\n')
            ++p2;
        *p2++ = '\0';

        // Convert string to integer
        cal_current += atoi(p);

        // Two consecutive newlines = next elf
        if (*p2 == '\n')
        {
            for (size_t i = 0; i < ARRAY_SIZE(elves); ++i)
            {
                if (cal_current > elves[i].calories)
                {
                    elves[i].index = elf_current;
                    elves[i].calories = cal_current;
                    break;
                }
            }

            cal_current = 0;
            ++p2;
            ++elf_current;
        }

        p = p2;
    }

    puts
    (
        "Top 3 elves:\n"
        "============"
    );

    uint32_t total_calories = 0;
    for (size_t i = 0; i < ARRAY_SIZE(elves); ++i)
    {
        printf("#%3d: %d kcal\n", elves[i].index, elves[i].calories);
        total_calories += elves[i].calories;
    }

    printf
    (
        "------------\n"
        "Total: %d kcal\n", total_calories
    );

    free(text);
    return EXIT_SUCCESS;
}
