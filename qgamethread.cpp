#include "qgamethread.h"

#include <iostream>

QGameThread::QGameThread(Player *player_a,Player *player_b,Board *board) : QThread(), player_a(player_a), player_b(player_b), board(board), abort(false), mutex() {}

void QGameThread::run() {
	std::cout<<"hello"<<std::endl;
	std::cout.flush();

	Player *player_current=player_a;
	Player *winner=NULL;
	Move * last_move=NULL;

	mutex.lock();
	while (not abort) {
		Board *copy=board->deepcopy();
		mutex.unlock();

		copy->print();

		//get the move
		Move *move=player_current->get_move(copy,last_move);
		if (not move) break;
		last_move=move;

		//actually play the move
		mutex.lock();
		board->play_move(*move);
		mutex.unlock();
		emit move_played();
		copy->play_move(*move);
		delete move;

		//check for win
		Token winner_token=copy->check_for_win();
		if (winner_token!=NOT_PLAYED) {
			if (winner_token==player_a->get_player()) winner=player_a;
			else winner=player_b;
			break;
		}

		//switch player
		if (player_current==player_a) player_current=player_b;
		else player_current=player_a;

		delete copy;

		mutex.lock();
	}
	Board *copy=board->deepcopy();
	mutex.unlock();

    copy->print();
	if (winner) {
		std::cout<<"winner: ";
		winner->print();
	} else {
		std::cout<<"draw";
	}
	std::cout<<std::endl;

	delete copy;
}

