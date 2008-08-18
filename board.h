#ifndef __BOARD__
#define __BOARD__

#include <vector>
#include "common.h"

class Board;

class Move {
public:
	Move(Token player);
    virtual void print() const =0;

	Token player;
};

typedef std::vector<Move*> Moves;

class Board {
public:
	virtual void print() const =0;
	virtual bool is_move_valid(const Move &move) const =0;
	virtual Moves get_possible_moves(Token player) const =0; //FIXME possible constness problem
	virtual void play_move(const Move &move) =0;
	virtual bool play_random_move(Token player) =0;
	virtual bool check_for_win() const =0;
    virtual Token play_random_game();
};

#endif
