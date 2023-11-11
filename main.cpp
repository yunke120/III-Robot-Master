#include "form/frmmain.h"
#include "core_base/appinit.h"
#include "core_base/quihelper.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QUIHelper::initMain();
    QApplication a(argc, argv);
    AppInit::Instance()->start();
    QUIHelper::setFont();

    frmMain w;
    QUIHelper::setFormInCenter(&w);
    w.show();
    return a.exec();
}
