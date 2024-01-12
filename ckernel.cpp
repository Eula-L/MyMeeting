#include "ckernel.h"
#include <QDebug>

//设置协议映射关系
void CKernel::setNetPackMap()
{
    memset(m_netPackMap,0,sizeof(m_netPackMap));
    m_netPackMap[DEF_PACK_LOGIN_RS-DEF_PACK_BASE] = &CKernel::slot_dealRegisterRs;
}

CKernel::CKernel(QObject *parent) : QObject(parent)
{
    qDebug()<<__func__;
    m_pMyMeetingDlg = new MyMeetingDialog;
    m_pMyMeetingDlg->show();

    connect(m_pMyMeetingDlg,SIGNAL(SIG_close()),
            this,SLOT(slot_destroy()));
    //添加网络
    m_pClient = new TcpClientMediator;
    m_pClient ->OpenNet(_DEF_SERVERIP,_DEF_PORT);

    connect(m_pClient,SIGNAL(SIG_ReadyData(uint,char*,int)),
            this,SLOT(slot_dealData(uint,char*,int)));
}


//回收
void CKernel::slot_destroy()
{
    qDebug()<<__func__;
    if(m_pMyMeetingDlg)
    {
        m_pMyMeetingDlg->hide();//先将窗口隐藏
        delete m_pMyMeetingDlg;
        m_pMyMeetingDlg = NULL;
    }
}
//登录回复处理
void CKernel::slot_dealLoginRs(uint sock, char *buf, int nLen)
{

}
//注册回复处理
void CKernel::slot_dealRegisterRs(uint sock, char *buf, int nLen)
{

}
