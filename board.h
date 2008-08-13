#ifndef __BOARD__
#define __BOARD__

#include "common.h"

class Board {
public:
	class Move {
	public:
		Size row,column;
		Token *token;
	}

	Board(Size width=7,Size height=5,Size win_length=4);
	~Board();

	void print() const;

protected:
	bool valid_move(Move move);

private:
	Size width,height,win_length;
	Token *flat;
	Token **tokens;
	Token **row_for_columns;
};

#endif
