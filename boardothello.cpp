#include "boardothello.h"

#include <iostream>
#include <sstream>
#include <cassert>
#include <cstdlib>

MoveOthello::MoveOthello(Token player) : Move(player), column(0), row(0), can_play(false) {}
MoveOthello::MoveOthello(Token player,Size column,Size row) : Move(player), column(column), row(row), can_play(true) {}

void MoveOthello::print() const {
	if (player!=NOT_PLAYED){
		switch (can_play) {
		case true:
			std::cout<<"column "<<this->column<<" row "<<this->row;
			break;
		case false:
			std::cout<<"can't play";
		}
		std::cout<<" for player "<<player;
	}
	else std::cout<<"othello null move";
}

Move *MoveOthello::deepcopy() const {
	Move *copy=NULL;
	if (can_play) copy=new MoveOthello(player,column,row); //normal move
	else copy=new MoveOthello(player); // "cant play" move
	return copy;
}


BoardOthello::BoardOthello(Size width,Size height) : width(width), height(height), size(width*height), played_count(4), cant_play_count(0), player_1_score(2), player_2_score(2) {

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

	//init board
	tokens[width/2-1][height/2-1]=PLAYER_1;
	tokens[width/2][height/2]=PLAYER_1;
	tokens[width/2-1][height/2]=PLAYER_2;
	tokens[width/2][height/2-1]=PLAYER_2;
	//score are updated in instance creation
}

BoardOthello::~BoardOthello() {
	delete [] tokens;
	delete [] flat;
}

Board *BoardOthello::deepcopy() const {
    BoardOthello *copy=new BoardOthello(width,height);

    //copy internal data
    copy->played_count=played_count;
	copy->cant_play_count=cant_play_count;
	copy->player_1_score=player_1_score;
	copy->player_2_score=player_2_score;

	//copy flat
    const Token *current_iter=flat;
	for (Token *iter=copy->flat; iter!=copy->flat+size; iter++) {
        *iter=*current_iter;
        current_iter++;
    }

    return copy;
}

Move *BoardOthello::parse_move_string(Token player,const char *string) const {
	std::stringstream stream(std::stringstream::in | std::stringstream::out);

	if (std::string(string)=="no") { //"no play" move
		Move *move=new MoveOthello(player); 

		if (is_move_valid(*move)) return move;

		delete move;
		return NULL;
	} else { //normal move
		int column=-1,row=-1;

		stream<<string;
		stream>>column>>row;

		if (stream.fail()) return NULL;

		Move *move=new MoveOthello(player,column,row);

		if (is_move_valid(*move)) return move;

		delete move;
		return NULL;
	}
}

void BoardOthello::print() const {
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

	std::cout<<"1: "<<player_1_score<<" 2: "<<player_2_score<<" x: "<<cant_play_count<<" "<<played_count<<" "<<size<<std::endl;
	std::cout<<std::endl;
}

bool BoardOthello::propagate(Token player, Size column, Size row, Size dcolumn, Size drow) const {
	Token enemy=other_player(player);
	Size length=0;

	//check enemy token
	while (row>=0 and row<height and column>=0 and column<width and tokens[column][row]==enemy) {
		length++;
		column+=dcolumn;
		row+=drow;
	}

	if (not length) return false;

	//check for a own token at the end
	if (row>=0 and row<height and column>=0 and column<width and tokens[column][row]==player) return true;

	return false;
}

bool BoardOthello::can_play_normal_move_at(Token player, Size column, Size row) const {
	if (tokens[column][row]!=NOT_PLAYED) return false;
	
	//horizontal and vertical
	if (propagate(player,column+1,row,1,0)) return true;
	if (propagate(player,column-1,row,-1,0)) return true;
	if (propagate(player,column,row+1,0,1)) return true;
	if (propagate(player,column,row-1,0,-1)) return true;

	//diagonals
	if (propagate(player,column+1,row+1,1,1)) return true;
	if (propagate(player,column+1,row-1,1,-1)) return true;
	if (propagate(player,column-1,row-1,-1,-1)) return true;
	if (propagate(player,column-1,row+1,-1,1)) return true;

	return false;
}

bool BoardOthello::can_play_normal_moves(Token player) const {
	for (Size column=0; column<width; column++) {
		for (Size row=0; row<height; row++) {
			if (can_play_normal_move_at(player,column,row)) return true;
		}
	}

	return false;
}

bool BoardOthello::is_move_valid(const Move &abstract_move) const {
	const MoveOthello &move=dynamic_cast<const MoveOthello&>(abstract_move);
	if (move.can_play) return can_play_normal_move_at(move.player,move.column,move.row); //just check if move is valid
	else return not can_play_normal_moves(move.player); //a cant play is invalid if you can play any normal move
}

Moves BoardOthello::get_possible_moves(Token player) const {
	Moves moves;
	
	for (Size column=0; column<width; column++) {
		for (Size row=0; row<height; row++) {
			if (can_play_normal_move_at(player,column,row)) moves.push_back(new MoveOthello(player,column,row));
		}
	}

	//if cant play normal moves and not player 2 "cant play" in a row then add a cant play moves
	if (moves.empty() and cant_play_count<3) moves.push_back(new MoveOthello(player));

	return moves;
}

Size BoardOthello::switch_tokens(Token player,Size column, Size row, Size dcolumn, Size drow) {
	if (not propagate(player,column,row,dcolumn,drow)) return 0; //FIXME should write a not_propagate for performance

	Token enemy=other_player(player);
	Size length=0;

	//switch enemy token
	while (row>=0 and row<height and column>=0 and column<width and tokens[column][row]==enemy) {
		tokens[column][row]=player;
		length++;
		column+=dcolumn;
		row+=drow;
	}

	return length;
}

void BoardOthello::play_move(const Move &abstract_move) {
	const MoveOthello &move=dynamic_cast<const MoveOthello&>(abstract_move);

	assert(this->is_move_valid(move));

	if (move.can_play) {
		tokens[move.column][move.row]=move.player;
		
		int dscore=0;
		Token player=move.player;
		Size column=move.column;
		Size row=move.row;
		//horizontal and vertical
		dscore+=switch_tokens(player,column+1,row,1,0);
		dscore+=switch_tokens(player,column-1,row,-1,0);
		dscore+=switch_tokens(player,column,row+1,0,1);
		dscore+=switch_tokens(player,column,row-1,0,-1);

		//diagonals
		dscore+=switch_tokens(player,column+1,row+1,1,1);
		dscore+=switch_tokens(player,column+1,row-1,1,-1);
		dscore+=switch_tokens(player,column-1,row-1,-1,-1);
		dscore+=switch_tokens(player,column-1,row+1,-1,1);
		
		//update scores
		if (move.player==PLAYER_1) {
			player_1_score+=dscore+1;
			player_2_score-=dscore;
		} else {
			player_2_score+=dscore+1;
			player_1_score-=dscore;
		}

		played_count++;
		cant_play_count=0;
	} else {
		cant_play_count++;
	}
}

bool BoardOthello::play_random_move(Token player) {
	if (played_count<size) {
		Moves possible_moves=get_possible_moves(player);

		if (possible_moves.empty()) return false; //two "cant play moves in a row"
			
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
	}
	
	//the board is full
	//std::cout<<"board full"<<std::endl;
	return false;
}

Token BoardOthello::check_for_win() const {
	if (played_count<size and cant_play_count<2) return NOT_PLAYED; //go on the game isn't finished
	else {
		if (player_1_score>=player_2_score) return PLAYER_1; //FIXME if score equals player1 win 
		return PLAYER_2;
	}
}

