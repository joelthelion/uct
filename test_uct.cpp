#include "utils.h"
#include "boardc4.h"
#include "player.h"

#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[]) {
	std::cout.precision(2);
	std::cout.setf(std::ios::fixed,std::ios::floatfield);
    srand(time(NULL));

    Board *board=new BoardC4();

	Player *player_a=new PlayerBot(PLAYER_1,1,100);
	Player *player_b=new PlayerBot(PLAYER_2,3,100);

    play_game(player_a,player_b,board,6);

    delete board;
    delete player_a;
    delete player_b;

	return 0;
}
