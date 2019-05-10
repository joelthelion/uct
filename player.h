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
	virtual Move *get_move(const Board *board,const Move * last_move) =0;
	virtual ~Player() = default;

protected:
	std::string name;
	Token player;
};

class PlayerBot : public Player {
public:
	explicit PlayerBot(Token player,double max_sec=1.5,int max_iteration=0,Value uct_constant= static_cast<Value>(1.));

    Move *get_move(const Board *board,const Move * last_move) override;

    ~PlayerBot() override;

    void print() const override;

private:
	double max_sec;
	int max_iteration;
	Node * root;
};

class PlayerHuman : public Player {
public:
	explicit PlayerHuman(Token player);

    Move *get_move(const Board *board,const Move * last_move) override;
};

#endif
