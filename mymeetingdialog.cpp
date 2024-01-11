#include "mymeetingdialog.h"
#include "ui_mymeetingdialog.h"
#include <QMessageBox>
#include <QtDebug>
MyMeetingDialog::MyMeetingDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MyMeetingDialog)
{
    ui->setupUi(this);
}

MyMeetingDialog::~MyMeetingDialog()
{
    qDebug()<<__func__;
    delete ui;
}

void MyMeetingDialog::closeEvent(QCloseEvent *event)
{
    qDebug()<<__func__;
    if(QMessageBox::question(this,"提示","是否退出？")==QMessageBox::Yes)
    {
        //需要关闭
        Q_EMIT SIG_close();
        event->accept();//关闭
    }
    else
    {
        //不需要关闭
        event->ignore();//忽略
    }
}

