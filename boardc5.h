#ifndef __BOARDC5__
#define __BOARDC5__

#include "board.h"

class MoveC5 : public Move {
friend class BoardC5;
public:
	MoveC5(Token player,Size column,Size row);

    virtual void print() const;
	virtual Move *deepcopy() const;

private:
    Size column;
    Size row;
};

class BoardC5 : public Board {
public:
	BoardC5(Size width=6,Size height=6,Size win_length=4);
	~BoardC5();

    virtual Board *deepcopy() const;
	virtual Move *parse_move_string(Token player,const char *string) const;
	virtual void print() const;
	inline virtual bool is_move_valid(const Move &move) const;
	virtual Moves get_possible_moves(Token player) const;
	virtual void play_move(const Move &move);
	virtual bool play_random_move(Token player);
	virtual bool check_for_win() const;

protected:
	inline Size propagate(Size row,Size column,Size drow,Size dcolumn,Token player) const;

private:
	MoveC5 lastmove;

	Size width;
	Size height;
	Size win_length;
	Size size,played_count;
	Token *flat;
	Token **tokens;
};

#endif
