#include "boardmatches.h"

#include <iostream>
#include <sstream>
#include <cassert>
#include <cstdlib>

MoveMatches::MoveMatches(Token player,int matches_taken) : Move(player), matches_taken(matches_taken) {}

void MoveMatches::print() const {
	if (player!=NOT_PLAYED) std::cout<<"player "<<player<<" take "<<matches_taken<<" matches";
	else std::cout<<"matches null move";
}

Move *MoveMatches::deepcopy() const {
	Move *copy=new MoveMatches(player,matches_taken);
	return copy;
}

bool MoveMatches::compare (const Move& abstract_move) const {
    const MoveMatches &move=dynamic_cast<const MoveMatches&>(abstract_move);
	return Move::compare(abstract_move) and matches_taken==move.matches_taken;
}




BoardMatches::BoardMatches(int matches_count) : matches_count(matches_count), last_player(NOT_PLAYED) {}

BoardMatches::~BoardMatches() {}

Board *BoardMatches::deepcopy() const {
    BoardMatches *copy=new BoardMatches(matches_count);
	copy->last_player = last_player;
    return copy;
}

Move *BoardMatches::parse_move_string(Token player,const char *string) const {
	std::stringstream stream(std::stringstream::in | std::stringstream::out);
	int matches_taken=-1;

	stream<<string;
	stream>>matches_taken;

	if (stream.fail()) return NULL;

	Move *move=new MoveMatches(player,matches_taken);

	if (is_move_valid(*move)) return move;

	delete move;
	return NULL;
}

void BoardMatches::print() const {
	std::cout << matches_count << " matches " << last_player << std::endl;
	if (matches_count<=0) return;

	for (int kk=0; kk<matches_count; kk++)
		std::cout << "\e[31m█\e[0m ";
	std::cout << std::endl;

	for (int ll=0; ll<2; ll++) {
		for (int kk=0; kk<matches_count; kk++)
			std::cout << "║ ";
		std::cout << std::endl;
	}
}

bool BoardMatches::is_move_valid(const Move &abstract_move) const {
	return is_move_valid(dynamic_cast<const MoveMatches&>(abstract_move));
}

bool BoardMatches::is_move_valid(const MoveMatches &move) const {
	return move.player!=NOT_PLAYED and move.matches_taken>=1 and move.matches_taken<=3 and move.matches_taken<=matches_count;
}

Moves BoardMatches::get_possible_moves(Token player) const {
	Moves moves;
	
	for (int kk=1; kk<=3; kk++)
		if (matches_count>=kk)
			moves.push_back(new MoveMatches(player,kk));

	return moves;
}

void BoardMatches::play_move(const Move &abstract_move) {
	const MoveMatches &move=dynamic_cast<const MoveMatches&>(abstract_move);

	assert(this->is_move_valid(move));

	matches_count -= move.matches_taken;
	last_player = move.player;
}

bool BoardMatches::play_random_move(Token player) {
	if (matches_count<=0) return false;

	//FIXME could be more efficient
	Moves possible_moves=get_possible_moves(player);

	int selected=rand()/(RAND_MAX + 1.0) * possible_moves.size();
	Moves::const_iterator selected_iter=possible_moves.begin();
	while (selected>0) {
		selected--;
		selected_iter++;
	}
	play_move(**selected_iter);

	return true;
}

Token BoardMatches::check_for_win() const {
	if (matches_count>0) return NOT_PLAYED;
	return other_player(last_player);
}

