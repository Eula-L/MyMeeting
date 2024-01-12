#include "ckernel.h"
#include <QDebug>

//设置协议映射关系
void CKernel::setNetPackMap()
{
    memset(m_netPackMap,0,sizeof(m_netPackMap));
    //m_netPackMap[DEF_PACK_LOGIN_RS-DEF_PACK_BASE] = &CKernel::slot_dealRegisterRs;//这样太长了，使用一个宏简化
    NetPackMap(DEF_PACK_LOGIN_RS)           = &CKernel::slot_dealLoginRs;
    NetPackMap(DEF_PACK_REGISTER_RS)        = &CKernel::slot_dealRegisterRs;
}

CKernel::CKernel(QObject *parent) : QObject(parent)
{
    qDebug()<<__func__;
    setNetPackMap();

    m_pMyMeetingDlg = new MyMeetingDialog;
    connect(m_pMyMeetingDlg,SIGNAL(SIG_close()),
            this,SLOT(slot_destroy()));

//    m_pMyMeetingDlg->show();
    m_pLoginDialog = new LoginDialog;
    connect(m_pLoginDialog,SIGNAL(SIG_loginCommit(QString,QString)),
            this,SLOT(slot_loginCommit(QString,QString)));
    m_pLoginDialog->show();


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

//发送登录信息
void CKernel::slot_loginCommit(QString tel, QString pwd)
{
    std::string strTel = tel.toStdString();
    std::string strPwd = pwd.toStdString();
    STRU_LOGIN_RQ rq;
    strcpy(rq.m_szUser,strTel.c_str());
    strcpy(rq.m_szPassword,strPwd.c_str());

    m_pClient->SendData(0,(char*)&rq,sizeof (rq));
}
//网络处理
void CKernel::slot_dealData(uint sock, char *buf, int nLen)
{
    int type = *(int*)buf;
    if(type >= DEF_PACK_BASE && type <= DEF_PACK_BASE + DEF_PACK_COUNT)
    {
        //取协议头，根据映射关系找到函数指针，调用函数
        PFUN pf = NetPackMap(type);
        if(pf)
        {
            (this->*pf)(sock,buf,nLen);
        }
    }
    delete[]buf;
}
//登录回复处理
void CKernel::slot_dealLoginRs(uint sock, char *buf, int nLen)
{
    qDebug()<<__func__;
}
//注册回复处理
void CKernel::slot_dealRegisterRs(uint sock, char *buf, int nLen)
{

}
