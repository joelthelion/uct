#ifndef __UCT__
#define __UCT__

#include <list>
#include "common.h"
#include "board.h"

class Node;

typedef std::list<Node*> Nodes;
typedef std::list<const Node*> ConstNodes;

class Node {
public:
    Node(Value uct_constant); //root constructor
    Node(const Move *move,Value uct_constant,Node *father=NULL);
    ~Node();

    void print() const;
    void print_tree(int level=0,int maxlevel=-1) const;
    void print_best_branch_down() const;
    void print_branch_up() const;

    const Node *get_best_child() const;
    Token play_random_game(Board *board,Token player);

	Value get_winning_probability() const;
    Value get_score() const;
    Value get_uct_constant() const;
	Mode get_mode() const;
	const Move *get_move() const;
    Count get_nb() const;
	Node * advance_and_detach(const Move * move);

protected:
    static void print_branch(const ConstNodes &branch);

    ConstNodes get_best_branch_down() const;
    ConstNodes get_branch_up() const;

    void update_father(Value value);
    void propagate_winning_to_granpa();
	void propagate_loosing_to_daddy();
    void recompute_inheritance();
    void tell_granpa_dad_is_a_looser(const Node *dad);
	Value uct_constant;

private:
    Node *father;
    //Nodes good_children;
    Nodes children;
    Moves unexplored_moves;

    Count nb;
    Value value;
    Value simulation_value;

    Mode mode;
    const Move *move;
};

#endif
