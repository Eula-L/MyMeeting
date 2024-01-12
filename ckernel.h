#ifndef CKERNEL_H
#define CKERNEL_H

#define NetPackMap(a) m_netPackMap[ a - DEF_PACK_BASE ]

#include <QObject>
#include "mymeetingdialog.h"
#include "TcpClientMediator.h"
#include "packdef.h"
#include "logindialog.h"

#include <QtDebug>

//协议映射表使用的类型
class CKernel;
typedef void (CKernel::*PFUN)(uint sock,char* buf,int nLen);

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

    void setNetPackMap();
    //用于回收的槽函数
    void slot_destroy();

    //发送登录信息
    void slot_loginCommit(QString tel,QString pwd);

    //网络信息处理
    void slot_dealData(uint sock,char* buf,int nLen);
    //登录回复
    void slot_dealLoginRs(uint sock,char* buf,int nLen);
    //注册回复
    void slot_dealRegisterRs(uint sock,char* buf,int nLen);



private:
    MyMeetingDialog * m_pMyMeetingDlg;//主窗口
    LoginDialog * m_pLoginDialog;//登录注册窗口


    INetMediator* m_pClient;

    //协议映射表
    PFUN m_netPackMap[DEF_PACK_COUNT];


};

#endif // CKERNEL_H
