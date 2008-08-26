#ifndef __QGAMETHREAD__
#define __QGAMETHREAD__

#include <QThread>
#include <QMutex>
#include "board.h"
#include "player.h"

class QGameThread : public QThread {
Q_OBJECT
public:
   	QGameThread(QMutex *mutex,Player *player_a,Player *player_b,Board *board);

    void set_abort(bool new_abort);

	Player *winner;
protected:
	virtual void run();

signals:
	void move_played();

private:
	QMutex *mutex;
	bool abort;

	Player *player_a;
	Player *player_b;
	Board *board;
};

#endif
