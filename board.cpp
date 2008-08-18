#include "board.h"

#include <iostream>

Move::Move(Token player) : player(player) {}

Token Board::play_random_game() {

    Token winner=NOT_PLAYED;
	Token player=PLAYER_1;
	while (this->play_random_move(player)) {
        if (this->check_for_win()) {
            winner=player;
            break;
        } else {
            player=other_player(player);
        }
    }

    if (winner==NOT_PLAYED) std::cout<<"draw"<<std::endl;
    else std::cout<<"player "<<player<<" won"<<std::endl;
	this->print();

    return winner;
}

