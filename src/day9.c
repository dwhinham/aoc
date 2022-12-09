// ----------------------------------------------------------------------------
// Advent of Code 2022 - Day 9
// Dale Whinham
//
// $ gcc day9.c -Wall -Wextra -Wpedantic -Werror -o day9
// $ ./day9
// ----------------------------------------------------------------------------

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "input/day9.txt"

#define PART_TWO

#ifdef PART_TWO
#define NUM_KNOTS 10
#else
#define NUM_KNOTS 2
#endif

typedef struct { int x; int y; } vec2_t;

static vec2_t knot_pos[NUM_KNOTS] = { 0 };
static vec2_t* tail_pos = NULL;
static size_t tail_pos_len = 0;

static void add_tail_pos(vec2_t pos)
{
    for (size_t i = 0; i < tail_pos_len; ++i)
        if (tail_pos[i].x == pos.x && tail_pos[i].y == pos.y)
            return;

    tail_pos = realloc(tail_pos, (tail_pos_len + 1) * sizeof(*tail_pos));
    tail_pos[tail_pos_len++] = pos;
}

static inline int sign(int x) { return (x > 0) - (x < 0); }

int main()
{
    FILE* f = fopen(INPUT_FILE, "r");
    if (f == NULL)
        return EXIT_FAILURE;

    char dir;
    int distance;
    while (fscanf(f, "%c %d\n", &dir, &distance) == 2)
    {
        int step = (dir == 'U' || dir == 'R') ? 1 : -1;
        int* axis = (dir == 'R' || dir == 'L') ? &knot_pos[0].x : &knot_pos[0].y;

        for (int i = 0; i < distance; ++i)
        {
            *axis += step;

            for (size_t j = 0; j < NUM_KNOTS - 1; ++j)
            {
                vec2_t* head_pos = &knot_pos[j];
                vec2_t* tail_pos = &knot_pos[j + 1];

                int delta_x = head_pos->x - tail_pos->x;
                int delta_y = head_pos->y - tail_pos->y;

                if (abs(delta_x) < 2 && abs(delta_y) < 2)
                    break;

                tail_pos->x += sign(delta_x);
                tail_pos->y += sign(delta_y);
            }

            add_tail_pos(knot_pos[NUM_KNOTS - 1]);
        }
    }

    printf("Tail positions: %zu\n", tail_pos_len);

    free(tail_pos);
    fclose(f);
    return EXIT_SUCCESS;
}
