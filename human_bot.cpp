#include "player.h"
#include "utils.h"

#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[]) {
	std::cout.precision(2);
	std::cout.setf(std::ios::fixed,std::ios::floatfield);
    srand(time(NULL));

    Board *board=choose_game(BOARDC4);

	Player *player_a=new PlayerBot(PLAYER_2,15,100000000);
	Player *player_b=new PlayerHuman(PLAYER_1);

	play_game(player_a,player_b,board);

    delete board;
	delete player_a;
	delete player_b;

	return 0;
}
