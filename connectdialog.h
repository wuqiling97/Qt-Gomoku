#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>
#include <QSignalMapper>

namespace Ui {
class ConnectDialog;
}

class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectDialog(QWidget *parent = 0);
    ~ConnectDialog();
    QString getHostIP();

protected:
    void accept();

private:
    Ui::ConnectDialog *ui;
    QSignalMapper* mapper;
    QString hostIP;

private slots:
    void input(const QString& add);
};

#endif // CONNECTDIALOG_H
