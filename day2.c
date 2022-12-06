// ----------------------------------------------------------------------------
// Advent of Code 2022 - Day 2
// Dale Whinham
//
// $ gcc day2.c -Wall -Wextra -Wpedantic -Werror -o day2
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

int main()
{
    FILE* f = fopen(INPUT_FILE, "r");
    if (f == NULL)
        return EXIT_FAILURE;

    uint32_t score_player = 0;
    uint32_t score_opponent = 0;

    char opponent_move, player_move;
    while (fscanf(f, "%c %c\n", &opponent_move, &player_move) == 2)
    {
        opponent_move -= 'A';
        player_move -= 'X';

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
    }

    printf
    (
        "Player score:   %d\n"
        "Opponent score: %d\n",
        score_player,
        score_opponent
    );

    fclose(f);
    return EXIT_SUCCESS;
}
