#include "createdialog.h"
#include "ui_createdialog.h"
#include <QHostAddress>
#include <QHostInfo>
#include <QDebug>

CreateDialog::CreateDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::CreateDialog)
{
    ui->setupUi(this);

    //get local ip
    QString localhostName = QHostInfo::localHostName();
    qDebug()<<localhostName;
    QHostInfo info = QHostInfo::fromName(localhostName);
    foreach(QHostAddress address, info.addresses())
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol) //是ipv4协议
        {
            localIP = address.toString();
            qDebug() << localIP;
            if(!localIP.contains("192.168."))
            {
                ui->ipEdit->setText(localIP);
            }
        }
    }
    if(info.addresses().isEmpty())
    {
        localIP = "127.0.0.1";
        ui->ipEdit->setText(localIP);
    }
}

CreateDialog::~CreateDialog()
{
    delete ui;
}

QString CreateDialog::getLocalIP()
{
    return localIP;
}
