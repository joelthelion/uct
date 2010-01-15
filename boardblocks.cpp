#include "boardblocks.h"

#include <iostream>
#include <sstream>
#include <cassert>
#include <cstdlib>
#include <list>
#include <iomanip>
using std::cout;
using std::endl;

MoveBlocks::MoveBlocks(Token player,Size line,Size column) : Move(player), line(line), column(column) {}

void MoveBlocks::print() const {
	if (player!=NOT_PLAYED) std::cout<<"line "<<this->line<<" column "<<this->column<<" for player "<<player;
	else std::cout<<"blocks null move";
}

Move *MoveBlocks::deepcopy() const {
	Move *copy=new MoveBlocks(player,line,column);
	return copy;
}

bool MoveBlocks::compare (const Move& abstract_move) const {
    const MoveBlocks &move=dynamic_cast<const MoveBlocks&>(abstract_move);
	return Move::compare(abstract_move) and column==move.column and line==move.line;
}

void BoardBlocks::TokenBlocks::print_char() const {
    //if (playable) {
    //    cout<<'x';
    //    return;
    //}

    char c = ' ';
    switch (color) {
    case VIOLET:
        c = 'v';
        break;
    case BLUE:
        c = 'b';
        break;
    case ORANGE:
        c = 'o';
        break;
    case GREEN:
        c = 'g';
        break;
    case YELLOW:
        c = 'y';
        break;
    case RED:
        c = 'r';
        break;
    case NONE:
        c = 'n';
        break;
    }

    if (player!=NOT_PLAYED) c = toupper(c);
    cout<<c;
}

void BoardBlocks::TokenBlocks::print() const {
    cout<<"i="<<i<<" j="<<j<<" color="<<color<<" player="<<player;
}

BoardBlocks::BoardBlocks(Size width,Size height,bool init) : lastplayer(PLAYER_2), width(width), height(height), size(width*height), lastcolor(TokenBlocks::NONE) {
	//allocate flat
	flat=new TokenBlocks[size];

    if (not init) return;

    srand(time(NULL));
	for (Size row=0; row<height; row++) for (Size column=0; column<width; column++) {
        TokenBlocks &current = get_token(row,column);
        current.i        = row;
        current.j        = column;
        current.playable = false;
        current.player   = NOT_PLAYED;
        current.color    = static_cast<TokenBlocks::Color>(rand()%6); //FIXME hardcoded color number
        //current.color    = TokenBlocks::ORANGE;
        //if (column==0) current.color = TokenBlocks::RED;
        //if (column==width-1) current.color = TokenBlocks::BLUE;
    }

    get_token(height-1,0).player = PLAYER_1;
    get_token(0,width-1).player  = PLAYER_2;
    update_playable();
}

void BoardBlocks::update_playable() {
    Token player = other_player(lastplayer);

    for (int k=0; k<size; k++) { flat[k].playable = false; }

    typedef std::list<TokenBlocks*> Tokens;
	for (Size row=0; row<height; row++) for (Size column=0; column<width; column++) {
        if (get_const_token(row,column).player != player) continue;

        Tokens neighbors;
        if (row>0) neighbors.push_back(&get_token(row-1,column));
        if (row<height-1) neighbors.push_back(&get_token(row+1,column));
        if (column>0) neighbors.push_back(&get_token(row,column-1));
        if (column<width-1) neighbors.push_back(&get_token(row,column+1));
        for (Tokens::iterator i=neighbors.begin(); i!=neighbors.end(); i++) {
            TokenBlocks *current = *i;
            if (current->color!=lastcolor and current->player==NOT_PLAYED) current->playable=true;
        }
    }
}

BoardBlocks::TokenBlocks& BoardBlocks::get_token(Size i, Size j) {
    assert(i>=0 and i<height and j>=0 and j<width);
    return flat[j*height+i];
}

const BoardBlocks::TokenBlocks& BoardBlocks::get_const_token(Size i, Size j) const {
    assert(i>=0 and i<height and j>=0 and j<width);
    return flat[j*height+i];
}

BoardBlocks::~BoardBlocks() {
	delete [] flat;
}

Board *BoardBlocks::deepcopy() const {
    BoardBlocks *copy=new BoardBlocks(width,height,false);

    //copy last move and played_count
    copy->lastplayer = lastplayer;
    copy->lastcolor  = lastcolor;

	//copy flat
    for (int k=0; k<size; k++) {
        TokenBlocks &dest = copy->flat[k];
        const TokenBlocks &orig = flat[k];
        dest.player = orig.player;
        dest.color  = orig.color;
        dest.i      = orig.i;
        dest.j      = orig.j;
        dest.playable = orig.playable;
    }

    return copy;
}

Move *BoardBlocks::parse_move_string(Token player,const char *string) const {
	std::stringstream stream(std::stringstream::in | std::stringstream::out);
	int line=-1,column=-1;

	stream<<string;
	stream>>line>>std::ws>>column;

	if (stream.fail()) return NULL;

	Move *move=new MoveBlocks(player,line,column);

	if (is_move_valid(*move)) return move;

	delete move;
	return NULL;
}

