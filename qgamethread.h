#ifndef __QGAMETHREAD__
#define __QGAMETHREAD__

#include <QThread>
#include <QMutex>
#include "board.h"
#include "player.h"

class QGameThread : public QThread {
Q_OBJECT
public:
   	QGameThread(Player *player_a,Player *player_b,Board *board);

	QMutex mutex;
	bool abort;

protected:
	virtual void run();

private:
	Player *player_a;
	Player *player_b;
	Player *winner;
	Board *board;
};

#endif
