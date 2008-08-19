#include "uct.h"
#include "boardc4.h"

#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[]) {
    srand(time(0));

    Node *root=new Node();
    Board *board=new BoardC4();
    board->play_move(MoveC4(PLAYER_1,3));
    board->play_move(MoveC4(PLAYER_1,2));
    board->play_move(MoveC4(PLAYER_1,1));

    for (int k=0; k<10000 and root->mode==NORMAL; k++) {
        Board *copy=board->deepcopy();

        Token winner=root->play_random_game(copy,PLAYER_1);

        //copy->print();
        //if (winner==NOT_PLAYED) std::cout<<"draw"<<std::endl;
        //else std::cout<<"player "<<winner<<" won"<<std::endl;
        //root->print_tree();
        //std::cout<<std::endl;

        delete copy;
    }
    root->print_tree();
    std::cout<<std::endl;

    root->print_best_branch_down();
    std::cout<<std::endl;

    board->print();
    const Move *move=root->get_best_child()->move;
    std::cout<<"playing ";
    move->print();
    std::cout<<std::endl;
    board->play_move(*move);
    board->print();


    delete root;
    delete board;

	return 0;
}
