#ifndef __BOARDC5__
#define __BOARDC5__

#include "board.h"

class MoveC5 : public Move {
friend class BoardC5;
public:
	MoveC5(Token player,Size column,Size row);

    void print() const override;

    Move *deepcopy() const override;

    bool compare(const Move& move) const override;

private:
    Size column;
    Size row;
};

class BoardC5 : public Board {
public:
	explicit BoardC5(Size width=6,Size height=6,Size win_length=5);

    ~BoardC5() override;

    Board *deepcopy() const override;

    Move *parse_move_string(Token player,const char *string) const override;

    void print() const override;
	inline bool is_move_valid(const Move &move) const override;

    Moves get_possible_moves(Token player) const override;

    void play_move(const Move &move) override;

    bool play_random_move(Token player) override;

    Token check_for_win() const override;

protected:
	inline Size propagate(Size row,Size column,Size drow,Size dcolumn,Token player) const;

private:
	MoveC5 lastmove;

	Size width;
	Size height;
	Size win_length;
	Size size,played_count;
	Token *flat;
	Token **tokens;
};

#endif
