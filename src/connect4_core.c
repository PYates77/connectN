#include <stdlib.h>
#include <string.h>
#include "connect4.h"

/* 
 * TODO: 
 * Error codes to differentiate memory alloc failure vs. invalid game state vs. invalid player move?
 * When players choose a column: index by 0 or index by 1?
 */

/* initializes game with default parameters */
int connect4_init(struct Connect4Game *game) 
{
    return connect4_custom_init(game,
            CONNECT4_DEFAULT_HEIGHT,
            CONNECT4_DEFAULT_WIDTH,
            CONNECT4_DEFAULT_NUM_PLAYERS,
            CONNECT4_DEFAULT_CONNECTN);
}

int connect4_custom_init(struct Connect4Game *game, 
        uint8_t height, 
        uint8_t width, 
        uint16_t players, 
        uint8_t connectN) 
{
    /* check for valid input */
    if (game == 0 || players <= 0 || height <= 0 || width <= 0 || connectN <= 0) {
        return -1;
    }

    int rc = -1;
    game->gameOver = false;
    game->players = players;
    game->playerTurn = 1;
    game->winner = 0;
    game->height = height;
    game->width = width;
    game->connectN = connectN;

    /* 
     * Set up enough space both for a NxN region, but also the pointers for convenient game[x][y] indexing 
     * (first <width> entries are pointers, next <width>x<height> are integers)
     */
    game->board = malloc(width * sizeof(*game->board) + (width * height * sizeof(**game->board)));
    if (game->board != 0) {
        uint16_t *data = (uint16_t *)(game->board + width);
        for (int x=0; x<width; x++) {
            game->board[x] = data + (x * height);
        }
        rc = 0;
    } 

    return rc;
}

/* 
 * Check whether the player whose turn it currently is has won 
 * Sets the "winner" field in the game struct
 *
 * Returns 0 for success, -1 for error
 *
 * TODO: check any arbitrary board for all possible winners?
 * TODO: early returns are spaghetti code?
 */
int connect4_check_winner(struct Connect4Game *game) 
{
    if (game == 0) {
        return -1;
    }

    game->winner = 0;

    /* check vertically */
    for (int x=0; x<game->width; x++) {
        int count = 0;
        for(int y=0; y<game->height; y++) {
            /* 
             * since we start checking from the bottom
             * we can stop counting when we reach the first empty slot
             * because gravity
             */
            if (game->board[x][y] == 0) {
                break;
            }

            if (game->board[x][y] == game->playerTurn) {
                count++;
            } else {
                count = 0;
            }

            if (count >= game->connectN) {
                game->winner = game->playerTurn;
                return 0;
            }
        }
    }

    /* check horizontally */
    for (int y=0; y<game->height; y++) {
        int count = 0;
        for(int x=0; x<game->width; x++) {
            if (game->board[x][y] == game->playerTurn) {
                count++;
            } else {
                count = 0;
            }

            if (count >= game->connectN) {
                game->winner = game->playerTurn;
                return 0;
            }
        }
    }

    /* check diagonally with positive slope */
    for (int x=0; x<game->width-game->connectN; x++) {
        for (int y=0; y<game->height-game->connectN; y++) {
            int count = 0;
            for (int i=0; i<game->connectN; i++) {
                int curX = x+i;
                int curY = y+i;

                if (game->board[curX][curY] == game->playerTurn) {
                    count++;
                } else {
                    count = 0;
                }

                if (count >= game->connectN) {
                    game->winner = game->playerTurn;
                    return 0;
                }
            }
        }
    }

    /* check diagonally with negative slope */
    for (int x=0; x<game->width-game->connectN; x++) {
        for (int y=game->connectN-1; y<game->height; y++) {
            int count = 0;
            for (int i=0; i<game->connectN; i++) {
                int curX = x+i;
                int curY = y-i;

                if (game->board[curX][curY] == game->playerTurn) {
                    count++;
                } else {
                    count = 0;
                }

                if (count >= game->connectN) {
                    game->winner = game->playerTurn;
                    return 0;
                }
            }
        }
    }

    return 0;
}

int connect4_play(struct Connect4Game *game, uint8_t column) 
{
    if (game == 0 || column >= game->width) {
        return -1;
    }

    int rc = -1;
    int y = game->height-1;

    /* ensure that the column isn't full */
    if (game->board[column][y] == 0) {
        rc = 0; /* we will certainly be able to place the piece, so set rc to success */
        /* drop the player's piece down until it hits the bottom or rests upon another piece */
        game->board[column][y] = game->playerTurn;
        while (y > 0 && game->board[column][y-1] == 0) {
            game->board[column][y] = 0;
            y--;
            game->board[column][y] = game->playerTurn;
        }
    }

    rc = connect4_check_winner(game);
    /* TODO: some sort of error handling?? */

    if (rc == 0) {
        /* current player's turn is indexed from 1 */
        game->playerTurn = (game->playerTurn%game->players)+1;
    }

    return rc;
}

/* 
 * Check whether the game board is valid, with no floating pieces 
 * and a valid number of pieces (assuming players alternate turns)
 * TODO
 */
int connect4_is_valid(struct Connect4Game *game) 
{
    if (game == 0) {
        return -1;
    }

    return 0;
}

void connect4_deinit(struct Connect4Game *game) {
    free(game->board);
    memset(game, 0, sizeof(*game));
}
