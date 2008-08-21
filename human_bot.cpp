#include "player.h"
#include "utils.h"

#include <iostream>

int main(int argc, char *argv[]) {
	std::cout.precision(2);
	std::cout.setf(std::ios::fixed,std::ios::floatfield);
    srand(time(NULL));

    Board *board=choose_game();

	Player *player_a=new PlayerHuman(PLAYER_1);
	Player *player_b=new PlayerBot(PLAYER_2);

	play_game(player_a,player_b,board);

    delete board;
	delete player_a;
	delete player_b;

	return 0;
}
