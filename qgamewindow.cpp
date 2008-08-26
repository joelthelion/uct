#include "qgamewindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

QPlayer::QPlayer(Token player) : QGroupBox(), player(player), time(Qt::Horizontal), iter(Qt::Horizontal) {
    if (player==PLAYER_1) setTitle("bot 1 settings");
    else setTitle("bot 2 settings");

    QVBoxLayout *layout=new QVBoxLayout(this);

    time.setMinimum(1.0);
    time.setMaximum(15.0);
    time.setValue(1.0);
    layout->addWidget(&time);

    iter.setMinimum(0.0);
    iter.setMaximum(100.0);
    iter.setValue(0.0);
    layout->addWidget(&iter);

    setLayout(layout);
    setMinimumWidth(200);
}

Player *QPlayer::get_player() const {
    return new PlayerBot(player,time.value(),iter.value()*1e3);
}


QGameWindow::QGameWindow() : QMainWindow(), winner_label(), start("start game"), mutex(), board_widget(&mutex), qplayer_a(PLAYER_1), qplayer_b(PLAYER_2), player_a(NULL), player_b(NULL), thread(NULL) {
    QWidget *center=new QWidget;
    QHBoxLayout *layout=new QHBoxLayout;
    QVBoxLayout *layout_player=new QVBoxLayout;

    layout->addWidget(&board_widget);
    layout_player->addWidget(&qplayer_a);
    layout_player->addWidget(&qplayer_b);
    layout_player->addWidget(&start);
    layout_player->addWidget(&winner_label);
    layout_player->addStretch(1);
    layout->addLayout(layout_player);

    center->setLayout(layout);
    setCentralWidget(center);

    connect(&start,SIGNAL(clicked()),this,SLOT(start_game()));
}

void QGameWindow::start_game() {
    assert(not player_a);
    assert(not qplayer_b);
    assert(not thread);

    board_widget.reset_board();

    player_a=qplayer_a.get_player();
    player_b=qplayer_b.get_player();

    thread=new QGameThread(&mutex,player_a,player_b,board_widget.board);

    connect(thread,SIGNAL(move_played()),&board_widget,SLOT(update()));
    connect(thread,SIGNAL(finished()),this,SLOT(game_ended()));

    thread->start();

    start.setEnabled(false);
}

void QGameWindow::game_ended() {
    assert(thread->isFinished());

    if (thread->winner==NULL) winner_label.setText("draw");
    else if (thread->winner==player_a) winner_label.setText("bot 1 wins!!!");
    else if (thread->winner==player_b) winner_label.setText("bot 2 wins!!!");

    delete thread;
    delete player_a;
    delete player_b;

    thread=NULL;
    player_a=player_b=NULL;

    start.setEnabled(true);
}

QGameWindow::~QGameWindow() {
    if (thread) {
        if (thread->isRunning()) {
            thread->set_abort(true);
            thread->wait();
        }
        delete thread;
    }
    if (player_a) delete player_a;
    if (player_b) delete player_b;
}

