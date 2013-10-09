#ifndef __BOARDMATCHES__
#define __BOARDMATCHES__

#include "board.h"

class MoveMatches : public Move {
friend class BoardMatches;
public:
	MoveMatches(Token player,int matches_taken);

    virtual void print() const;
	virtual Move *deepcopy() const;
	virtual bool compare (const Move& move) const;

private:
	int matches_taken;
};

class BoardMatches : public Board {
public:
	BoardMatches(int matches_count=10);
	virtual ~BoardMatches();

    virtual Board *deepcopy() const;
	virtual Move *parse_move_string(Token player,const char *string) const;
	virtual void print() const;
	inline virtual bool is_move_valid(const Move &move) const;
	inline bool is_move_valid(const MoveMatches &move) const;
	virtual Moves get_possible_moves(Token player) const;
	virtual void play_move(const Move &move);
	virtual bool play_random_move(Token player);
	virtual Token check_for_win() const;

private:
	int matches_count;
	Token last_player;
};

#endif
