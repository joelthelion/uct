#ifndef __UTILS__
#define __UTILS__

#include "board.h"
#include "player.h"

Board *choose_game();
Player *play_game(Player *player_a,Player *player_b,Board *board);

#endif
