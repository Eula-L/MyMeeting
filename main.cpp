#include "ckernel.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MyMeetingDialog w;
//    w.show();
    CKernel::GetInstance();
    return a.exec();
}
