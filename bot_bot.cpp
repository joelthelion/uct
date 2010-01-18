#include "player.h"
#include "utils.h"

#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[]) {
	std::cout.precision(2);
	std::cout.setf(std::ios::fixed,std::ios::floatfield);
    srand(time(NULL));

    Game game=OTHELLO;
    if (argc>1) game=parse_game(argv[1]);
    Board *board=choose_game(game);

    float p1time = 1.;
    float p2time = 1.;
    if (argc>2) p1time = parse_float(argv[2],p1time);
    if (argc>3) p2time = parse_float(argv[3],p2time);

	Player *player_a=new PlayerBot(PLAYER_1,p1time,0,0.2);
	Player *player_b=new PlayerBot(PLAYER_2,p2time,0,0.2);

	play_game(player_a,player_b,board);

    delete board;
	delete player_a;
	delete player_b;

	return 0;
}
