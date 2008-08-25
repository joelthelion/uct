#include "board.h"

#include <cassert>
#include <iostream>

Move::Move() : player(NOT_PLAYED) {}
Move::Move(Token player) : player(player) {}

void Move::print() const {
    std::cout<<"null move";
}

Move *Move::deepcopy() const {
	assert(false);
	return NULL;
}

Board::~Board() {}

Token Board::play_random_game(Token next_player) {

    Token winner=NOT_PLAYED;
	Token player=next_player;
	while (this->play_random_move(player)) {
        winner=this->check_for_win();
		
		if (winner) break;

		player=other_player(player);
    }

    return winner;
}

