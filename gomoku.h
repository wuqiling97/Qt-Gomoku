#ifndef GOMOKU_H
#define GOMOKU_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>

namespace Ui {
class Gomoku;
}

class Gomoku : public QMainWindow
{
    Q_OBJECT

public:
    explicit Gomoku(QWidget *parent = 0);
    ~Gomoku();

private slots:
    void on_createButton_clicked();
    void on_connectButton_clicked();
    void readData();//读取发送过来的数据
    void sendChessInfo(QString info);
    void sendWin();
    void on_remindButton_clicked();

private:
    Ui::Gomoku *ui;
    QTcpServer* server = nullptr;
    QTcpSocket* socket = nullptr;
    QString localIP;
    quint16 port;
    QTimer* timer;

    QString colortostring(const QColor& color);
    void gameend();
    void gamestart(QString side);
};

#endif // GOMOKU_H
