#include "board.h"

#include <iostream>

Move::Move(Token player) : player(player) {}

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

