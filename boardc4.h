#ifndef __BOARDC4__
#define __BOARDC4__

#include <vector>
#include "board.h"
#include "common.h"

class MoveC4 : public Move {
public:
    virtual void print() const;

    Size row,column;
    Token *token;
}

class BoardC4 : public Board {
public:
	typedef MoveC4 Move;

	BoardC4(Size width=7,Size height=5,Size win_length=4);
	~BoardC4();

	virtual void print() const =0;
	virtual bool is_move_valid(Move move) const =0;
	virtual Moves get_possible_moves() const =0; //FIXME not sure about constness
	virtual void play_move(Move move) =0;
	virtual void play_random_move(Move move) =0;
	virtual bool check_for_win() const =0;

protected:
	Size propagate(Size row,Size column,Size drow,Size dcolumn,Token player) const;

private:
	Move lastmove;

	Size width,height,win_length;
	Token *flat;
	Token **tokens;
	Token **row_for_columns;
};

#endif
