// ----------------------------------------------------------------------------
// Advent of Code 2022 - Day 2
// Dale Whinham
//
// $ gcc day2.c -o day2
// $ ./day2
// ----------------------------------------------------------------------------

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "day2_input.txt"

#define PART_TWO

enum
{
    SHAPE_ROCK = 0,
    SHAPE_PAPER = 1,
    SHAPE_SCISSORS = 2,
    SCORE_LOSE = 0,
    SCORE_DRAW = 3,
    SCORE_WIN = 6,
};

// Outcomes
// 0 - 0 =  0 (draw)
// 0 - 1 = -1 (win)
// 0 - 2 = -2 (loss)
// 1 - 0 =  1 (loss)
// 1 - 1 =  0 (draw)
// 1 - 2 = -1 (win)
// 2 - 0 =  2 (win)
// 2 - 1 =  1 (loss)
// 2 - 2 =  0 (draw)

long file_size(FILE* f)
{
    long current_pos = ftell(f);
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, current_pos, SEEK_SET);
    return size;
}

char* read_file(const char* path, size_t* out_size)
{
    FILE* f = fopen(path, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Couldn't open %s for reading\n", path);
        return NULL;
    }

    *out_size = file_size(f);
    char* text = malloc(*out_size);
    fread(text, 1, *out_size, f);
    fclose(f);

    return text;
}

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    size_t size;
    char* text = read_file(INPUT_FILE, &size);
    if (text == NULL)
        return EXIT_FAILURE;

    uint32_t score_player = 0;
    uint32_t score_opponent = 0;

    char* token = strtok(text, " \n");
    while (token)
    {
        int opponent_move = *token - 'A';
        token = strtok(NULL, " \n");
        int player_move = *token - 'X';

#ifdef PART_TWO
        switch (player_move)
        {
            // Should lose
            case 0:
                player_move = opponent_move == SHAPE_ROCK ? SHAPE_SCISSORS : opponent_move - 1;
                break;

             // Should draw
            case 1:
                player_move = opponent_move;
                break;

             // Should win
            case 2:
                player_move = opponent_move == SHAPE_SCISSORS ? SHAPE_ROCK : opponent_move + 1;
                break;
        }
#endif

        score_opponent += opponent_move + 1;
        score_player += player_move + 1;

        int result = opponent_move - player_move;

        // Draw
        if (result == 0)
        {
            score_player += SCORE_DRAW;
            score_opponent += SCORE_DRAW;
        }

        // Player wins
        else if (result == -1 || result == 2)
            score_player += SCORE_WIN;

        // Opponent wins
        else
            score_opponent += SCORE_WIN;

        token = strtok(NULL, " \n");
    }

    printf(
        "Player score:   %d\n"
        "Opponent score: %d\n",
        score_player,
        score_opponent
    );

    free(text);
    return EXIT_SUCCESS;
}
