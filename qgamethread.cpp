#include "qgamethread.h"

#include <iostream>

QGameThread::QGameThread(QMutex *mutex,Player *player_a,Player *player_b,Board *board) : QThread(), player_a(player_a), player_b(player_b), board(board), abort(false), mutex(mutex) {}

void QGameThread::set_abort(bool new_abort) {
    mutex->lock();
    abort=new_abort;
    mutex->unlock();
}

void QGameThread::run() {
	Player *player_current=player_a;
	Move *last_move=NULL;

	mutex->lock();
	while (not abort) {
		Board *copy=board->deepcopy();
		mutex->unlock();

		//get the move
		Move *move=player_current->get_move(copy,last_move);
		if (not move) {
			mutex->lock();
			break;
		}
        if (last_move) delete last_move;
		last_move=move;

		//actually play the move
		mutex->lock();
		board->play_move(*move);
		mutex->unlock();
		emit move_played();
		copy->play_move(*move);

		//check for win
		Token winner_token=copy->check_for_win();
		if (winner_token!=NOT_PLAYED) {
			if (winner_token==player_a->get_player()) winner=player_a;
			else winner=player_b;
			mutex->lock();
			break;
		}

		//switch player
		if (player_current==player_a) player_current=player_b;
		else player_current=player_a;

		delete copy;

		mutex->lock();
	}
	mutex->unlock();
    if (last_move) delete last_move;

	if (winner) {
		std::cout<<"winner: ";
		winner->print();
	} else {
		std::cout<<"draw";
	}
	std::cout<<std::endl;
}

