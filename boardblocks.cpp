#include "boardblocks.h"

#include <iostream>
#include <sstream>
#include <cassert>
#include <cstdlib>
#include <list>
using std::cout;
using std::endl;

MoveBlocks::MoveBlocks(Token player,Color color) : Move(player), color(color) {}

void MoveBlocks::print() const {
	if (player==NOT_PLAYED) cout<<"block null move";
    std::cout<<"color ";
    switch (color) {
    case VIOLET:
        cout<<"violet";
        break;
    case ORANGE:
        cout<<"orange";
        break;
    case BLUE:
        cout<<"blue";
        break;
    case GREEN:
        cout<<"green";
        break;
    case YELLOW:
        cout<<"yellow";
        break;
    case RED:
        cout<<"red";
        break;
    case NONE:
        cout<<"none";
        break;
    }
    cout<<" for player "<<player;
}

Move *MoveBlocks::deepcopy() const {
	Move *copy=new MoveBlocks(player,color);
	return copy;
}

bool MoveBlocks::compare (const Move& abstract_move) const {
    const auto &move=dynamic_cast<const MoveBlocks&>(abstract_move);
	return Move::compare(abstract_move) and color==move.color;
}

void BoardBlocks::TokenBlocks::print_char() const {
    //if (playable) {
    //    cout<<'x';
    //    return;
    //}

    std::string c;
    if (player==NOT_PLAYED) c = "█";
    else c="■";

    switch (color) {
    case VIOLET:
        c = "\e[35m"+c+"\e[0m";
        break;
    case BLUE:
        c = "\e[34m"+c+"\e[0m";
        break;
    case ORANGE:
        c = "\e[36m"+c+"\e[0m"; //This is actually Cyan
        break;
    case GREEN:
        c = "\e[32m"+c+"\e[0m";
        break;
    case YELLOW:
        c = "\e[33m"+c+"\e[0m";
        break;
    case RED:
        c = "\e[31m"+c+"\e[0m";
        break;
    case NONE:
        c = 'N';
        break;
    }

    cout<<c;
}

void BoardBlocks::TokenBlocks::print() const {
    cout<<"i="<<i<<" j="<<j<<" color="<<color<<" player="<<player;
}

BoardBlocks::BoardBlocks(Size width,Size height,bool init) : lastmove(PLAYER_2,NONE), width(width), height(height), size(width*height), p1score(1), p2score(1) {
	//allocate flat
	flat=new TokenBlocks[size];

    if (not init) return;

    assert(width%2==0 and height%2==0);
    srand(static_cast<unsigned int>(time(nullptr)));
	for (Size row=0; row<height; row++) for (Size column=0; column<width/2; column++) {
        Color color = static_cast<Color>(rand()%6); //FIXME hardcoded color number
        {
            TokenBlocks &current = get_token(row,column);
            current.i        = row;
            current.j        = column;
            current.playable = false;
            current.player   = NOT_PLAYED;
            current.color    = color;
        }
        color = static_cast<Color>(5-color); //FIXME hardcoded color number
        {
            TokenBlocks &current = get_token(height-1-row,width-1-column);
            current.i        = height-1-row;
            current.j        = width-1-column;
            current.playable = false;
            current.player   = NOT_PLAYED;
            current.color    = color;
        }

    }

    get_token(height-1,0).player = PLAYER_1;
    get_token(0,width-1).player  = PLAYER_2;
    update_playable();
}

void BoardBlocks::update_playable() {
    Token player = other_player(lastmove.player);

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
            if (current->color!=lastmove.color and current->player==NOT_PLAYED) current->playable=true;
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
    copy->lastmove.player = lastmove.player;
    copy->lastmove.color  = lastmove.color;

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
    Color c;
    switch (string[0]) {
    case 'r':
        c = RED;
        break;
    case 'b':
        c = BLUE;
        break;
    case 'y':
        c = YELLOW;
        break;
    case 'g':
        c = GREEN;
        break;
    case 'o':
        c = ORANGE;
        break;
    case 'v':
        c = VIOLET;
        break;
    default:
        return NULL;
        break;
    }

	Move *move=new MoveBlocks(player,c);

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

	for (Size row=0; row<height; row++) {
		std::cout<<(row%10)<<"|";
		for (Size column=0; column<width; column++) {
            const TokenBlocks &current = get_const_token(row,column);
            current.print_char();
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
    cout<<"p1 "<<p1score<<" ";
    cout<<"p2 "<<p2score<<endl;
}

bool BoardBlocks::is_move_valid(const Move &abstract_move) const {
	return is_move_valid(dynamic_cast<const MoveBlocks&>(abstract_move));
}

bool BoardBlocks::is_move_valid(const MoveBlocks &move) const {
    if (move.color==NONE or move.player!=other_player(lastmove.player)) return false;
    for (int k=0; k<size; k++) {
        const TokenBlocks &current = flat[k];
        if (current.playable and current.color==move.color) return true;
    }
    return false;
}

Moves BoardBlocks::get_possible_moves(Token player) const {
    typedef std::set<Color> Colors;
    Colors colors;
	
    for (int k=0; k<size; k++) {
        const TokenBlocks &current = flat[k];
        if (current.playable) colors.insert(current.color);
    }

	Moves moves;
    for (Colors::const_iterator i=colors.begin(); i!=colors.end(); i++) { moves.push_back(new MoveBlocks(player,*i)); }
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

    for (int k=0; k<size; k++) {
        TokenBlocks *token = &flat[k];
        if (token->playable and token->color==move.color) {
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

        assert(current->color==move.color and current->player==NOT_PLAYED);

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
        if (current.player==move.player) current.color = move.color;
    }

    lastmove.color  = move.color;
	lastmove.player = move.player;
    //cout<<"END OF PLAY MOVE"<<endl;
    update_playable();

    if (lastmove.player==PLAYER_2) {
        p1score = 0;
        p2score = 0;

        for (int k=0; k<size; k++) {
            const TokenBlocks &current = flat[k];
            if (current.player==PLAYER_1) p1score++;
            if (current.player==PLAYER_2) p2score++;
        }
    }

}

bool BoardBlocks::play_random_move(Token player) {
    Moves possible_moves=get_possible_moves(player);
    if (possible_moves.empty()) return false;

    int selected= static_cast<int>(rand() / (RAND_MAX + 1.0) * possible_moves.size());
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
    int n = 0;

    for (int k=0; k<size; k++) {
        const TokenBlocks &current = flat[k];
        if (current.playable) n++;
    }

	if (n!=0) return NOT_PLAYED;

    if (p1score>p2score) return PLAYER_1;
    else return PLAYER_2;
}

