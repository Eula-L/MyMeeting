#include "ckernel.h"
#include <QDebug>

CKernel::CKernel(QObject *parent) : QObject(parent)
{
    qDebug()<<__func__;
    m_pMyMeetingDlg = new MyMeetingDialog;
    m_pMyMeetingDlg->show();

    connect(m_pMyMeetingDlg,SIGNAL(SIG_close()),
            this,SLOT(slot_destroy()));

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
