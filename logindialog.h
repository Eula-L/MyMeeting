#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT
signals:
    void SIG_loginCommit(QString tel, QString password);
public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private slots:
    void on_pb_commit_clicked();

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
