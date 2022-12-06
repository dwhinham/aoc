// ----------------------------------------------------------------------------
// Advent of Code 2022 - Day 5
// Dale Whinham
//
// $ gcc day5.c -Wall -Wextra -Wpedantic -Werror -o day5
// $ ./day5
// ----------------------------------------------------------------------------

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "day5_input.txt"

#define PART_TWO

#define MAX_STACKS 32
#define MAX_STACK_HEIGHT 64

typedef struct
{
    size_t num_crates;
    char crates[MAX_STACK_HEIGHT];
} stack_t;

static size_t num_stacks = 0;
static stack_t stacks[MAX_STACKS];

static inline void reverse_array(char* arr, size_t len)
{
    char* p1 = arr;
    char* p2 = arr + len - 1;

    while (p1 < p2)
    {
        char tmp = *p1;
        *p1++ = *p2;
        *p2-- = tmp;
    }
}

static inline void move_crates(size_t count, size_t src_index, size_t dst_index)
{
    stack_t* src = &stacks[src_index];
    stack_t* dst = &stacks[dst_index];
    assert(dst->num_crates + count <= MAX_STACK_HEIGHT);

#ifdef PART_TWO
    memmove(&dst->crates[dst->num_crates], &src->crates[src->num_crates - count], count * sizeof(char));
    src->num_crates -= count;
    dst->num_crates += count;
#else
    for (size_t i = 0; i < count; ++i)
        dst->crates[dst->num_crates++] = src->crates[--src->num_crates];
#endif
}

int main()
{
    FILE* f = fopen(INPUT_FILE, "r");
    if (f == NULL)
        return EXIT_FAILURE;

    // Parse crates and populate stack structures
    char buffer[512];
    while (fgets(buffer, sizeof(buffer), f))
    {
        // Subtract 1 from string length to ignore '\n' character
        size_t len = strlen(buffer) - 1;
        if (len == 0)
            break;

        // Skip " 1 2 3..." etc. line
        if (buffer[1] == '1')
            continue;

        if (num_stacks == 0)
        {
            num_stacks = (len + 1) / 4;
            assert(num_stacks <= MAX_STACKS);
        }

        for (size_t i = 0; i < len; i += 4)
        {
            if (buffer[i] == '[' && buffer[i + 2] == ']')
            {
                char crate = buffer[i + 1];
                stack_t* dst = &stacks[i / 4];
                dst->crates[dst->num_crates++] = crate;
            }
        }
    }

    // Reverse stack arrays as we built them top-down
    for (size_t i = 0; i < num_stacks; ++i)
        reverse_array(stacks[i].crates, stacks[i].num_crates);

    // Perform crate movements
    int count, src, dst;
    while (fscanf(f, "move %d from %d to %d\n", &count, &src, &dst) == 3)
        move_crates(count, src - 1, dst - 1);

    // Report topmost crates
    for (size_t i = 0; i < num_stacks; ++i)
    {
        stack_t* stack = &stacks[i];
        putchar(stack->num_crates ? stack->crates[stack->num_crates - 1] : '-');
    }
    putchar('\n');

    fclose(f);
    return EXIT_SUCCESS;
}
