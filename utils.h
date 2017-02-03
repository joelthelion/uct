#ifndef __UTILS__
#define __UTILS__

#include "board.h"
#include "player.h"

typedef enum {BOARDC4,BOARDC5,OTHELLO,BLOCKS,MATCHES,AWALE} Game;

Game parse_game(const char *arg);
float parse_float(const char *str,float defvalue);
Board *choose_game(Game game);
Player *play_game(Player *player_a,Player *player_b,Board *board,int max_move=0);

#endif
