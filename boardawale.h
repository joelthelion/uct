#ifndef __BOARDAWALE__
#define __BOARDAWALE__

#include "board.h"

class MoveAwale : public Move {
		friend class BoardAwale;
		public:
				MoveAwale(Token player,int position);

    void print() const override;

    Move* deepcopy() const override;

    bool compare(const Move& move) const override;

		private:
				int position;
};

class BoardAwale : public Board {
		public:
				BoardAwale();

    ~BoardAwale() override;

    Board* deepcopy() const override;

    Move* parse_move_string(Token player,const char *string) const override;

    void print() const override;
				inline bool is_move_valid(const Move& move) const override;
				inline bool is_move_valid(const MoveAwale& move) const;

    Moves get_possible_moves(Token player) const override;

    void play_move(const Move& move) override;

    bool play_random_move(Token player) override;

    Token check_for_win() const override;

		private:
				static int format_slot(int count);
				int player1_slots[6];
				int player2_slots[6];
				int player1_score;
				int player2_score;
				int round_counter;
};

#endif
