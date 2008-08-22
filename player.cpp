#include "player.h"

#include <iostream> 
#include <cassert>
#include <ctime>

Player::Player(const std::string &name,Token player) : name(name), player(player) {
	assert(not player==NOT_PLAYED);
}

Token Player::get_player() const {
	return player;
}

void Player::print() const {
	std::cout<<name<<" "<<player;
}



PlayerBot::PlayerBot(Token player,double max_sec,int max_iteration) : Player("bot",player), max_sec(max_sec),max_iteration(max_iteration) {}

Move *PlayerBot::get_move(const Board *board) const {
    Node *root=new Node();

	clock_t start=clock(),end=clock();
	int k;

    for (k=0; k<max_iteration and root->get_mode()==NORMAL and end-start<max_sec*CLOCKS_PER_SEC; k++) {
        Board *copy=board->deepcopy();

        Token winner=root->play_random_game(copy,player);

        delete copy;

		end=clock();
    }

	const Node *best_child=root->get_best_child();
	if (not best_child) {
		delete root;
		return NULL;
	}


    const Move *move=best_child->get_move();

	//debug report
    //root->print_tree();
    //std::cout<<std::endl;
    //root->print_best_branch_down();
    //std::cout<<std::endl;

	//simulation report
	std::cout<<"simulated "<<k<<" games in "<<float(end-start)/CLOCKS_PER_SEC<<"s"<<std::endl;

	//move report
    std::cout<<"playing ";
	switch (root->get_mode()) {
	case NORMAL:
		std::cout<<"normal "<<best_child->get_winning_probability()<<" ";
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

	Move *copy=move->deepcopy();
	delete root;

	return copy;
}



PlayerHuman::PlayerHuman(Token player) : Player("human",player) {}

Move *PlayerHuman::get_move(const Board *board) const {
	Move *move=NULL;

	while (not move) {

		std::cout<<"choose move: ";
		char string[MAX_INPUT_LENGTH];
		std::cin.getline(string,MAX_INPUT_LENGTH);

		if (not std::cin.good()) {
			std::cout<<std::endl;
			break;
		}

		move=board->parse_move_string(player,string);
	}

	if (not move) return NULL;

	//move report
    std::cout<<"playing move ";
    move->print();
    std::cout<<std::endl;

	return move;
}

