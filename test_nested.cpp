#include <iostream>
#include <vector>

//abstract class definition
struct Move {
    virtual void print_move() const =0;
};

typedef std::vector<Move*> Moves;

struct Board {
    typedef ::Move Move;

	virtual void print_board() const =0;
	virtual Moves get_moves() const=0;

	Move *move{};
};

typedef std::vector<Board*> Boards;

//class implementation
struct MoveA: public Move {
    void print_move() const override { std::cout<<"move_a"<<std::endl; }
};

struct BoardA: public Board {
    typedef MoveA Move;

	BoardA() : Board() { this->move=new Move(); }
	~BoardA() { delete this->move; }

    void print_board() const override { std::cout<<"board_a"<<std::endl; }

    Moves get_moves() const override {
		Moves moves;
		moves.push_back(this->move);
		moves.push_back(this->move);
		return moves;
	}
};

struct BoardB: public Board {
	struct Move: public ::Move {
        void print_move() const override { std::cout<<"move_b"<<std::endl; }
	};

	BoardB() : Board() { this->move=new Move(); }
	~BoardB() { delete this->move; }

    void print_board() const override { std::cout<<"board_b"<<std::endl; }

    Moves get_moves() const override {
		Moves moves;
		moves.push_back(this->move);
		moves.push_back(this->move);
		moves.push_back(this->move);
		return moves;
	}
};

//do the stuff
int main(int argc, char *argv[]) {
	Boards boards;
	boards.push_back(new BoardA());
	boards.push_back(new BoardB());

	for (Boards::const_iterator board_iter=boards.begin(); board_iter!=boards.end(); board_iter++) {
		const Board *board=*board_iter;

		board->print_board();

		Moves moves=board->get_moves();
		for (Moves::const_iterator move_iter=moves.begin(); move_iter!=moves.end(); move_iter++) {
			const Board::Move *move=*move_iter;

			move->print_move();
		}
	}

	for (auto iter=boards.begin(); iter<boards.end(); iter++) {
		delete *iter;
	}

	return 0;
}
