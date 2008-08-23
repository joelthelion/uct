#include <QApplication>
#include <QMainWindow>

#include "qboardc4.h"

int main(int argc,char *argv[]) {
	QApplication app(argc,argv);

	QBoardC4 widget;
	widget.show();

	widget.board->play_move(MoveC4(PLAYER_1,3));
	widget.board->play_move(MoveC4(PLAYER_2,3));

	return app.exec();
}

