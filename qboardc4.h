#ifndef __QBOARDC4__
#define __QBOARDC4__

#include <QWidget>
#include <QPaintEvent>
#include "boardc4.h"

class QBoardC4 : public QWidget {
Q_OBJECT
public:
	QBoardC4(QWidget *parent=0,int cell_size=100,Size width=7,Size height=5,Size win_length=4);
	~QBoardC4();

	BoardC4 *board;

protected:
	virtual void paintEvent(QPaintEvent * event);

private:
	int cell_size;
	int cell_border;
	int border_size;
};


#endif

