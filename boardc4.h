#ifndef __BOARDC4__
#define __BOARDC4__

#include "board.h"

class MoveC4 : public Move {
friend class BoardC4;
public:
	MoveC4(Token player,Size column);

    void print() const override;

    Move *deepcopy() const override;

    bool compare (const Move& move) const override;

private:
    Size column;
};

class BoardC4 : public Board {
friend class QBoardC4;
public:
	explicit BoardC4(Size width=7,Size height=6,Size win_length=4);

    ~BoardC4() override;

    Board *deepcopy() const override;

    Move *parse_move_string(Token player,const char *string) const override;

    void print() const override;
	inline bool is_move_valid(const Move &move) const override;
	inline bool is_move_valid(const MoveC4 &move) const;

    Moves get_possible_moves(Token player) const override; //FIXME not sure about constness
    void play_move(const Move &move) override;

    bool play_random_move(Token player) override;

    Token check_for_win() const override;

protected:
	inline Size propagate(Size row,Size column,Size drow,Size dcolumn,Token player) const;

private:
	MoveC4 lastmove;

	Size width;
	Size height;
	Size win_length;
	Size size,played_count;
	Token *flat;
	Token **token_for_columns;
	Token **tokens;
};

#endif
