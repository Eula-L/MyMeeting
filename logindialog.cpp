#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

//提交登录信息
void LoginDialog::on_pb_commit_clicked()
{
    QString strTel = ui->le_tel->text();
    QString strPassword = ui->le_password->text();
    //TODO校验处理

    Q_EMIT SIG_loginCommit(strTel,strPassword);
}