void BoardBlocks::print() const {
	std::cout<<"  ";
	for (Size column=0; column<width; column++) std::cout<<(column%10);
	std::cout<<std::endl;

	std::cout<<" +";
	for (Size column=0; column<width; column++) std::cout<<"-";
	std::cout<<"+"<<std::endl;

    float nplayer1 = 0;
    float nplayer2 = 0;
	for (Size row=0; row<height; row++) {
		std::cout<<(row%10)<<"|";
		for (Size column=0; column<width; column++) {
            const TokenBlocks &current = get_const_token(row,column);
            current.print_char();
            if (current.player==PLAYER_1) nplayer1++;
            if (current.player==PLAYER_2) nplayer2++;
		}
		std::cout<<"|"<<(row%10)<<std::endl;
	}

	std::cout<<" +";
	for (Size column=0; column<width; column++) std::cout<<"-";
	std::cout<<"+"<<std::endl;

	std::cout<<"  ";
	for (Size column=0; column<width; column++) std::cout<<(column%10);
	std::cout<<std::endl;

    cout<<endl;
    cout<<"p1 "<<std::fixed<<std::setprecision(0)<<100.*nplayer1/size<<"% ";
    cout<<"p2 "<<std::fixed<<std::setprecision(0)<<100.*nplayer2/size<<"%"<<endl;
}

bool BoardBlocks::is_move_valid(const Move &abstract_move) const {
	return is_move_valid(dynamic_cast<const MoveBlocks&>(abstract_move));
}

bool BoardBlocks::is_move_valid(const MoveBlocks &move) const {
    if (move.line<0 or move.line>height-1 or move.column<0 or move.column>width-1) return false;
    const TokenBlocks &current = get_const_token(move.line,move.column);
	return move.player==other_player(lastplayer) and current.color!=lastcolor and current.playable;
}

Moves BoardBlocks::get_possible_moves(Token player) const {
	Moves moves;
	
    for (int k=0; k<size; k++) {
        const TokenBlocks &current = flat[k];
        if (current.playable) moves.push_back(new MoveBlocks(player,current.i,current.j));
    }

	return moves;
}

bool BoardBlocks::SeedGreater::operator()(const Seed &a, const Seed &b) {
    return a.first > b.first;
}

void BoardBlocks::play_move(const Move &abstract_move) {
    typedef std::list<TokenBlocks*> Tokens;

	const MoveBlocks &move=dynamic_cast<const MoveBlocks&>(abstract_move);

	assert(this->is_move_valid(move));

    SeedsQueue queue;
    TokenBlocksSet won;

    TokenBlocks::Color color = get_const_token(move.line,move.column).color;
    for (int k=0; k<size; k++) {
        TokenBlocks *token = &flat[k];
        if (token->playable and token->color==color) {
            queue.push(std::make_pair(0,token));
            won.insert(token);
        }
    }

    while (not queue.empty()) {
        Seed current_seed = queue.top();
        queue.pop();
        
        const int distance = current_seed.first;
        const TokenBlocks *current = current_seed.second;
        //cout<<"TOP ";
        //current->print();
        //cout<<" distance="<<distance<<" queue_size="<<queue.size()<<endl;

        assert(current->color==color and current->player==NOT_PLAYED);

        Tokens neighbors;
        if (current->i>0) neighbors.push_back(&get_token(current->i-1,current->j));
        if (current->i<height-1) neighbors.push_back(&get_token(current->i+1,current->j));
        if (current->j>0) neighbors.push_back(&get_token(current->i,current->j-1));
        if (current->j<width-1) neighbors.push_back(&get_token(current->i,current->j+1));
        //cout<<neighbors.size()<<" NEIGHBORS"<<endl;
        for (Tokens::const_iterator i=neighbors.begin(); i!=neighbors.end(); i++) {
            TokenBlocks *neighbor = *i;
            bool not_in_won = (won.find(neighbor)==won.end());
            //cout<<"PUSHED ";
            //neighbor->print();
            //cout<<" "<<distance+1<<" "<<not_in_won<<endl;
            if (neighbor->color==current->color and neighbor->player==NOT_PLAYED and not_in_won) {
                queue.push(std::make_pair(distance+1,neighbor));
                won.insert(neighbor);
            }
        }
    }

    for (TokenBlocksSet::const_iterator i=won.begin(); i!=won.end(); i++) {
        (*i)->player = move.player;
    }

    for (int k=0; k<size; k++) {
        TokenBlocks &current = flat[k];
        if (current.player==move.player) current.color = color;
    }

    lastcolor  = color;
	lastplayer = move.player;
    //cout<<"END OF PLAY MOVE"<<endl;
    update_playable();
}

bool BoardBlocks::play_random_move(Token player) {
    Moves possible_moves=get_possible_moves(player);
    if (possible_moves.empty()) return false;

    int selected=rand()/(RAND_MAX + 1.0) * possible_moves.size();
    Moves::const_iterator selected_iter=possible_moves.begin();
    while (selected>0) {
        selected--;
        selected_iter++;
    }
    play_move(**selected_iter);

    //play_move(*selected);
    //Move *selected=possible_moves[rand()%possible_moves.size()];
    //play_move(*selected);

    for (Moves::iterator iter=possible_moves.begin(); iter!=possible_moves.end(); iter++) delete *iter;

    return true;
}

Token BoardBlocks::check_for_win() const {
    int p1score = 0;
    int p2score = 0;
    int n = 0;

    for (int k=0; k<size; k++) {
        const TokenBlocks &current = flat[k];
        if (current.player==PLAYER_1) p1score++;
        if (current.player==PLAYER_2) p2score++;
        if (current.playable) n++;
    }

	if (n!=0) return NOT_PLAYED;

    if (p1score>p2score) return PLAYER_1;
    else return PLAYER_2;
}

