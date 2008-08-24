#include "qgamethread.h"

#include <iostream>

QGameThread::QGameThread(Player *player_a,Player *player_b,Board *board) : QThread(), player_a(player_a), player_b(player_b), board(board), abort(false), mutex() {}

void QGameThread::run() {
	std::cout<<"hello"<<std::endl;
	std::cout.flush();
}

