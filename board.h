#ifndef __BOARD__
#define __BOARD__

#include <vector>
#include "common.h"

class Board {
public:
	class Move {
	public:
		Size row,column;
		Token *token;
	}
	typedef std::vector<Move> Moves;

	Board(Size width=7,Size height=5,Size win_length=4);
	~Board();

	void print() const;
	bool is_move_valid(Move move) const;
	Moves get_possible_moves() const; //FIXME not sure about constness
	Move play_move(Move move);
	Move play_random_move(Move move);
	Move check_for_win_at_move(Move move); //FIXME not general

protected:
	Size propagate(Size row,Size column,Size drow,Size dcolumn,Token player) const;

private:
	Size width,height,win_length;
	Token *flat;
	Token **tokens;
	Token **row_for_columns;
};

#endif
