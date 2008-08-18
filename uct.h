#ifndef __UCT__
#define __UCT__

#include <vector>
#include "common.h"
#include "board.h"

class Node;

typedef std::vector<Node*> Nodes; //FIXME should be a list be implementation is easyer with vector

class Node {
public:
    Node(const Move &move,Node *father=NULL);
    ~Node();

    void print() const;
    void print_tree(int level=0) const;
    void print_best_branch_down() const;
    void print_branch_up() const;

    Node *get_best_child();
    Token play_random_game(Board *board);

protected:
    static void print_branch(const Nodes &branch);

    Nodes get_best_branch_down() const;
    Nodes get_branch_up() const;

    void update_father(Value value);
    void propagate_winning();
    void recompute_inheritance();
    void tell_granpa_dad_is_a_looser(Node *dad);

private:
    Node *father;
    //Nodes good_children;
    Nodes children;

    Count nb;
    Value value;
    Value simulation_value;
    Mode mode;

    const Move &move;
};

#endif
