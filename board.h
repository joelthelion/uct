#ifndef __BOARD__
#define __BOARD__

class Move {
public:
    virtual void print() const =0;
}

class Board {
public:
	typedef ::Move Move;
	typedef std::vector<Move> Moves;

	virtual void print() const =0;
	virtual bool is_move_valid(Move move) const =0;
	virtual Moves get_possible_moves() const =0; //FIXME not sure about constness
	virtual void play_move(Move move) =0;
	virtual void play_random_move(Move move) =0;
	virtual bool check_for_win() const =0;
};

#endif
