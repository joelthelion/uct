#include "board.h"

#include <cassert>
#include <iostream>

Move::Move() : player(NOT_PLAYED) {}
Move::Move(Token player) : player(player) {}

void Move::print() const {
    std::cout<<"null move";
};

Token Board::play_random_game(Token next_player) {

    Token winner=NOT_PLAYED;
	Token player=next_player;
	while (this->play_random_move(player)) {
        if (this->check_for_win()) {
            winner=player;
            break;
        } else {
            player=other_player(player);
        }
    }

    return winner;
}

