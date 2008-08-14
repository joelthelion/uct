#include <iostream>
#include <vector>

//abstract class definition
struct Move {
    virtual void print_move() const =0;
};

struct Board {
    typedef ::Move Move;

	virtual void print_board() const =0;

	Move *move;
};

//class implementation
struct MoveA: public Move {
    virtual void print_move() const { std::cout<<"move_a"<<std::endl; }
};

struct BoardA: public Board {
    typedef MoveA Move;

	BoardA() : Board() { this->move=new Move(); }
	~BoardA() { delete this->move; }
	virtual void print_board() const { std::cout<<"board_a"<<std::endl; }
};

struct BoardB: public Board {
	struct Move: public ::Move {
		virtual void print_move() const { std::cout<<"move_b"<<std::endl; }
	};

	BoardB() : Board() { this->move=new Move(); }
	~BoardB() { delete this->move; }
	virtual void print_board() const { std::cout<<"board_b"<<std::endl; }
};

//typedef
typedef std::vector<Board*> Boards;

//do the stuff
int main(int argc, char *argv[]) {
	Boards boards;
	boards.push_back(new BoardA());
	boards.push_back(new BoardB());

	for (Boards::const_iterator iter=boards.begin(); iter<boards.end(); iter++) {
		const Board *current=*iter;
		current->print_board();
		current->move->print_move();
	}

	for (Boards::iterator iter=boards.begin(); iter<boards.end(); iter++) {
		delete *iter;
	}

	return 0;
}
