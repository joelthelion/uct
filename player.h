#ifndef __PLAYER__
#define __PLAYER__

#include <string>
#include "board.h"
#include "uct.h"
#include "common.h"

class Player {
public:
	Player(const std::string &name,Token player);

	virtual void print() const;
	Token get_player() const;
	virtual Move *get_move(const Board *board) const =0;

protected:
	std::string name;
	Token player;
};

class PlayerBot : public Player {
public:
	PlayerBot(Token player,double max_sec=1.5,int max_iteration=50e3);

	virtual Move *get_move(const Board *board) const;

private:
	double max_sec;
	int max_iteration;
};

class PlayerHuman : public Player {
public:
	PlayerHuman(Token player);

	virtual Move *get_move(const Board *board) const;
};

#endif
