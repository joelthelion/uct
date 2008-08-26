#include <QApplication>
#include <QMainWindow>

#include "qboardc4.h"
#include "qgamethread.h"

int main(int argc,char *argv[]) {
	QApplication app(argc,argv);

	QBoardC4 widget;
	widget.show();

	Player *player_a=new PlayerBot(PLAYER_1,1,10000);
	Player *player_b=new PlayerBot(PLAYER_2,3);

	QGameThread game(player_a,player_b,widget.board);
	QObject::connect(&game,SIGNAL(move_played()),&widget,SLOT(update()));

	//widget.board->play_move(MoveC4(PLAYER_1,3));
	//widget.board->play_move(MoveC4(PLAYER_2,3));

	game.start();

	return app.exec();
}

