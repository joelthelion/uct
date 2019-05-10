#ifndef __BOARDMATCHES__
#define __BOARDMATCHES__

#include "board.h"

class MoveMatches : public Move {
friend class BoardMatches;
public:
	MoveMatches(Token player,int matches_taken);

    void print() const override;

    Move *deepcopy() const override;

    bool compare (const Move& move) const override;

private:
	int matches_taken;
};

class BoardMatches : public Board {
public:
	explicit BoardMatches(int matches_count=10);

    ~BoardMatches() override;

    Board *deepcopy() const override;

    Move *parse_move_string(Token player,const char *string) const override;

    void print() const override;
	inline bool is_move_valid(const Move &move) const override;
	inline bool is_move_valid(const MoveMatches &move) const;

    Moves get_possible_moves(Token player) const override;

    void play_move(const Move &move) override;

    bool play_random_move(Token player) override;

    Token check_for_win() const override;

private:
	int matches_count;
	Token last_player;
};

#endif
