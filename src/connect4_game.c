#include <stdio.h>
#include <stdlib.h>
#include "connect4.h"

/* TODO: unjankify this */
int get_player_input(int width) 
{
    int i=-1;
    while (i < 0 || i >= width) {
        char c = getchar();
        while (getchar() != '\n') {} /* flush other characters */
        /* 
         * normally you'd subtract 48 to get an int,
         * but we want to convert from index-by-1 to index-by 0
         */
        i = c - 49;
    }
    return i;
}

/* TODO: prettify */
void display(struct Connect4Game *game) 
{
    printf("\n");
    for (int y=game->height-1; y>=0; y--) {
        for (int x=0; x<game->width; x++) {
            printf("%d ", game->board[x][y]);
        }
        printf("\n");
    }

    printf("\n");
    for (int x=1; x<=game->width; x++) {
        printf("%d ", x);
    }
    printf("\n");
}

int main(int argc, char **argv) 
{
    int height = CONNECT4_DEFAULT_HEIGHT;
    int width = CONNECT4_DEFAULT_WIDTH;
    int num_players = CONNECT4_DEFAULT_NUM_PLAYERS;
    int connectN = CONNECT4_DEFAULT_CONNECTN;

    if (argc > 2) {
        /* TODO: never use atoi ever */
        height = atoi(argv[1]);
        width = atoi(argv[2]);
    }
    
    if (argc > 3) {
        num_players = atoi(argv[3]);
    }

    if (argc > 4) {
        connectN = atoi(argv[4]);
    }
        
    struct Connect4Game game;
    connect4_custom_init(&game, height, width, num_players, connectN);
    display(&game);

    while (1) {
        int i = get_player_input(game.width);
        connect4_play(&game, i);
        display(&game);
        if (game.winner != 0) {
            printf("\nPlayer %d Won!\n", game.winner);
            break;
        }
    }

    return 0;
}
