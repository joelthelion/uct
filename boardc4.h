#ifndef __BOARDC4__
#define __BOARDC4__

#include "board.h"

class MoveC4 : public Move {
friend class BoardC4;
public:
	MoveC4(Token player,Size column);

    virtual void print() const;
	virtual Move *deepcopy() const;

private:
    Size column;
};

class BoardC4 : public Board {
public:
	BoardC4(Size width=7,Size height=5,Size win_length=4);
	~BoardC4();

    virtual Board *deepcopy() const;
	virtual void print() const;
	virtual bool is_move_valid(const Move &move) const;
	virtual Moves get_possible_moves(Token player) const; //FIXME not sure about constness
	virtual void play_move(const Move &move);
	virtual bool play_random_move(Token player);
	virtual bool check_for_win() const; //you can't loose when you play a move

protected:
	Size propagate(Size row,Size column,Size drow,Size dcolumn,Token player) const;

private:
	MoveC4 lastmove;

	Size width;
	Size height;
	Size win_length;
	Size size,played_count;
	Token *flat;
	Token **token_for_columns;
	Token **tokens;
};

#endif
