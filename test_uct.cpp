#include "uct.h"
#include "boardc4.h"
#include "player.h"

#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[]) {
	std::cout.precision(2);
	std::cout.setf(std::ios::fixed,std::ios::floatfield);
    srand(time(NULL));

	Player *bot=new PlayerBot(PLAYER_1);
    Board *board=new BoardC4();


    board->print();

	//get the move
	Move *move=bot->get_move(board,NULL);
	//actually play the move
    board->play_move(*move);
	delete move;

    board->print();

    delete board;
	delete bot;

	return 0;
}
