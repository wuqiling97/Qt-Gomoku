#include "chessboard.h"
#include <QDebug>
#include <QtMath>

int ChessBoard::margin = 20;
int ChessBoard::sidelength = 432 - 2*margin;
int ChessBoard::spacing = sidelength / (gamesize-1);//28

ChessBoard::ChessBoard(QWidget *parent) : QWidget(parent)
{
//    qDebug()<<"chess board initialize";
//    qDebug("width = %d, height = %d", width(), height());
//    qDebug("sidelength = %d, spacing = %d", sidelength, spacing);
    bomb.load(":/images/bomb.png");
    boardbackground.load(":/images/board.jpg");
    blackchess.load(":/images/blackchess.png");
    whitechess.load(":/images/whitechess.png");
}

inline int ChessBoard::toIndex(const int& pos)
{
    int ret = int(round(double(pos - margin) / double(spacing)));
    return ret;
}

ChessBoard::PointState ChessBoard::colortostate(const QColor &color)
{
    if(color == Qt::white)
        return white;
    if(color == Qt::black)
        return black;

    qDebug()<<"color to state error!!!";
    return no;
}

bool ChessBoard::isIndexValid(const int &index)
{
    return (index>=0 && index<gamesize);
}

void ChessBoard::initialize(QString side)
{
    if(side == "black")
    {
        mycolor = Qt::black;
        enemycolor = Qt::white;
        inRound = true;
    }
    if(side == "white")
    {
        mycolor = Qt::white;
        enemycolor = Qt::black;
        inRound = false;
    }
}

void ChessBoard::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
//    p.setBrush(QColor(242,184,27));
    p.setPen(Qt::NoPen);
    p.drawImage(QRect(0, 0, width(), height()), boardbackground);

    //平移到棋盘格子左上角
    p.translate(margin, margin);

    //********************************************
//    p.setPen(Qt::black);
//    for(int i=0; i<gamesize; ++i)
//    {
//        p.drawText(QPoint(i*spacing, -5), QString::number(i+1));
//        p.drawText(QPoint(-10, i*spacing + 10), QString::number(i+1));
//    }
    //**********************************************

    //画棋盘
    p.setPen(QPen(Qt::black));
    for(int i=0; i<gamesize; ++i)
    {
        p.drawLine(spacing * i, 0, spacing * i, sidelength);
        p.drawLine(0, spacing * i, sidelength, spacing * i);
    }
    p.setPen(QPen(Qt::black, 5));
    p.drawPoint(3*spacing, 3*spacing);
    p.drawPoint(3*spacing, 11*spacing);
    p.drawPoint(11*spacing, 3*spacing);
    p.drawPoint(11*spacing, 11*spacing);
    p.drawPoint(7*spacing, 7*spacing);
    //画棋子
//    p.setPen(QPen(Qt::black));
//    for(int i=0; i<chessList.size(); ++i)
//    {
//        const Chess& chess = chessList[i];
//        p.setBrush(QBrush(chess.color));
//        p.drawEllipse(QPoint(chess.x(), chess.y()), 12, 12);
//    }
    p.setPen(Qt::NoPen);
    for(int i=0; i<chessList.size(); ++i)
    {
        const Chess& chess = chessList[i];
        if(i%2 == 0)
            p.drawImage(QRect(chess.x()-12, chess.y()-12, 24, 24), blackchess);
        else
            p.drawImage(QRect(chess.x()-12, chess.y()-12, 24, 24), whitechess);
    }

    //危险提醒
    if(dangermode)
    {
        p.setBrush(QBrush(bomb));
        int size = 20;
        for(int i=0; i<dangerList.size(); ++i)
        {
            const Chess& chess = dangerList[i];
            p.drawImage(QRect(chess.x()-size, chess.y()-size-5, 2*size, 2*size), bomb);
        }
    }

    QWidget::paintEvent(event);
}

void ChessBoard::mousePressEvent(QMouseEvent *event)
{
    if(!inRound)
    {
        qDebug()<<"it's not your turn";
        return;
    }
    if(event->button() == Qt::LeftButton)
    {
        int row = toIndex(event->y());
        int col = toIndex(event->x());
//        qDebug("row = %d, col = %d", row, col);
        //add chess
        if(state[row][col] == no && isIndexValid(row) && isIndexValid(col))
        {
            state[row][col] = colortostate(mycolor);
            chessList.append(Chess(row, col, mycolor));
            inRound = false;
            qDebug()<<"opponent's turn";
            QString info = QString("add %1 %2 ").arg(QString::number(row), QString::number(col));
            emit sendChessInfo(info);
            if(isWin(row, col))
            {
                emit Win();
            }

            dangermode = false;
            this->update();
        }
    }
    QWidget::mousePressEvent(event);
}

