#include "board.h"
#include "boardc4.h"

#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[]) {
    srand(time(0));

	Board *board=new BoardC4;
    board->play_random_game();
	delete board;

	return 0;
}
