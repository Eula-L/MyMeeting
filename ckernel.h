#ifndef CKERNEL_H
#define CKERNEL_H

#define NetPackMap(a) m_netPackMap[ a - DEF_PACK_BASE ]

#include <QObject>
#include "mymeetingdialog.h"
#include "TcpClientMediator.h"
#include "packdef.h"

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
    //网络信息处理
    void slot_dealData(uint sock,char* buf,int nLen);
    //登录回复
    void slot_dealLoginRs(uint sock,char* buf,int nLen);
    //注册回复
    void slot_dealRegisterRs(uint sock,char* buf,int nLen);


private:
    MyMeetingDialog * m_pMyMeetingDlg;
    INetMediator* m_pClient;

    //协议映射表
    PFUN m_netPackMap[DEF_PACK_COUNT];


};

#endif // CKERNEL_H
