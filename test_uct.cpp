#include "uct.h"
#include "boardc4.h"

#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[]) {
    srand(time(0));

    Node *root=new Node(MoveC4(PLAYER_1,0));
    Board *board=new BoardC4();

    for (int k=0; k<1000; k++) {
        Board *copy=board->deepcopy();

        Token winner=root->play_random_game(copy);

        //copy->print();
        //if (winner==NOT_PLAYED) std::cout<<"draw"<<std::endl;
        //else std::cout<<"player "<<winner<<" won"<<std::endl;
        //root->print_tree();
        //std::cout<<std::endl;

        delete copy;
    }
    //root->print_tree();
    //std::cout<<std::endl;

    root->print_best_branch_down();
    std::cout<<std::endl;

    delete root;

	return 0;
}
