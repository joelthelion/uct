#ifndef __BOARDBLOCKS__
#define __BOARDBLOCKS__

#include "board.h"
#include <queue>
#include <set>

enum Color {VIOLET,BLUE,ORANGE,GREEN,YELLOW,RED,NONE};

class MoveBlocks : public Move {
friend class BoardBlocks;
friend class Pixel;
public:
	MoveBlocks(Token player,Color color);

    void print() const override;

    Move *deepcopy() const override;

    bool compare (const Move& move) const override;

protected:
    Color color;
};

class BoardBlocks : public Board {
friend class QBoardBlocks;
public:
	BoardBlocks(Size width,Size height,bool init);

    ~BoardBlocks() override;

    Board *deepcopy() const override;

    Move *parse_move_string(Token player,const char *string) const override;

    void print() const override;
	inline bool is_move_valid(const Move &move) const override;
	inline bool is_move_valid(const MoveBlocks &move) const;

    Moves get_possible_moves(Token player) const override; //FIXME not sure about constness
    void play_move(const Move &move) override;

    bool play_random_move(Token player) override;

    Token check_for_win() const override;

    struct TokenBlocks {
        void print() const;
        Color color;
        Token player;
        bool playable;
        void print_char() const;
        Size i,j;
    };

    const TokenBlocks& get_const_token(Size i, Size j) const;
    int get_p1score() const;
    int get_p2score() const;
protected:
    void update_playable();

private:
    typedef std::pair<int,BoardBlocks::TokenBlocks*> Seed;

    struct SeedGreater {
        bool operator()(const Seed &a, const Seed &b);
    };

    typedef std::priority_queue<Seed,std::vector<Seed>,SeedGreater> SeedsQueue;
    typedef std::set<TokenBlocks*> TokenBlocksSet;

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
