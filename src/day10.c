// ----------------------------------------------------------------------------
// Advent of Code 2022 - Day 10
// Dale Whinham
//
// $ gcc day10.c -Wall -Wextra -Wpedantic -Werror -o day10
// $ ./day10
// ----------------------------------------------------------------------------

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "input/day10.txt"

#define PART_TWO

#ifdef PART_TWO
#define SCREEN_WIDTH  40
#define SCREEN_HEIGHT 6
#else
#define CYCLE_RANGE_START 20
#define CYCLE_RANGE_END   220
#define CYCLE_RANGE_STEP  40

static uint32_t signal_strength = 0;
#endif

static void do_cycle(uint32_t cycle, int32_t reg_x)
{
#ifdef PART_TWO
    int32_t screen_x = (cycle - 1) % SCREEN_WIDTH;
    printf((screen_x >= reg_x - 1 && screen_x <= reg_x + 1) ? "\033[1;32m#\033[0m" : " ");
    if (screen_x == SCREEN_WIDTH - 1)
        putchar('\n');
#else
    if (cycle >= CYCLE_RANGE_START && cycle <= CYCLE_RANGE_END && (cycle - CYCLE_RANGE_START) % CYCLE_RANGE_STEP == 0)
    {
        printf("Cycle %d: X = %d (%d)\n", cycle, reg_x, reg_x * cycle);
        signal_strength += reg_x * cycle;
    }
#endif
}

int main()
{
    FILE* f = fopen(INPUT_FILE, "r");
    if (f == NULL)
        return EXIT_FAILURE;

    uint32_t cycles = 0;
    int32_t reg_x = 1;

    char instruction[5];
    int operand;
    while (fscanf(f, "%s %d\n", instruction, &operand) > 0)
    {
        if (strcmp(instruction, "noop") == 0)
            do_cycle(++cycles, reg_x);
        else if (strcmp(instruction, "addx") == 0)
        {
            do_cycle(++cycles, reg_x);
            do_cycle(++cycles, reg_x);
            reg_x += operand;
        }
    }

#ifndef PART_TWO
    printf("Signal strength: %d\n", signal_strength);
#endif

    fclose(f);
    return EXIT_SUCCESS;
}
