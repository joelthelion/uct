#include "player.h"
#include "utils.h"

#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[]) {
	std::cout.precision(2);
	std::cout.setf(std::ios::fixed,std::ios::floatfield);
    srand(static_cast<unsigned int>(time(nullptr)));

    Game game=OTHELLO;
    if (argc>1) game=parse_game(argv[1]);
    Board *board=choose_game(game);

    float bottime = static_cast<float>(1.);
    if (argc>2) bottime = parse_float(argv[2],bottime);

	Player *player_a;
	Player *player_b;
    if (argc>3) {
        player_a=new PlayerBot(PLAYER_1,bottime,0,0.3);
        player_b=new PlayerHuman(PLAYER_2);
    } else {
        player_a=new PlayerHuman(PLAYER_1);
        player_b=new PlayerBot(PLAYER_2,bottime,0,0.3);
    }

	play_game(player_a,player_b,board);

    delete board;
	delete player_a;
	delete player_b;

	return 0;
}
