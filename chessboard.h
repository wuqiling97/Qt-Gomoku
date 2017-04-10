#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QColor>
#include <QImage>
#include <QMouseEvent>

#include <QString>
#include <QVector>

class ChessBoard : public QWidget
{
    Q_OBJECT
public:
    enum PointState{no, white, black};
    struct Chess
    {
        int row, col;
        QColor color;
        Chess() {}
        Chess(const int& rr, const int& cc, const QColor& coco)
        {
            row = rr; col = cc; color = coco;
        }
        int x() const
        {
            return (col * spacing);
        }

        int y() const
        {
            return (row * spacing);
        }
    };
    explicit ChessBoard(QWidget *parent = 0);
    bool inRound = false;
    QColor mycolor;
    QColor enemycolor;
    void initialize(QString side);

protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

private:
    static const int gamesize = 15;
    static int margin, spacing, sidelength;
    PointState state[gamesize][gamesize] = {{no}};
    QVector<Chess> chessList;
    QVector<Chess> dangerList;
    bool dangermode = false;
    QImage bomb, boardbackground, blackchess, whitechess;

    bool isWin(int row, int col);
    inline int toIndex(const int& pos);
    PointState colortostate(const QColor& color);
    bool isIndexValid(const int& index);

signals:
    void sendChessInfo(QString);
    void Win();

public slots:
    void addEnemyChess(int row, int col);
    void pointoutDanger();
    void clear();//重新开始游戏
};

#endif // CHESSBOARD_H
