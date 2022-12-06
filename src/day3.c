// ----------------------------------------------------------------------------
// Advent of Code 2022 - Day 3
// Dale Whinham
//
// $ gcc day3.c -Wall -Wextra -Wpedantic -Werror -o day3
// $ ./day3
// ----------------------------------------------------------------------------

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "input/day3.txt"

#define MAX_ITEMS (26 * 2) // a-z, A-Z
#define ITEM_BITPOS(CHAR) (CHAR >= 'a' ? CHAR - 'a' : CHAR - 'A' + 26)
#define ITEM_BITMASK(CHAR) (1L << ITEM_BITPOS(CHAR))
#define ELVES_PER_GROUP 3

#define PART_TWO

int main()
{
    FILE* f = fopen(INPUT_FILE, "r");
    if (f == NULL)
        return EXIT_FAILURE;

    uint32_t priority_sum = 0;
#ifdef PART_TWO
    uint64_t group_items = -1;
    uint32_t group_bag = 0;
#endif

    char buffer[512];
    while (fgets(buffer, sizeof(buffer), f))
    {
        // Subtract 1 from string length to ignore '\n' character
        size_t len = strlen(buffer) - 1;
#ifdef PART_TWO
        uint64_t this_bag = 0;
        for (size_t i = 0; i < len; ++i)
            this_bag |= ITEM_BITMASK(buffer[i]);

        // Bitwise AND to find common items
        group_items &= this_bag;

        // Done 3 bags
        if (++group_bag == ELVES_PER_GROUP)
        {
            // Loop over each bit and compute priority sum
            for (uint8_t i = 0; i < MAX_ITEMS; ++i)
            {
                if ((group_items >> i) & 1)
                    priority_sum += i + 1;
            }

            // Next group
            group_bag = 0;
            group_items = -1;
        }
#else
        uint64_t compartments[2] = { 0 };

        // For each character, convert to bit position, separate compartments
        for (size_t i = 0; i < len; ++i)
        {
            if (i < len / 2)
                compartments[0] |= ITEM_BITMASK(buffer[i]);
            else
                compartments[1]  |= ITEM_BITMASK(buffer[i]);
        }

        // Bitwise AND to find common items common to both compartments
        uint64_t both_compartments = compartments[0] & compartments[1];

        // Loop over each bit and compute priority sum
        for (uint8_t i = 0; i < MAX_ITEMS; ++i)
        {
            if ((both_compartments >> i) & 1)
                priority_sum += i + 1;
        }
#endif
    }

    printf("Priority sum: %d\n", priority_sum);
    fclose(f);
    return EXIT_SUCCESS;
}
