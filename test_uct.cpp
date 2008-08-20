#include "uct.h"
#include "boardc4.h"

#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[]) {
	std::cout.precision(2);
	std::cout.setf(std::ios::fixed,std::ios::floatfield);
    srand(time(NULL));

    Node *root=new Node();
    Board *board=new BoardC4();
    //board->play_move(MoveC4(PLAYER_1,3));
    //board->play_move(MoveC4(PLAYER_1,2));
    //board->play_move(MoveC4(PLAYER_1,1));

    board->print();

	const double max_time=1.5;
	const int max_iteration=100000;

	clock_t start=clock(),end=clock();
	int k;
    for (k=0; k<max_iteration and root->mode==NORMAL and end-start<max_time*CLOCKS_PER_SEC; k++) {
        Board *copy=board->deepcopy();

        Token winner=root->play_random_game(copy,PLAYER_1);

        //copy->print();
        //if (winner==NOT_PLAYED) std::cout<<"draw"<<std::endl;
        //else std::cout<<"player "<<winner<<" won"<<std::endl;
        //root->print_tree();
        //std::cout<<std::endl;

        delete copy;

		end=clock();
    }
    const Move *move=root->get_best_child()->move;
	
	//debug report
    //root->print_tree();
    //std::cout<<std::endl;
    root->print_best_branch_down();
    std::cout<<std::endl;


	//simulation report
	std::cout<<"simulated "<<k<<" games in "<<float(end-start)/CLOCKS_PER_SEC<<"s"<<std::endl;

	//move report
    std::cout<<"playing ";
	switch (root->mode) {
	case NORMAL:
		std::cout<<"normal ";
		break;
	case WINNER:
		std::cout<<"loosing ";
		break;
	case LOOSER:
		std::cout<<"winning ";
		break;
	}
	std::cout<<"move ";
    move->print();
    std::cout<<std::endl;

	//actually play the move
    board->play_move(*move);
    board->print();

    delete root;
    delete board;

	return 0;
}
