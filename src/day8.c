// ----------------------------------------------------------------------------
// Advent of Code 2022 - Day 8
// Dale Whinham
//
// $ gcc day8.c -Wall -Wextra -Wpedantic -Werror -o day8
// $ ./day8
// ----------------------------------------------------------------------------

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "input/day8.txt"

typedef enum { DIR_NORTH, DIR_EAST, DIR_SOUTH, DIR_WEST, DIR_MAX } direction_t;

typedef struct
{
    uint8_t visibility;
    uint8_t height;
} tree_t;

static tree_t** tree_grid;
static size_t grid_w = 0, grid_h = 0;

static uint32_t scenic_score_direction(size_t x, size_t y, direction_t dir)
{
    static const struct { int x; int y; } dir_inc[DIR_MAX] =
    {
        { 0, -1 }, { 1, 0 }, { 0, 1 }, { -1, 0 },
    };

    int x_inc = dir_inc[dir].x, y_inc = dir_inc[dir].y;
    int x_pos = x + x_inc, y_pos = y + y_inc;

    tree_t* tree = &tree_grid[y][x];
    uint32_t score = 0;

    while (x_pos >= 0 && x_pos < (int)grid_w
        && y_pos >= 0 && y_pos < (int)grid_h)
    {
        ++score;
        if (tree_grid[y_pos][x_pos].height >= tree->height)
            break;
        x_pos += x_inc;
        y_pos += y_inc;
    }

    return score;
}

static inline uint32_t compute_scenic_score(size_t x, size_t y)
{
    return scenic_score_direction(x, y, DIR_NORTH)
         * scenic_score_direction(x, y, DIR_EAST)
         * scenic_score_direction(x, y, DIR_SOUTH)
         * scenic_score_direction(x, y, DIR_WEST);
}

int main()
{
    FILE* f = fopen(INPUT_FILE, "r");

    char line_buffer[512];
    while (fgets(line_buffer, sizeof(line_buffer), f))
    {
        if (grid_w == 0)
            grid_w = strlen(line_buffer) - 1;

        tree_grid = realloc(tree_grid, ++grid_h * sizeof(*tree_grid));
        tree_grid[grid_h - 1] = malloc(grid_w * sizeof(**tree_grid));

        for (size_t i = 0; i < grid_w; ++i)
            tree_grid[grid_h - 1][i] = (tree_t) { .height = line_buffer[i] - '0', .visibility = 0, };
    }

    // Compute east-west visibility
    for (size_t y = 0; y < grid_h; ++y)
    {
        int8_t max_west = -1;
        int8_t max_east = -1;

        for (size_t x = 0; x < grid_w; ++x)
        {
            if (tree_grid[y][grid_w - 1 - x].height > max_east)
            {
                max_east = tree_grid[y][grid_w - 1 - x].height;
                tree_grid[y][grid_w - 1 - x].visibility |= (1 << DIR_EAST);
            }

            if (tree_grid[y][x].height > max_west)
            {
                max_west = tree_grid[y][x].height;
                tree_grid[y][x].visibility |= (1 << DIR_WEST);
            }
        }
    }

    // Compute north-south visibility
    for (size_t x = 0; x < grid_w; ++x)
    {
        int8_t max_north = -1;
        int8_t max_south = -1;

        for (size_t y = 0; y < grid_h; ++y)
        {
            if (tree_grid[grid_h - 1 - y][x].height > max_south)
            {
                max_south = tree_grid[grid_h - 1 - y][x].height;
                tree_grid[grid_h - 1 - y][x].visibility |= (1 << DIR_SOUTH);
            }

            if (tree_grid[y][x].height > max_north)
            {
                max_north = tree_grid[y][x].height;
                tree_grid[y][x].visibility |= (1 << DIR_NORTH);
            }
        }
    }

    size_t visible_count = 0;
    uint32_t best_scenic_score = 0;

    for (size_t y = 0; y < grid_h; ++y)
    {
        for (size_t x = 0; x < grid_w; ++x)
        {
            tree_t* tree = &tree_grid[y][x];
            visible_count += !!tree->visibility;

            uint32_t scenic_score = compute_scenic_score(x, y);
            if (scenic_score > best_scenic_score)
                best_scenic_score = scenic_score;

            if (tree->visibility)
                printf("\033[1;32m%d\033[0m", tree->height);
            else
                printf("\033[1;90m%d\033[0m", tree->height);
        }
        putchar('\n');
    }

    printf("Visible trees:     %zu\n", visible_count);
    printf("Best scenic score: %d\n", best_scenic_score);

    for (size_t y = 0; y < grid_h; ++y)
        free(tree_grid[y]);
    free(tree_grid);
    fclose(f);
    return EXIT_SUCCESS;
}
