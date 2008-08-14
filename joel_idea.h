//Much simpler and just as good because the nested classes are not really linked in practice,
//so they add a lot of syntax overhead and don't simplify anything
//
//Pierre there is a lot of overengineering in your version
#include <string>
#include <sstream>

class Move
{
	public:
	virtual std::string repr() const=0;
};

class Board
{
	public:
	virtual std::string repr() const=0;
	virtual void play_move(Move *)=0;
	virtual void play_random_move()=0;
	virtual bool check_for_win_at_move()=0;
};

typedef enum {NOT_PLAYED,PLAYER_1,PLAYER_2} C4_Token;
class C4_Move(public Move)
{
	public:
	C4_Move(int x, int y) : x(x),y(y);
	virtual std::string print_self() const
	{
		std::ostringstream out_stream
		out_stream << player << ":("<<x<<","<<"y"<<")";
		return out_stream.str()
	}
	protected:
	int x,y;
	C4_Token player;
};

class C4_Board
{
	public:
	C4_Board(int width,int height) : width(width), height(height)
	{
		tokens = new Token[width*height];
	}
	virtual std::string repr() const;
	virtual void play_move(Move * move)
	{
		C4_Move * c4move=dynamic_cast<C4_Move*>move;
		tokens[c4move->x*width+c4move->y]=c4move->player;
	}
	virtual void play_random_move();
	virtual bool check_for_win_at_move();

	protected:
	Token *tokens;
	int width,height;
};


