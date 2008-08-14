#ifndef __BOARDC4__
#define __BOARDC4__

#include "board.h"
#include "common.h"

class MoveC4 : public Move {
public:
	MoveC4(const Board *board,Token player,Size column);

    virtual void print() const;

    Size row,column;
    const Token *token;
};

class BoardC4 : public Board {
friend class MoveC4;
public:
	BoardC4(Size width=7,Size height=5,Size win_length=4);
	~BoardC4();

	virtual void print() const;
	virtual bool is_move_valid(const Move &move) const;
	virtual Moves get_possible_moves() const; //FIXME not sure about constness
	virtual void play_move(const Move &move);
	virtual void play_random_move(const Move &move);
	virtual bool check_for_win() const;

protected:
	Size propagate(Size row,Size column,Size drow,Size dcolumn,Token player) const;

private:
	MoveC4 lastmove;

	Size width;
	Size height;
	Size win_length;
	Size size;
	Token *flat;
	Token **row_for_columns;
	Token **tokens;
};

#endif
