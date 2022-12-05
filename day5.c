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

static inline void stack_crate(size_t stack_index, char crate)
{
    stack_t* dst = &stacks[stack_index];
    memmove(dst->crates + 1, dst->crates, dst->num_crates * sizeof(char));
    dst->crates[0] = crate;
    ++dst->num_crates;
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

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

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
                size_t stack_index = i / 4;
                stack_crate(stack_index, crate);
            }
        }
    }

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
