#ifndef __BOARDBLOCKS__
#define __BOARDBLOCKS__

#include "board.h"
#include <queue>
#include <set>

enum Color {VIOLET,BLUE,ORANGE,GREEN,YELLOW,RED,NONE};

class MoveBlocks : public Move {
friend class BoardBlocks;
public:
	MoveBlocks(Token player,Color color);

    virtual void print() const;
	virtual Move *deepcopy() const;
	virtual bool compare (const Move& move) const;

private:
    Color color;
};

class BoardBlocks : public Board {
friend class QBoardBlocks;
public:
	BoardBlocks(Size width,Size height,bool init);
	virtual ~BoardBlocks();

    virtual Board *deepcopy() const;
	virtual Move *parse_move_string(Token player,const char *string) const;
	virtual void print() const;
	inline virtual bool is_move_valid(const Move &move) const;
	inline bool is_move_valid(const MoveBlocks &move) const;
	virtual Moves get_possible_moves(Token player) const; //FIXME not sure about constness
	virtual void play_move(const Move &move);
	virtual bool play_random_move(Token player);
	virtual Token check_for_win() const;

protected:
    void update_playable();

private:
    struct TokenBlocks {
        void print() const;
        Color color;
        Token player;
        bool playable;
        void print_char() const;
        Size i,j;
    };

    typedef std::pair<int,BoardBlocks::TokenBlocks*> Seed;

    struct SeedGreater {
        bool operator()(const Seed &a, const Seed &b);
    };

    typedef std::priority_queue<Seed,std::vector<Seed>,SeedGreater> SeedsQueue;
    typedef std::set<TokenBlocks*> TokenBlocksSet;

    const TokenBlocks& get_const_token(Size i, Size j) const;
    TokenBlocks& get_token(Size i,Size j);

    MoveBlocks lastmove;

	const Size width;
	const Size height;
	const Size size;
	TokenBlocks *flat;
    int p1score;
    int p2score;
};

#endif
