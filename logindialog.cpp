#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("注册&登录");
    ui->tw_page->setCurrentIndex(0);//默认显示登录
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::closeEvent(QCloseEvent *event)
{
    event->ignore();
    Q_EMIT SIG_close();
}
//登录界面清空
void LoginDialog::on_pb_clear_clicked()
{
    ui->le_tel->setText("");
    ui->le_password->setText("");
}

#include <QRegExp>//正则表达式
#include <QMessageBox>//弹窗提示
//提交登录信息
void LoginDialog::on_pb_commit_clicked()
{
    QString strTel = ui->le_tel->text();
    QString strPassword = ui->le_password->text();
    //校验处理
    //0、非空校验，清楚空格后，不能有空格
    QString tmpTel = strTel;
    QString tmpPwd = strPassword;
    if(tmpPwd.remove(" ").isEmpty()||tmpTel.remove(" ").isEmpty())
    {
        QMessageBox::about(this,"提示","手机号和密码不能为空格");
        return;
    }
    //1、校验手机号（正则）
    QRegExp regex("^1[3-9]\\d{9}$");
    bool res = regex.exactMatch(strTel);
    if(!res)
    {
        QMessageBox::about(this,"提示","手机号格式不正确");
        return;
    }
    //2、校验密码（长度6-20不能有空格）
    if(strPassword.length()<6||strPassword.length()>20)
    {
        QMessageBox::about(this,"提示","密码长度非法");
        return ;
    }

    //通过校验
    Q_EMIT SIG_loginCommit(strTel,strPassword);
}

//注册界面清空信息
void LoginDialog::on_pb_clear_register_clicked()
{
    ui->le_name_register->setText("");
    ui->le_tel_register->setText("");
    ui->le_password_register->setText("");
    ui->le_confirm_register->setText("");
}

//注册界面提交
void LoginDialog::on_pb_commit_register_clicked()
{
    QString strTel = ui->le_tel_register->text();
    QString strName = ui->le_name_register->text();
    QString strPassword = ui->le_password_register->text();
    QString strConfirm = ui->le_confirm_register->text();

    //TODO校验处理
    //非空校验，清楚空格后，不能有空格
    QString tmpTel = strTel;
    QString tmpName = strName;
    QString tmpPwd = strPassword;
    QString tmpConfirm = strConfirm;
    if(tmpTel.remove(" ").isEmpty()||tmpName.remove(" ").isEmpty()
            ||tmpPwd.remove(" ").isEmpty())
    {
        QMessageBox::about(this,"提示","手机号、昵称、密码不能为空格");
        return;
    }
    //1、校验手机号（正则）
    QRegExp regex("^1[3-9]\\d{9}$");
    bool res = regex.exactMatch(strTel);
    if(!res)
    {
        QMessageBox::about(this,"提示","手机号格式非法");
        return;
    }
    //2、校验密码（长度6-20不能有空格）
    if(strPassword.length()<6||strPassword.length()>20)
    {
        QMessageBox::about(this,"提示","密码长度非法");
        return ;
    }
    //3、校验密码和确认密码时候一致
    if(strPassword != strConfirm)
    {
        QMessageBox::about(this,"提示","两次密码不一致");
        return ;
    }
    //4、昵称长度不超过10
    if(strName.length()>=10)
    {
        QMessageBox::about(this,"提示","昵称长度过长");
        return ;
    }

    Q_EMIT SIG_registerCommit(strTel,strName,strPassword);
}



