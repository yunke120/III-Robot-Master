#include "frmmain.h"
#include "ui_frmmain.h"

#include "quihelper.h"

frmMain::frmMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmMain)
{
    ui->setupUi(this);

    initForm();
}

frmMain::~frmMain()
{
    delete ui;
}

bool frmMain::eventFilter(QObject *watched, QEvent *event)
{
    return 0;
}

void frmMain::initForm()
{
    QUIHelper::setFramelessForm(this);
//    this->setProperty("form", true);
//    this->setProperty("canMove", true);
//    this->setWindowFlags(Qt::FramelessWindowHint);
//#ifdef Q_OS_WIN
//    this->setWindowFlags(this->windowFlags() | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
//#endif
}
