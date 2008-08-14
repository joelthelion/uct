#include "boardc4.h"

#include <iostream>
#include <cassert>

MoveC4::MoveC4(const Board *abstract_board,Token player,Size column) : Move(abstract_board,player), column(column) {
	if (abstract_board) {
		const BoardC4 *board=dynamic_cast<const BoardC4*>(abstract_board);
		token=board->row_for_columns[column];
		row=token-board->tokens[column];
	}
}

void MoveC4::print() const {
	std::cout<<"("<<this->row<<","<<this->column<<")";
}

BoardC4::BoardC4(Size width,Size height,Size win_length) : lastmove(NULL,NOT_PLAYED,0) {
	this->width=width;
	this->height=height;
	this->win_length=win_length;
	this->size=width*height;

	//allocate flat
	this->flat=new Token[size];
	for (Token *iter=flat; iter!=flat+size; iter++) *iter=NOT_PLAYED;

	//allocate column pointer and playable move cache
	this->tokens=new Token*[width];
	this->row_for_columns=new Token*[width];
	Size k=0;
	for (Token *iter=flat; iter<=flat+size; iter+=height) {
		tokens[k]=iter;
		row_for_columns[k]=iter+height-1;
		k++;
	}
}

BoardC4::~BoardC4() {
	delete [] row_for_columns;
	delete [] tokens;
	delete [] flat;
}

void BoardC4::print() const {
	std::cout<<"  ";
	for (Size column=0; column<width; column++) std::cout<<column;
	std::cout<<std::endl;

	std::cout<<" +";
	for (Size column=0; column<width; column++) std::cout<<"-";
	std::cout<<"+"<<std::endl;

	for (Size row=0; row<height; row++) {
		std::cout<<row<<"|";
		for (Size column=0; column<width; column++) {
			switch(tokens[row][column]) {
			case NOT_PLAYED:
				std::cout<<" ";
				break;
			case PLAYER_1:
				std::cout<<"x";
				break;
			case PLAYER_2:
				std::cout<<"o";
				break;
			}
		}
		std::cout<<"|"<<row<<std::endl;
	}

	std::cout<<" +";
	for (Size column=0; column<width; column++) std::cout<<"-";
	std::cout<<"+"<<std::endl;

	std::cout<<"  ";
	for (Size column=0; column<width; column++) std::cout<<column;
	std::cout<<std::endl;
}

bool BoardC4::is_move_valid(const Move &abstract_move) const {
	const MoveC4 &move=dynamic_cast<const MoveC4&>(abstract_move);
	return move.row<height and move.column<width and not move.player==NOT_PLAYED and &tokens[move.row][move.column]==row_for_columns[move.column];
}

Moves BoardC4::get_possible_moves() const {
	Moves moves;
	return moves;
}

void BoardC4::play_move(const Move &abstract_move) {
	const MoveC4 &move=dynamic_cast<const MoveC4&>(abstract_move);

	assert(this->is_move_valid(move));

	tokens[move.row][move.column]=move.player;
	lastmove=move;
}

void BoardC4::play_random_move(const Move &move) {
}

bool BoardC4::check_for_win() const {
	return true;
}

Size BoardC4::propagate(Size row,Size column,Size drow,Size dcolumn,Token player) const {
	return 0;
}	

