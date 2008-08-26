#ifndef __QGAMEWINDOW__
#define __QGAMEWINDOW__

#include <QMainWindow>
#include <QGroupBox>
#include <QSlider>
#include <QMutex>
#include <QLabel>
#include <QPushButton>
#include "qboardc4.h"
#include "qgamethread.h"
#include "board.h"
#include "player.h"

class QPlayer : public QGroupBox {
Q_OBJECT
public:
    QPlayer(Token player);

    Player *get_player() const;

protected slots:
	void update_desc();

private:
    QSlider time,iter;
    Token player;
	QLabel desc;
};
    
class QGameWindow : public QMainWindow {
Q_OBJECT
public:
    QGameWindow();
    ~QGameWindow();

protected slots:
    void start_game();
    void game_ended();

private:
    QMutex mutex;
    QPushButton start;
    QLabel winner_label;
    QPlayer qplayer_a,qplayer_b;
    QBoardC4 *board_widget;
    Player *player_a,*player_b;
	Board *board;

    QGameThread *thread;
};




#endif