void ChessBoard::mouseMoveEvent(QMouseEvent *event)
{
//    if(!inRound)
//        return;
    int xm = event->x();
    int ym = event->y();
    int row = toIndex(ym);
    int col = toIndex(xm);
    if(!isIndexValid(row) || !isIndexValid(col))
        return;

    int xg = col * spacing + margin;
    int yg = row * spacing + margin;
    double dis = sqrt((xm-xg) * (xm-xg) + (ym-yg) * (ym-yg));
    if(dis < 10)
        setCursor(Qt::PointingHandCursor);
    else
        setCursor(Qt::ArrowCursor);
}

bool ChessBoard::isWin(int row, int col)
{
    PointState mystate = colortostate(mycolor);
    int length = 1;
    int i, j;
    for(i=row-1; i>=0; --i) {
        if(state[i][col] != mystate)
            break;
        length++;
    }
    for(i=row+1; i<gamesize; ++i) {
        if(state[i][col] != mystate)
            break;
        length++;
    }
    if(length >= 5)
        return true;

    length = 1;
    for(j=col-1; j>=0; --j) {
        if(state[row][j] != mystate)
            break;
        length++;
    }
    for(j=col+1; j<gamesize; ++j) {
        if(state[row][j] != mystate)
            break;
        length++;
    }
    if(length >= 5)
        return true;

    length = 1;
    for(i=row-1, j=col-1; i>=0 && j>=0; --i, --j) {
        if(state[i][j] != mystate)
            break;
        length++;
    }
    for(i=row+1, j=col+1; i<gamesize && j<gamesize; ++i, ++j) {
        if(state[i][j] != mystate)
            break;
        length++;
    }
    if(length >= 5)
        return true;

    length = 1;
    for(i=row-1, j=col+1; i>=0 && j<gamesize; --i, ++j) {
        if(state[i][j] != mystate)
            break;
        length++;
    }
    for(i=row+1, j=col-1; i<gamesize && j>=0; ++i, --j) {
        if(state[i][j] != mystate)
            break;
        length++;
    }
    if(length >= 5)
        return true;

    return false;
}

void ChessBoard::addEnemyChess(int row, int col)
{
    if(state[row][col] == no)
    {
        state[row][col] = colortostate(enemycolor);
        chessList.append(Chess(row, col, enemycolor));
        inRound = true;
        qDebug()<<"my turn";

        this->update();
    }
    else
    {
        qDebug("add enemy chess error!\nrow %d, col %d", row, col);
    }
}

void ChessBoard::clear()
{
    chessList.clear();
    for(int i=0; i<gamesize; ++i)
        for(int j=0; j<gamesize; ++j)
        {
            state[i][j] = no;
        }
    dangermode = false;
    this->update();
}

