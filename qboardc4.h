#ifndef __QBOARDC4__
#define __QBOARDC4__

#include <QWidget>
#include <QMutex>
#include <QPaintEvent>
#include "boardc4.h"

class QBoardC4 : public QWidget {
Q_OBJECT
public:
	QBoardC4(QMutex *mutex,Board *board,int cell_size=100);

	Board *board;

protected:
	virtual void paintEvent(QPaintEvent * event);

private:
    QMutex *mutex;
	int cell_size;
	int cell_border;
	int border_size;
    Size width,height,win_length;
};


#endif

