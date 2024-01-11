#ifndef CKERNEL_H
#define CKERNEL_H

#include <QObject>
#include "mymeetingdialog.h"
#include <QtDebug>
class CKernel : public QObject
{
    Q_OBJECT
public:
    explicit CKernel(QObject *parent = nullptr);

    //单例模式
    static CKernel* GetInstance()
    {
        qDebug()<<__func__;
        static CKernel kernel;
        return &kernel;
    }

signals:

public slots:
    //用于回收的槽函数
    void slot_destroy();

private:
    MyMeetingDialog * m_pMyMeetingDlg;


};

#endif // CKERNEL_H
