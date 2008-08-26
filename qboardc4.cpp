#include "qboardc4.h"

#include <QPainter>

QBoardC4::QBoardC4(QMutex *mutex,int cell_size,Size width,Size height,Size win_length) : QWidget(), mutex(mutex),
board(new BoardC4(width,height,win_length)), cell_size(cell_size), border_size(20), cell_border(15), width(width), height(height), win_length(win_length) {
	setFixedSize(cell_size*width+2*border_size,cell_size*height+2*border_size);
}

QBoardC4::~QBoardC4() {
	delete board;
}

void QBoardC4::reset_board() {
    if (board->played_count) {
        delete board;
        board=new BoardC4(width,height,win_length);
        update();
    }
}

void QBoardC4::paintEvent(QPaintEvent * event) {
	Q_UNUSED(event);

	QPainter painter(this);

    mutex->lock();
    BoardC4 *local_board=dynamic_cast<BoardC4*>(board->deepcopy());
    mutex->unlock();

	//draw grid
	int min_x=border_size,max_x=border_size+local_board->width*cell_size;
	int min_y=border_size,max_y=border_size+local_board->height*cell_size;

	int x=min_x;
	for (int column=0; column<=local_board->width; column++) {
		painter.drawLine(x,min_y,x,max_y);
		x+=cell_size;
	}

	int y=min_y;
	for (int row=0; row<=local_board->height; row++) {
		painter.drawLine(min_x,y,max_x,y);
		y+=cell_size;
	}

	//draw_tokens
	x=min_x;
	for (int column=0; column<local_board->width; column++) {
		y=min_y;
		for (int row=0; row<local_board->height; row++) {
			switch (local_board->tokens[column][row]) {
			case PLAYER_1:
				painter.drawLine(x+cell_border,y+cell_border,x+cell_size-cell_border,y+cell_size-cell_border);
				painter.drawLine(x+cell_border,y+cell_size-cell_border,x+cell_size-cell_border,y+cell_border);
				break;
			case PLAYER_2:
				painter.drawEllipse(x+cell_border,y+cell_border,cell_size-2*cell_border,cell_size-2*cell_border);
				break;
			case NOT_PLAYED:
				break;
			}
			y+=cell_size;
		}
		x+=cell_size;
	}

    delete local_board;
}

