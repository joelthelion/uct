#include <QApplication>
#include "qgamewindow.h"

int main(int argc,char *argv[]) {
	QApplication app(argc,argv);

    QGameWindow widget;
    widget.show();

	return app.exec();
}

