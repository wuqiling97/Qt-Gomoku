#include "connectdialog.h"
#include "ui_connectdialog.h"
#include <QRegExp>
#include <QMessageBox>
#include <QDebug>

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);
    mapper = new QSignalMapper(this);
    connect(ui->Button0, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    connect(ui->Button1, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    connect(ui->Button2, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    connect(ui->Button3, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    connect(ui->Button4, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    connect(ui->Button5, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    connect(ui->Button6, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    connect(ui->Button7, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    connect(ui->Button8, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    connect(ui->Button9, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    connect(ui->ButtonDot, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    connect(ui->ButtonClear, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    mapper->setMapping(ui->Button0, "0");
    mapper->setMapping(ui->Button1, "1");
    mapper->setMapping(ui->Button2, "2");
    mapper->setMapping(ui->Button3, "3");
    mapper->setMapping(ui->Button4, "4");
    mapper->setMapping(ui->Button5, "5");
    mapper->setMapping(ui->Button6, "6");
    mapper->setMapping(ui->Button7, "7");
    mapper->setMapping(ui->Button8, "8");
    mapper->setMapping(ui->Button9, "9");
    mapper->setMapping(ui->ButtonDot, ".");
    mapper->setMapping(ui->ButtonClear, "clear");
    connect(mapper, SIGNAL(mapped(QString)), this, SLOT(input(QString)));
}

ConnectDialog::~ConnectDialog()
{
    delete ui;
}

void ConnectDialog::accept()
{
    hostIP = ui->ipEdit->text();
    //验证ip是否合法
    QRegExp syntax("(((25[0-5]|2[0-4][0-9]|((1[0-9]{2})|([1-9]?[0-9])))\\.){3}(25[0-5]|2[0-4][0-9]|((1[0-9]{2})|([1-9]?[0-9]))))");
    if(!syntax.exactMatch(hostIP))
    {
        qDebug()<<"invalid IP";
        QMessageBox::warning(this, "warning", "无效IP，请再次输入", QMessageBox::Ok);
        return;
    }
    else
    {
        qDebug()<<hostIP;
    }
    QDialog::accept();
}

QString ConnectDialog::getHostIP()
{
    return hostIP;
}

void ConnectDialog::input(const QString &add)
{
    if(add == "clear")
        ui->ipEdit->setText("");
    else
        ui->ipEdit->setText(ui->ipEdit->text() + add);
}
