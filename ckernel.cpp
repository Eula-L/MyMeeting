#include "ckernel.h"
#include <QDebug>
#include "md5.h"
#include <QMessageBox>
//设置协议映射关系
void CKernel::setNetPackMap()
{
    memset(m_netPackMap,0,sizeof(m_netPackMap));
    //m_netPackMap[DEF_PACK_LOGIN_RS-DEF_PACK_BASE] = &CKernel::slot_dealRegisterRs;//这样太长了，使用一个宏简化
    NetPackMap(DEF_PACK_LOGIN_RS)           = &CKernel::slot_dealLoginRs;
    NetPackMap(DEF_PACK_REGISTER_RS)        = &CKernel::slot_dealRegisterRs;
}
#include <QSettings>//管理配置文件
#include <QApplication>//用于获取程序的exe目录，绝对路径
#include <QFileInfo>//查看文件信息，用于判断一个路径下时候存在文件

void CKernel::initConfig()
{
    //设置一个默认值
    m_serverIp = _DEF_SERVERIP;
    //获取exe目录
    QString path = QCoreApplication::applicationDirPath()+="/config.ini";
    //根据目录
    //查看文件是否存在，存在加载，不存在创建并写入默认值
    QFileInfo info(path);
    if(info.exists())
    {
        qDebug()<<"存在";
        //存在
        QSettings setting(path,QSettings::IniFormat);
        //打开组
        setting.beginGroup("net");
        //读取
        auto strIP = setting.value("ip","");
        if(!strIP.toString().isEmpty())
        {
            m_serverIp = strIP.toString();
        }
        //关闭组
        setting.endGroup();
    }
    else
    {
        qDebug()<<"不存在";
        //不存在
        QSettings setting(path,QSettings::IniFormat);//创建
        //打开组
        setting.beginGroup("net");
        //设置
        setting.setValue("ip",m_serverIp);
        //关闭组
        setting.endGroup();
    }
    qDebug()<<"ip: "<<m_serverIp;
}

CKernel::CKernel(QObject *parent) : QObject(parent)
{
    qDebug()<<__func__;
    setNetPackMap();
    initConfig();

    m_pMyMeetingDlg = new MyMeetingDialog;
    m_pLoginDialog = new LoginDialog;
    m_pClient = new TcpClientMediator;
    //主界面-关闭事件
    connect(m_pMyMeetingDlg,SIGNAL(SIG_close()),
            this,SLOT(slot_destroy()));
    //登录界面-关闭事件
    connect(m_pLoginDialog,SIGNAL(SIG_close()),
            this,SLOT(slot_destroy()));

    connect(m_pLoginDialog,SIGNAL(SIG_loginCommit(QString,QString)),
            this,SLOT(slot_loginCommit(QString,QString)));
    connect(m_pLoginDialog,SIGNAL(SIG_registerCommit(QString,QString,QString)),
            this,SLOT(slot_registerCommit(QString,QString,QString)));

    //处理接收到的数据
    connect(m_pClient,SIGNAL(SIG_ReadyData(uint,char*,int)),
            this,SLOT(slot_dealData(uint,char*,int)));

    m_pLoginDialog->show();
    //添加网络
    m_pClient ->OpenNet(m_serverIp.toStdString().c_str(),_DEF_PORT);

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
    if(m_pLoginDialog)
    {
        m_pLoginDialog->hide();
        delete m_pLoginDialog;
        m_pLoginDialog = NULL;
    }
    //回收网络
    if(m_pClient)
    {
        m_pClient->CloseNet();
        delete m_pClient;
        m_pClient = NULL;
    }
    exit(0);
}
#define MD5_KEY (1234)
//创建一个获取md5密文的静态方法
static std::string getMD5(QString value)
{
    QString str  = QString("%1_%2").arg(value).arg(MD5_KEY);
    std::string strRes = str.toStdString();
    MD5 md5(strRes);
    return md5.toString();
}

//发送登录信息
void CKernel::slot_loginCommit(QString tel, QString pwd)
{
    std::string strTel = tel.toStdString();
    std::string PwdMD5 = getMD5(pwd);

    STRU_LOGIN_RQ rq;
    strcpy(rq.m_tel,strTel.c_str());
    strcpy(rq.m_password,PwdMD5.c_str());

    m_pClient->SendData(0,(char*)&rq,sizeof (rq));
}
//发送注册信息
void CKernel::slot_registerCommit(QString tel, QString name, QString pwd)
{
    std::string strTel = tel.toStdString();
    std::string PwdMD5 = getMD5(pwd);
    //中文
    //兼容中文QString->std::string->char*
    std::string strName = name.toStdString();//utf8

    STRU_REGISTER_RQ rq;
    strcpy(rq.m_tel,strTel.c_str());
    strcpy(rq.m_password,PwdMD5.c_str());
    strcpy(rq.m_name,strName.c_str());

    m_pClient->SendData(0,(char*)&rq,sizeof (rq));
}




//一下是处理从服务器接收到的信息
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
    qDebug()<<__func__;
    //拆包
    STRU_REGISTER_RS *rs = (STRU_REGISTER_RS*)buf;
    //根据不同结果弹出不同窗口
    switch(rs->m_lResult)
    {
    case tel_is_exist:
        QMessageBox::about(m_pLoginDialog,"提示","手机号存在，注册失败");
        break;
    case name_is_exist:
        QMessageBox::about(m_pLoginDialog,"提示","昵称存在，注册失败");
        break;
    case register_sucess:
        QMessageBox::about(m_pLoginDialog,"提示","注册成功");
        break;
    default:
        break;
    }
}