void ChessBoard::pointoutDanger()
{
    dangermode = true;
    dangerList.clear();
    PointState bigstate[gamesize+2][gamesize+2] = {{no}};
    PointState opnstate = colortostate(enemycolor);
    PointState mystate = colortostate(mycolor);
    //创建一个较大的棋盘,外圈都是我方棋子,方便判断
    for(int i=0; i<gamesize+2; ++i)
        bigstate[i][gamesize+1] = bigstate[i][0] = mystate;
    for(int j=0; j<gamesize+2; ++j)
        bigstate[0][j] = bigstate[gamesize+1][j] = mystate;
    for(int i=0; i<gamesize; ++i)
        for(int j=0; j<gamesize; ++j)
            bigstate[i+1][j+1] = state[i][j];

    int live = 2;//双向是否活
    int empty = 0;//最多空1个子,表示空了几个子
    int count;
    int huoqi = 0;//有几个方向的活棋
    int i,j;
    for(int row=1; row<gamesize; ++row)
        for(int col=1; col<gamesize; ++col)
        {
            if(bigstate[row][col] == no) {
                huoqi = 0;
                //纵向
                live = 2; empty = 0; count = 1;
                for(i=row-1; i>=0; --i) {
                    if(empty == 2)
                        break;
                    if(bigstate[i][col] == no) {
                        if(bigstate[i-1][col] == opnstate) {
                            empty++;
                            count--;//empty 不能算入count中
                        }
                        else
                            break;
                    }
                    if(bigstate[i][col] == mystate) {
                        live--;
                        break;
                    }
                    count++;
                }
                for(i=row+1; i<gamesize; ++i) {
                    if(empty == 2)
                        break;
                    if(bigstate[i][col] == no) {
                        if(bigstate[i+1][col] == opnstate) {
                            empty++;
                            count--;//empty 不能算入count中
                        }
                        else
                            break;
                    }
                    if(bigstate[i][col] == mystate) {
                        live--;
                        break;
                    }
                    count++;
                }
                if((live==2 && count>=3) && empty<2) {
                    huoqi++;
                    qDebug("活三 row %d col %d", row, col);
                }
                if((live==1 && count>=4) && empty<2) {
                    huoqi++;
                    qDebug("冲四 row %d col %d", row, col);
                }

                //横向
                live = 2; empty = 0; count = 1;
                for(j=col-1; j>=0; --j) {
                    if(empty == 2)
                        break;
                    if(bigstate[row][j] == no) {
                        if(bigstate[row][j-1] == opnstate) {
                            empty++;
                            count--;//empty 不能算入count中
                        }
                        else
                            break;
                    }
                    if(bigstate[row][j] == mystate) {
                        live--;
                        break;
                    }
                    count++;
                }
                for(j=col+1; j<gamesize; ++j) {
                    if(empty == 2)
                        break;
                    if(bigstate[row][j] == no) {
                        if(bigstate[row][j+1] == opnstate) {
                            empty++;
                            count--;//empty 不能算入count中
                        }
                        else
                            break;
                    }
                    if(bigstate[row][j] == mystate) {
                        live--;
                        break;
                    }
                    count++;
                }
                if(((live==2 && count>=3) || (live==1 && count>=4)) && empty<2) {
                    huoqi++;
//                    qDebug("活棋 row %d col %d", row, col);
                }

                //主对角线
                live = 2; empty = 0; count = 1;
                for(i=row-1, j=col-1; i>=0, j>=0; --i, --j) {
                    if(empty == 2)
                        break;
                    if(bigstate[i][j] == no) {
                        if(bigstate[i-1][j-1] == opnstate) {
                            empty++;
                            count--;//empty 不能算入count中
                        }
                        else
                            break;
                    }
                    if(bigstate[i][j] == mystate) {
                        live--;
                        break;
                    }
                    count++;
                }
                for(i=row+1, j=col+1; i<gamesize, j<gamesize; ++i, ++j) {
                    if(empty == 2)
                        break;
                    if(bigstate[i][j] == no) {
                        if(bigstate[i+1][j+1] == opnstate) {
                            empty++;
                            count--;//empty 不能算入count中
                        }
                        else
                            break;
                    }
                    if(bigstate[i][j] == mystate) {
                        live--;
                        break;
                    }
                    count++;
                }
                if(((live==2 && count>=3) || (live==1 && count>=4)) && empty<2) {
                    huoqi++;
//                    qDebug("活棋 row %d col %d", row, col);
                }

                //副对角线
                live = 2; empty = 0; count = 1;
                for(i=row-1, j=col+1; i>=0, j<gamesize; --i, ++j) {
                    if(empty == 2)
                        break;
                    if(bigstate[i][j] == no) {
                        if(bigstate[i-1][j+1] == opnstate) {
                            empty++;
                            count--;//empty 不能算入count中
                        }
                        else
                            break;
                    }
                    if(bigstate[i][j] == mystate) {
                        live--;
                        break;
                    }
                    count++;
                }
                for(i=row+1, j=col-1; i<gamesize, j>=0; ++i, --j) {
                    if(empty == 2)
                        break;
                    if(bigstate[i][j] == no) {
                        if(bigstate[i+1][j-1] == opnstate) {
                            empty++;
                            count--;//empty 不能算入count中
                        }
                        else
                            break;
                    }
                    if(bigstate[i][j] == mystate) {
                        live--;
                        break;
                    }
                    count++;
                }
                if(((live==2 && count>=3) || (live==1 && count>=4)) && empty<2) {
                    huoqi++;
//                    qDebug("活棋 row %d col %d", row, col);
                }

                if(huoqi>=2)
                {
                    dangerList.append(Chess(row-1, col-1, Qt::red));
                }
            }
        }
    this->update();
    qDebug()<<"remind";
}
