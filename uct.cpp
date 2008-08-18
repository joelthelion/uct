#include "uct.h"

#include <iostream>
#include <cstdlib>

Node::Node(const Move &move,Node *father) :  move(move), father(father), nb(0), value(0), simulation_value(0), mode(NORMAL) {}

Node::~Node() {
    for (Nodes::iterator iter=children.begin(); iter!=children.end(); iter++) {
        Node *child=*iter;
        delete child;
    }
}

void Node::print() const {
    std::cout<<"[";
    move.print();

    std::cout<<","<<children.size()<<" children";

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

Node *Node::get_best_child() {
    if (children.empty()) {
        std::cout<<"no children";
        return NULL;
    }

    Value best_score=0;
    Node *best_child=NULL;

    for (Nodes::iterator iter=children.begin(); iter!=children.end(); iter++) {
        Node *child=*iter;

        if (child->mode==WINNER) return child;

        if (child->mode==NORMAL and (not best_child or best_score<child->value/child->nb)) {
            best_score=child->value/child->nb;
            best_child=child;
        }
    }

    if (best_child) return best_child;

    //no non-losing move, all move moves are marked loosing...
    std::cout<<"SEPUKU!!!"<<std::endl;
    return children[rand() % children.size()];
}

Token Node::play_random_game(Board *board) {
    return NOT_PLAYED;
}

void Node::print_branch(const Nodes &branch) {
}

Nodes Node::get_best_branch_down() const {
    return Nodes();
}

Nodes Node::get_branch_up() const {
    return Nodes();
}

void Node::update_father(Value value) {
    if (father) {
        father->nb++;
        father->value+=1.-value;
        father->update_father(1.-value);
    }
}

void Node::propagate_winning() {
}

void Node::recompute_inheritance() {
}

void Node::tell_granpa_dad_is_a_looser(Node *dad) {
}

