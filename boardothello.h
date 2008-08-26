#ifndef __BOARDOTHELLO__
#define __BOARDOTHELLO__

#include "board.h"

class MoveOthello : public Move {
friend class BoardOthello;
public:
	MoveOthello(Token player); //cant play move
	MoveOthello(Token player,Size column,Size row);
	virtual bool compare(const Move& move) const;

    virtual void print() const;
	virtual Move *deepcopy() const;

private:
    Size column;
    Size row;
	bool can_play;
};

class BoardOthello : public Board {
public:
	BoardOthello(Size width=8,Size height=8);
	virtual ~BoardOthello();

    virtual Board *deepcopy() const;
	virtual Move *parse_move_string(Token player,const char *string) const;
	virtual void print() const;
	inline virtual bool is_move_valid(const Move &move) const;
	virtual Moves get_possible_moves(Token player) const;
	virtual void play_move(const Move &move);
	virtual bool play_random_move(Token player);
	virtual Token check_for_win() const;

protected:
	bool propagate(Token player, Size column, Size row, Size dcolumn, Size drow) const;
	bool can_play_normal_move_at(Token player, Size column, Size row) const;
	bool can_play_normal_moves(Token player) const;
	Size switch_tokens(Token player,Size column, Size row, Size dcolumn, Size drow);

private:
	Size player_1_score, player_2_score;
	Size cant_play_count;

	Size width;
	Size height;
	Size size,played_count;
	Token *flat;
	Token **tokens;
};

#endif
