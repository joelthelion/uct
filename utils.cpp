#include "utils.h"

#include <iostream>
#include "boardc4.h"
#include "boardc5.h"
#include "boardothello.h"

Board *choose_game(Game game) {
	switch (game) {
	case BOARDC4:
		return new BoardC4();
	case BOARDC5:
		return new BoardC5();
	case OTHELLO:
		return new BoardOthello();
	}
}

Player *play_game(Player *player_a,Player *player_b,Board *board) {
	Player *player_current=player_a;
	Player *winner=NULL;

	while (true) {
		board->print();

		//get the move
		Move *move=player_current->get_move(board);
		if (not move) break;

		//actually play the move
		board->play_move(*move);
		delete move;

		//check for win
		Token winner_token=board->check_for_win();
		if (winner_token!=NOT_PLAYED) {
			winner=player_current;
			break;
		}

		//switch player
		if (player_current==player_a) player_current=player_b;
		else player_current=player_a;
	}

    board->print();
	if (winner) {
		std::cout<<"winner: ";
		winner->print();
	} else {
		std::cout<<"draw";
	}
	std::cout<<std::endl;

	return winner;
}

