#include "uct.h"

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>

Node::Node() : move(new Move()), father(NULL), nb(0), value(0), simulation_value(0), mode(NORMAL) {}
Node::Node(const Move *move,Node *father) :  move(move), father(father), nb(0), value(0), simulation_value(0), mode(NORMAL) {}

Node::~Node() {
    delete move;

    for (Nodes::iterator iter=children.begin(); iter!=children.end(); iter++) {
        Node *child=*iter;
        delete child;
    }
}

void Node::print() const {
    std::cout<<"[";
    move->print();

    std::cout<<","<<children.size()<<" children";
    std::cout<<","<<unexplored_moves.size()<<" unexplored";

    if (not father) std::cout<<",ROOT";

    std::cout<<",";
    switch (mode) {
    case NORMAL:
        std::cout<<"NORMAL";
        break;
    case WINNER:
        std::cout<<"WINNER";
        break;
    case LOOSER:
        std::cout<<"LOOSER";
        break;
    }

    std::cout<<",";
    if (nb>0) std::cout<<value<<","<<nb<<","<<value/nb;
    else std::cout<<nb;

    std::cout<<"]";
}

void Node::print_tree(int indent) const {
    for (int k=0; k<indent*2; k++) std::cout<<"-";
    print();
    std::cout<<std::endl;

    for (Nodes::const_iterator iter=children.begin(); iter!=children.end(); iter++) {
        const Node *child=*iter;
        child->print_tree(indent+1);
    }
}

void Node::print_best_branch_down() const {
    print_branch(get_best_branch_down());
}

void Node::print_branch_up() const {
    print_branch(get_branch_up());
}

Value Node::get_winning_probability() const {
	return value/nb;
}

const Node *Node::get_best_child() const {
    if (children.empty()) return NULL;

    Value best_score=0;
    const Node *best_child=NULL;

    for (Nodes::const_iterator iter=children.begin(); iter!=children.end(); iter++) {
        Node *child=*iter;

        if (child->mode==WINNER) return child;

        if (child->mode==NORMAL and (not best_child or best_score<child->value/child->nb)) {
            best_score=child->value/child->nb;
            best_child=child;
        }
    }

    if (best_child) return best_child;

    //no non-losing move, all move moves are marked loosing...
	assert(mode==WINNER); //if all child are loosing then this is a winner node
    std::cout<<"SEPUKU!!!"<<std::endl;
    return children[rand() % children.size()];
}

Token Node::play_random_game(Board *board,Token player) {
    const Value loose_value=0., draw_value=.5, win_value=1.;

    if (father) assert(player==other_player(move->player));
    else assert(move->player==NOT_PLAYED);

    assert(mode==NORMAL);
    
    if (father) board->play_move(*move); //root as no move

    if (father and board->check_for_win()) {
        //std::cout<<"win situation detected"<<std::endl;
        //move->print();
        //std::cout<<std::endl;

        propagate_winning();
        return move->player;
    }

    if (not nb) {
        Token winner;
        if (father) {
            unexplored_moves=board->get_possible_moves(player);
            winner=board->play_random_game(player);
        } else {
            unexplored_moves=board->get_possible_moves(player);
            winner=board->play_random_game(player);
        }

        assert(not value);
        if (winner==NOT_PLAYED) value=draw_value;
        else if (winner==move->player) value=win_value;
        else value=loose_value;
        simulation_value=value;

        nb=1;

        update_father(value);
        return winner;
    }

    if (not unexplored_moves.empty()) {
        Move *move=unexplored_moves.back();
        unexplored_moves.pop_back();

        Node *child=new Node(move,this);
        children.push_back(child);
        return child->play_random_game(board,other_player(player));
    }

    Value best_score=0;
    Node *best_child=NULL;
    for (Nodes::iterator iter=children.begin(); iter!=children.end(); iter++) {
        Node *child=*iter;

        if (not child->mode==LOOSER and (not best_child or best_score<child->value/child->nb+sqrtf(2.*logf(nb)/child->nb))) {
             best_score=child->value/child->nb+sqrtf(2.*logf(nb)/child->nb);
             best_child=child;
        }
    }

    if (not best_child) {
        std::cout<<"no child move possible"<<std::endl;
        value+=draw_value;
        nb++;
        return NOT_PLAYED;
    }

    return best_child->play_random_game(board,other_player(player));
}

void Node::print_branch(const ConstNodes &branch) {
    for (ConstNodes::const_iterator iter=branch.begin(); iter!=branch.end(); iter++) {
        const Node *node=*iter;
        node->print();
        //std::cout<<" ";
        std::cout<<std::endl;
    }
}

ConstNodes Node::get_best_branch_down() const {
    ConstNodes branch;
    const Node *current=this;

    while (current and not current->mode==WINNER) {
        branch.push_back(current);
        current=current->get_best_child();
    }

    return branch;
}

ConstNodes Node::get_branch_up() const {
    ConstNodes branch;
    const Node *current=this;

    while (current->father) {
        branch.push_back(current);
        current=current->father;
    }

    return branch;
}

void Node::update_father(Value value) {
    if (father) {
        father->nb++;
        father->value+=1.-value;
        father->update_father(1.-value);
    }
}

void Node::propagate_winning() {
    mode=WINNER;
    
    if (father) {
        father->mode=LOOSER;
        if (father->father) {
            father->father->tell_granpa_dad_is_a_looser(father);
        }
    }
}

void Node::recompute_inheritance() {
    nb=1;
    value=simulation_value;
    for (Nodes::const_iterator iter=children.begin(); iter!=children.end(); iter++) {
        const Node *child=*iter;

        if (not child->mode==LOOSER) {
            nb+=child->nb;
            value+=child->nb-child->value;
        }
    }

    if (father) father->recompute_inheritance();
}

void Node::tell_granpa_dad_is_a_looser(const Node *dad) {
    Count new_nb=1;
    Value new_value=simulation_value;
    for (Nodes::const_iterator iter=children.begin(); iter!=children.end(); iter++) {
        const Node *child=*iter;

        if (not child->mode==LOOSER) {
            new_nb+=child->nb;
            new_value+=child->nb-child->value;
        }
    }

    if (new_nb==1) { //all child are loosers
        propagate_winning();
    } else {
        nb=new_nb;
        value=new_value;

        if (father) {
            father->recompute_inheritance();
        }
    }
}

