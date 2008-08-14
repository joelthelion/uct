#include "board.h"
#include "boardc4.h"

int main(int argc, char *argv[]) {
	Board *board=new BoardC4;

	board->print();

	return 0;
}
