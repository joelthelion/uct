#include "uct.h"
#include "boardc4.h"

#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[]) {
    srand(time(0));

    Node *root=new Node(MoveC4(PLAYER_1,0));
    root->print();
    delete root;

	return 0;
}
