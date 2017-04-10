#include "gomoku.h"
#include "ui_gomoku.h"
#include "createdialog.h"
#include "connectdialog.h"
#include "chessboard.h"
#include <QMessageBox>
#include <QStringList>
#include <QString>
#include <QTimer>

QString Gomoku::colortostring(const QColor &color)
{
    if(color == Qt::black)
        return "black";
    if(color == Qt::white)
        return "white";
    return "no such color";
}

Gomoku::Gomoku(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Gomoku)
{
    ui->setupUi(this);
    port = 55555;
    timer = new QTimer(this);
}

Gomoku::~Gomoku()
{
    delete ui;
}

void Gomoku::on_createButton_clicked()
{
    CreateDialog* dialog = new CreateDialog(this);
    if(dialog->exec() == QDialog::Accepted)
    {
        localIP = dialog->getLocalIP();
        QMessageBox* message = new QMessageBox
                (QMessageBox::NoIcon, "waiting", localIP + "\n等待连接", QMessageBox::Cancel, this);

        server = new QTcpServer(this);
        server->listen(QHostAddress::Any, port);

        //lambda expression //connect succeed
        connect(server, &QTcpServer::newConnection, this, [&]
        {
            ui->textBrowser->append("client connect in");
            socket = server->nextPendingConnection();
            message->close();
            delete message;

            gamestart("black");
            QMessageBox::information(this, "", "connect succeed", QMessageBox::Ok);
        }, Qt::UniqueConnection);

        //cancel connection
        if(message->exec() == QMessageBox::Cancel)
        {
            ui->textBrowser->append("server close by canceling");
            server->close();
            disconnect(server, 0, 0, 0);
            delete server;
            server = nullptr;
            delete message;
        }
    }

    delete dialog;
}

void Gomoku::on_connectButton_clicked()
{
    ConnectDialog* dialog = new ConnectDialog(this);
    if(dialog->exec() == QMessageBox::Accepted)
    {
        QString hostIP = dialog->getHostIP();
        socket = new QTcpSocket(this);
        socket->connectToHost(QHostAddress(hostIP), port);

        timer->start(5000);

        connect(socket, &QTcpSocket::connected, this, [&]
        {
            timer->stop();
            ui->textBrowser->append("连接成功");
            gamestart("white");
        });

        connect(timer, &QTimer::timeout, this, [&]
        {
            timer->stop();
            socket->close();
            ui->textBrowser->append("连接超时");
            disconnect(socket, 0, 0, 0);
            delete socket;
            socket = nullptr;
        });
    }
}


void Gomoku::on_remindButton_clicked()
{
    ui->chessboard->pointoutDanger();
}

void Gomoku::sendChessInfo(QString info)
{
    socket->write(info.toStdString().c_str());
    ui->remindButton->setEnabled(false);
    ui->cursideEdit->setText(colortostring(ui->chessboard->enemycolor));
}

void Gomoku::sendWin()
{
    socket->write("win ");
    QMessageBox::information(this, "You Win!", "你赢了", QMessageBox::Ok);
    gameend();
}

void Gomoku::readData()
{
    QString netdata = QString(socket->readAll());
    QStringList args = netdata.split(' ');
    ui->textBrowser->append("receive data:" + netdata);
    bool finished = false;
    while(!finished)
    {
        if(args[0] == "add")
        {
            //添加棋子
            ui->chessboard->addEnemyChess(args[1].toInt(), args[2].toInt());
            ui->cursideEdit->setText(colortostring(ui->chessboard->mycolor));
            ui->remindButton->setEnabled(true);
            if(!args[3].isEmpty())
            {
                auto temp = args;
                args.clear();
                for(int i=3; i<temp.size(); ++i)
                    args.append(temp[i]);
            }
            else
                finished = true;
        }
        if(args[0] == "win")
        {
            //you lose
            QMessageBox::information(this, "You Lose", "很遗憾，你输了", QMessageBox::Ok);
            gameend();
            if(!args[1].isEmpty())
            {
                auto temp = args;
                args.clear();
                for(int i=1; i<temp.size(); ++i)
                    args.append(temp[i]);
            }
            else
                finished = true;
        }
    }
}

void Gomoku::gameend()
{
    //开启新建,连接功能,关闭提醒
    ui->connectButton->setEnabled(true);
    ui->createButton->setEnabled(true);
    ui->remindButton->setEnabled(false);
    //关闭连接
    socket->close();
    ui->textBrowser->append("socket closed");
    disconnect(socket, 0, 0, 0);
    delete socket;
    socket = nullptr;
    if(server != nullptr)
    {
        server->close();
        ui->textBrowser->append("server close");
        disconnect(server, 0, 0, 0);
        delete server;
        server = nullptr;
    }
    disconnect(ui->chessboard, 0, 0 ,0);
}

void Gomoku::gamestart(QString side)
{
    //清空上回状态
    ui->chessboard->clear();
    ui->cursideEdit->clear();
    ui->yoursideEdit->clear();

    ui->createButton->setEnabled(false);
    ui->connectButton->setEnabled(false);
    ui->yoursideEdit->setText(side);
    ui->cursideEdit->setText("black");

    connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(ui->chessboard, SIGNAL(sendChessInfo(QString)), this, SLOT(sendChessInfo(QString)));
    connect(ui->chessboard, SIGNAL(Win()), this, SLOT(sendWin()));

    ui->chessboard->initialize(side);
    if(side == "black")
    {
        ui->remindButton->setEnabled(true);
    }
    if(side == "white")
    {
        ui->remindButton->setEnabled(false);
    }
}
