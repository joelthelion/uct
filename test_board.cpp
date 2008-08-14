#include "board.h"
#include "boardc4.h"

int main(int argc, char *argv[]) {
	Board *board=new BoardC4;

	board->play_move(MoveC4(board,PLAYER_1,1));
	board->play_move(MoveC4(board,PLAYER_2,1));
	board->play_move(MoveC4(board,PLAYER_2,6));
	board->print();

	return 0;
}
