#ifndef __BOARDAWALE__
#define __BOARDAWALE__

#include "board.h"

class MoveAwale : public Move {
		friend class BoardAwale;
		public:
				MoveAwale(Token player,int position);

				virtual void print() const;
				virtual Move* deepcopy() const;
				virtual bool compare(const Move& move) const;

		private:
				int position;
};

class BoardAwale : public Board {
		public:
				BoardAwale();
				virtual ~BoardAwale();

				virtual Board* deepcopy() const;
				virtual Move* parse_move_string(Token player,const char *string) const;
				virtual void print() const;
				inline virtual bool is_move_valid(const Move& move) const;
				inline bool is_move_valid(const MoveAwale& move) const;
				virtual Moves get_possible_moves(Token player) const;
				virtual void play_move(const Move& move);
				virtual bool play_random_move(Token player);
				virtual Token check_for_win() const;

		private:
				static char format_slot(int count);
				int player1_slots[6];
				int player2_slots[6];
				int player1_score;
				int player2_score;
				int round_counter;
};

#endif
