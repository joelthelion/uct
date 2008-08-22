#include "boardc5.h"

#include <iostream>
#include <sstream>
#include <cassert>
#include <cstdlib>

MoveC5::MoveC5(Token player,Size column,Size row) : Move(player), column(column), row(row) {}

void MoveC5::print() const {
	if (player!=NOT_PLAYED) std::cout<<"column "<<this->column<<" row "<<this->row<<" for player "<<player;
	else std::cout<<"c5 null move";
}

Move *MoveC5::deepcopy() const {
	Move *copy=new MoveC5(player,column,row);
	return copy;
}


BoardC5::BoardC5(Size width,Size height,Size win_length) : lastmove(NOT_PLAYED,0,0), width(width), height(height), win_length(win_length), size(width*height), played_count(0) {

	//allocate flat
	flat=new Token[size];
	for (Token *iter=flat; iter!=flat+size; iter++) *iter=NOT_PLAYED;

	//allocate column pointer and playable move cache
	tokens=new Token*[width];
	Size k=0;
	for (Token *iter=flat; iter<flat+size; iter+=height) {
		tokens[k]=iter;
		k++;
	}

	assert(k==width);
}

BoardC5::~BoardC5() {
	delete [] tokens;
	delete [] flat;
}

Board *BoardC5::deepcopy() const {
    BoardC5 *copy=new BoardC5(width,height,win_length);

    //copy last move and played_count
    copy->lastmove=lastmove;
    copy->played_count=played_count;

	//copy flat
    const Token *current_iter=flat;
	for (Token *iter=copy->flat; iter!=copy->flat+size; iter++) {
        *iter=*current_iter;
        current_iter++;
    }

    return copy;
}

Move *BoardC5::parse_move_string(Token player,const char *string) const {
	std::stringstream stream(std::stringstream::in | std::stringstream::out);
	int column=-1,row=-1;

	stream<<string;
	stream>>column>>row;

	if (stream.fail()) return NULL;

	Move *move=new MoveC5(player,column,row);

	if (is_move_valid(*move)) return move;

	delete move;
	return NULL;
}

void BoardC5::print() const {
	std::cout<<"  ";
	for (Size column=0; column<width; column++) std::cout<<column;
	std::cout<<std::endl;

	std::cout<<" +";
	for (Size column=0; column<width; column++) std::cout<<"-";
	std::cout<<"+"<<std::endl;

	for (Size row=0; row<height; row++) {
		std::cout<<row<<"|";
		for (Size column=0; column<width; column++) {
			switch(tokens[column][row]) {
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

bool BoardC5::is_move_valid(const Move &abstract_move) const {
	const MoveC5 &move=dynamic_cast<const MoveC5&>(abstract_move);
	return move.player!=NOT_PLAYED and move.column>=0 and move.column<width and move.row>=0 and move.row<height and tokens[move.column][move.row]==NOT_PLAYED;
}

Moves BoardC5::get_possible_moves(Token player) const {
	Moves moves;
	
	for (Size column=0; column<width; column++) {
		for (Size row=0; row<height; row++) {
			if (tokens[column][row]==NOT_PLAYED) moves.push_back(new MoveC5(player,column,row));
		}
	}

	return moves;
}

void BoardC5::play_move(const Move &abstract_move) {
	const MoveC5 &move=dynamic_cast<const MoveC5&>(abstract_move);

	assert(this->is_move_valid(move));

	tokens[move.column][move.row]=move.player;

	played_count++;
	lastmove=move;
}

bool BoardC5::play_random_move(Token player) {
	if (played_count<size) {
		Moves possible_moves=get_possible_moves(player);

		int selected=rand() % possible_moves.size();
		Moves::const_iterator selected_iter=possible_moves.begin();
		while (selected>0) {
			selected--;
			selected_iter++;
		}
		play_move(**selected_iter);

		//play_move(*selected);
		//Move *selected=possible_moves[rand()%possible_moves.size()];
		//play_move(*selected);

		for (Moves::iterator iter=possible_moves.begin(); iter!=possible_moves.end(); iter++) delete *iter;

		return true;
	} else {
		//std::cout<<"board full"<<std::endl;
		return false;
	}
}

Token BoardC5::check_for_win() const {
    Size column=lastmove.column;
    Size row=lastmove.row;

    if (propagate(row,column,1,0,lastmove.player)+propagate(row,column,-1,0,lastmove.player)>win_length) return lastmove.player;
    if (propagate(row,column,0,1,lastmove.player)+propagate(row,column,0,-1,lastmove.player)>win_length) return lastmove.player;
    if (propagate(row,column,1,1,lastmove.player)+propagate(row,column,-1,-1,lastmove.player)>win_length) return lastmove.player;
    if (propagate(row,column,1,-1,lastmove.player)+propagate(row,column,-1,1,lastmove.player)>win_length) return lastmove.player;

	return NOT_PLAYED;
}

Size BoardC5::propagate(Size row,Size column,Size drow,Size dcolumn,Token player) const {
    Size length=0;
    while (row>=0 and row<height and column>=0 and column<width and tokens[column][row]==player) {
        length++;
        row+=drow;
        column+=dcolumn;
    }

	return length;
}	

