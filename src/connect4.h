#ifndef _CONNECT_4_H_
#define _CONNECT_$_H_

#include <stdint.h>
#include <stdbool.h>

#define CONNECT4_DEFAULT_HEIGHT 6
#define CONNECT4_DEFAULT_WIDTH 7
#define CONNECT4_DEFAULT_NUM_PLAYERS 2
#define CONNECT4_DEFAULT_CONNECTN 4

/**
 * Contains all data necessary for a connect4 game
 *
 * Supports up to 255x255 size game boards,
 * allows any number of players which could fit on a board,
 * allows the number of in-a-row tiles required for victory 
 * to be set to any threshold (which could fit on any board).
 *
 * So go nuts
 */
struct Connect4Game {
    /** whether the game is active */
    bool gameOver; 
    /** the number of players */
    uint16_t players;
    /** the player who must drop the next tile, 0 if game over */
    uint16_t playerTurn;
    /** player who has won the game, 0 if game in progress */
    unsigned int winner;
    /** height of the game board */
    uint8_t height; 
    /** width of the game board */;
    uint8_t width;
    /** how many tiles in a row a required to win */
    uint8_t connectN;
    /** 
     * board[width][height], to be allocated to any size.
     * board[x][y] refers to the player whose tile is at (x,y) on the board,
     * where y=0 is at the bottom of the board (where tiles drop down to).
     * 0 refers to an empty tile. 
     */
    uint16_t **board;
};

/**
 * Sets up a classic Connect4Game
 * Height: 6, Width: 7, Players: 2
 * First player to line up 4 tiles wins
 *
 * @return 0 on success, -1 on failure
 */
int connect4_init(struct Connect4Game *game);

/** 
 * Sets up a custom Connect4Game 
 * 
 * @param game Pointer to a user-supplied Connect4Game struct
 * @param height Height of the game board
 * @param width Width of the game board
 * @param players Number of players
 * @param connectN Number of tiles in a row required for victory
 *
 * @return 0 on success, -1 on failure
 */
int connect4_custom_init(struct Connect4Game *game, 
        uint8_t height, 
        uint8_t width, 
        uint16_t players, 
        uint8_t connectN);

/**
 * Drops a tile into the specified column
 * for the player whose turn it currently is.
 * TODO: check for winner?
 *
 * @param game An initialized Connect4Game
 * @praam column The column in which the current player wishes to drop their tile
 *
 * @return 0 on success, -1 on failure
 */
int connect4_play(struct Connect4Game *game, uint8_t column);

/**
 * Empties the board for a new game
 *
 * @param game An initialized Connect4Game
 */
void connect4_new_game(struct Connect4Game *game);

/**
 * Frees memory associated with the game board 
 *
 * @param game An initialized Connect4Game
 */
void connect4_deinit(struct Connect4Game *game);

#endif /* _CONNECT_4_H_ */
