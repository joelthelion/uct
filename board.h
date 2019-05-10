#ifndef __BOARD__
#define __BOARD__

#include <list>
#include "common.h"

class Board;

class Move {
public:
    Move();

    explicit Move(Token player);

	virtual Move *deepcopy() const;
    virtual void print() const;
	virtual bool compare(const Move& move) const;

	Token player;
};

typedef std::list<Move*> Moves;

class Board {
public:
    virtual ~Board() =0;

    virtual Board *deepcopy() const =0;
	virtual Move *parse_move_string(Token player,const char *string) const =0;
	virtual void print() const =0;
	virtual bool is_move_valid(const Move &move) const =0;
	virtual Moves get_possible_moves(Token player) const =0; //FIXME possible constness problem
	virtual void play_move(const Move &move) =0;
	virtual bool play_random_move(Token player) =0;
	virtual Token check_for_win() const =0;
    virtual Token play_random_game(Token next_player);
};

#endif
