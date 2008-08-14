#include "board.h"
#include "boardc4.h"

int main(int argc, char *argv[]) {
	Board *board=new BoardC4;

	board->play_move(MoveC4(board,PLAYER_1,1));
	board->play_move(MoveC4(board,PLAYER_2,1));
	board->play_move(MoveC4(board,PLAYER_2,6));
	board->print();

	Token player;
	
	player=PLAYER_1;
	while (true) {
		MoveC4 move(board,player,2);

		if (board->is_move_valid(move)) board->play_move(move);
		else break;

		player=other_player(player);
	}
	board->print();

	player=PLAYER_1;
	while (board->play_random_move(player)) player=other_player(player);
	board->print();

	delete board;
	return 0;
}
