#include <QApplication>
#include <QMainWindow>

int main(int argc,char *argv[]) {
	QApplication app(argc,argv);

	QMainWindow widget;
	widget.show();

	return app.exec();
}

