#ifndef MYMEETINGDIALOG_H
#define MYMEETINGDIALOG_H

#include <QDialog>
#include <QCloseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MyMeetingDialog; }
QT_END_NAMESPACE

class MyMeetingDialog : public QDialog
{
    Q_OBJECT
signals:
    void SIG_close();
public:
    MyMeetingDialog(QWidget *parent = nullptr);
    ~MyMeetingDialog();

    //重写closeEvent
    void closeEvent(QCloseEvent *event);
private:
    Ui::MyMeetingDialog *ui;
};
#endif // MYMEETINGDIALOG_H
