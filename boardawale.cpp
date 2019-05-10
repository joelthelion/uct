#include "boardawale.h"

#include <iostream>
#include <sstream>
#include <cassert>
#include <cstdlib>

MoveAwale::MoveAwale(Token player,int position)
		: Move(player), position(position)
{
}

void
MoveAwale::print() const
{
		if (player==NOT_PLAYED)
		{
				std::cout<<"awale null move";
				return;
		}
		std::cout<<"player "<<player<<" spreads slot "<<position;
}

Move*
MoveAwale::deepcopy() const
{
		Move* copy=new MoveAwale(player,position);
		return copy;
}

bool
MoveAwale::compare(const Move& abstract_move) const
{
		const auto & move=dynamic_cast<const MoveAwale&>(abstract_move);
		return Move::compare(abstract_move) and position==move.position;
}



BoardAwale::BoardAwale()
		: player1_score(0), player2_score(0), round_counter(0)
{
		for (int kk=0; kk<6; kk++)
		{
				player1_slots[kk] = 4;
				player2_slots[kk] = 4;
		}
}

BoardAwale::~BoardAwale()
= default;

Board*
BoardAwale::deepcopy() const
{
		BoardAwale* copy = new BoardAwale();
		copy->player1_score = player1_score;
		copy->player2_score = player2_score;
		for (int kk=0; kk<6; kk++)
		{
				copy->player1_slots[kk] = player1_slots[kk];
				copy->player2_slots[kk] = player2_slots[kk];
		}
		copy->round_counter = round_counter;
		return copy;
}

Move*
BoardAwale::parse_move_string(Token player,const char *string) const
{
		std::stringstream stream(std::stringstream::in | std::stringstream::out);
		int position=-1;

		stream<<string;
		stream>>position;

		if (stream.fail()) return NULL;

		Move* move=new MoveAwale(player,position);

		if (is_move_valid(*move)) return move;

		delete move;
		return NULL;
}

int
BoardAwale::format_slot(int count)
{
		if (count==0) return ' ';
		if (count<10) return '0'+count;
		if (count<33) return 'a'+count-10;
		return '?';
}

void
BoardAwale::print() const
{
		std::cout << "    5 4 3 2 1 0" << std::endl;
		std::cout << "   ╔═╤═╤═╤═╤═╤═╗" << std::endl;

		std::cout << "p2 ║" << format_slot(player2_slots[5]);
		for (int kk=4; kk>=0; kk--)
				std::cout << "│" << format_slot(player2_slots[kk]);
		std::cout << "║ " << player2_score << std::endl;

		std::cout << "   ╠═╪═╪═╪═╪═╪═╣" << std::endl;

		std::cout << "p1 ║" << format_slot(player1_slots[0]);
		for (int kk=1; kk<6; kk++)
				std::cout << "│" << format_slot(player1_slots[kk]);
		std::cout << "║ " << player1_score << std::endl;

		std::cout << "   ╚═╧═╧═╧═╧═╧═╝" << std::endl;
		std::cout << "    0 1 2 3 4 5" << std::endl;
}

bool
BoardAwale::is_move_valid(const Move& abstract_move) const
{
		return is_move_valid(dynamic_cast<const MoveAwale&>(abstract_move));
}

bool
BoardAwale::is_move_valid(const MoveAwale& move) const
{
		if (move.player==NOT_PLAYED) return false;
		if (move.position<0) return false;
		if (move.position>5) return false;

		const int* slots = player1_slots;
		const int* other_slots = player2_slots;
		if (move.player==PLAYER_2)
		{
				slots = player2_slots;
				other_slots = player1_slots;
		}

		if (slots[move.position]<=0) return false;

		int other_total = 0;
		for (int kk=0; kk<6; kk++)
				other_total += other_slots[kk];

		if (other_total!=0) return true;

		if (move.position + slots[move.position] >= 6) return true;

		return false;
}

Moves
BoardAwale::get_possible_moves(Token player) const
{
		Moves moves;

		if (round_counter>128)
				return moves;

		for (int kk=0; kk<6; kk++)
		{
				MoveAwale* move = new MoveAwale(player, kk);
				if (!is_move_valid(*move))
				{
						delete move;
						continue;
				}

				moves.push_back(move);
		}

		return moves;
}

void
BoardAwale::play_move(const Move &abstract_move)
{
		const MoveAwale &move=dynamic_cast<const MoveAwale&>(abstract_move);

		assert(this->is_move_valid(move));

		int* slots = player1_slots;
		int* other_slots = player2_slots;
		if (move.player==PLAYER_2)
		{
				slots = player2_slots;
				other_slots = player1_slots;
		}

		int position = move.position;
		int count = slots[position];
		Token player = move.player;
		slots[position] = 0;

		while (count>0)
		{
				// advance position
				position++;
				while (position>5)
				{
						std::swap(slots, other_slots);
						player = other_player(player);
						position -= 6;
				}

				// skip start position
				if (position==move.position and player==move.player)
						continue;

				// distribute seed
				slots[position]++;
				count--;
		}

		if (player==move.player) return;

		int* score = &player1_score;
		if (move.player==PLAYER_2) score = &player2_score;

		// collect seeds
		while (position>=0 and (slots[position]==2 or slots[position]==3))
		{
				*score += slots[position];
				slots[position] = 0;
				position--;
		}

		round_counter++;
}

bool
BoardAwale::play_random_move(Token player)
{
		//FIXME could be more efficient
		Moves possible_moves=get_possible_moves(player);

		if (possible_moves.empty()) return false;

		int selected= static_cast<int>(rand() / (RAND_MAX + 1.0) * possible_moves.size());
		Moves::const_iterator selected_iter=possible_moves.begin();
		while (selected>0) {
				selected--;
				selected_iter++;
		}
		play_move(**selected_iter);

		return true;
}

Token
BoardAwale::check_for_win() const
{
		if (player1_score>24) return PLAYER_1;
		if (player2_score>24) return PLAYER_2;

		if (48-player1_score-player2_score<6)
		{
				if (player1_score>player2_score) return PLAYER_1;
				if (player1_score<player2_score) return PLAYER_2;
		}

		return NOT_PLAYED;
}

