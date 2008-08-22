#ifndef __UTILS__
#define __UTILS__

#include "board.h"
#include "player.h"

typedef enum {BOARDC4,BOARDC5,OTHELLO} Game;

Board *choose_game(Game game);
Player *play_game(Player *player_a,Player *player_b,Board *board);

#endif
