#include "boardc4.h"

#include <iostream>
#include <sstream>
#include <cassert>
#include <cstdlib>

MoveC4::MoveC4(Token player,Size column) : Move(player), column(column) {}

void MoveC4::print() const {
	if (player!=NOT_PLAYED) std::cout<<"column "<<this->column<<" for player "<<player;
	else std::cout<<"c4 null move";
}

Move *MoveC4::deepcopy() const {
	Move *copy=new MoveC4(player,column);
	return copy;
}


BoardC4::BoardC4(Size width,Size height,Size win_length) : lastmove(NOT_PLAYED,0), width(width), height(height), win_length(win_length), size(width*height), played_count(0) {

	//allocate flat
	flat=new Token[size];
	for (Token *iter=flat; iter!=flat+size; iter++) *iter=NOT_PLAYED;

	//allocate column pointer and playable move cache
	tokens=new Token*[width];
	token_for_columns=new Token*[width];
	Size k=0;
	for (Token *iter=flat; iter<flat+size; iter+=height) {
		tokens[k]=iter;
		token_for_columns[k]=iter+height-1;
		k++;
	}

	assert(k==width);
}

BoardC4::~BoardC4() {
	delete [] token_for_columns;
	delete [] tokens;
	delete [] flat;
}

Board *BoardC4::deepcopy() const {
    BoardC4 *copy=new BoardC4(width,height,win_length);

    //copy last move and played_count
    copy->lastmove=lastmove;
    copy->played_count=played_count;

	//copy flat
    const Token *current_iter=flat;
	for (Token *iter=copy->flat; iter!=copy->flat+size; iter++) {
        *iter=*current_iter;
        current_iter++;
    }

	//copy token_for_columns
    for (int k=0; k<width; k++) {
        copy->token_for_columns[k]=copy->tokens[k]+(token_for_columns[k]-tokens[k]);
    }

    return copy;
}

Move *BoardC4::parse_move_string(Token player,const char *string) const {
	std::stringstream stream(std::stringstream::in | std::stringstream::out);
	int column=-1;

	stream<<string;
	stream>>column;

	if (stream.fail()) return NULL;

	Move *move=new MoveC4(player,column);

	if (is_move_valid(*move)) return move;

	delete move;
	return NULL;
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

bool BoardC4::is_move_valid(const Move &abstract_move) const {
	return is_move_valid(dynamic_cast<const MoveC4&>(abstract_move));
}

bool BoardC4::is_move_valid(const MoveC4 &move) const {
	return move.player!=NOT_PLAYED and move.column>=0 and move.column<width and token_for_columns[move.column]>=tokens[move.column];
}

Moves BoardC4::get_possible_moves(Token player) const {
	Moves moves;
	
	for (Size column=0; column<width; column++) {
		if (tokens[column]<=token_for_columns[column]) moves.push_back(new MoveC4(player,column));
	}

	return moves;
}

void BoardC4::play_move(const Move &abstract_move) {
	const MoveC4 &move=dynamic_cast<const MoveC4&>(abstract_move);

	assert(this->is_move_valid(move));

	*token_for_columns[move.column]=move.player;
	token_for_columns[move.column]--;

	played_count++;
	lastmove=move;
}

bool BoardC4::play_random_move(Token player) {
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

Token BoardC4::check_for_win() const {
    Size column=lastmove.column;
    Size row=token_for_columns[column]-tokens[column]+1;

	assert(propagate(row,column,-1,0,lastmove.player)==1); //move up are never played

    if (propagate(row,column,1,0,lastmove.player)+1>win_length) return lastmove.player;
    if (propagate(row,column,0,1,lastmove.player)+propagate(row,column,0,-1,lastmove.player)>win_length) return lastmove.player;
    if (propagate(row,column,1,1,lastmove.player)+propagate(row,column,-1,-1,lastmove.player)>win_length) return lastmove.player;
    if (propagate(row,column,1,-1,lastmove.player)+propagate(row,column,-1,1,lastmove.player)>win_length) return lastmove.player;

	return NOT_PLAYED;
}

Size BoardC4::propagate(Size row,Size column,Size drow,Size dcolumn,Token player) const {
    Size length=0;
    while (row>=0 and row<height and column>=0 and column<width and tokens[column][row]==player) {
        length++;
        row+=drow;
        column+=dcolumn;
    }

	return length;
}	

