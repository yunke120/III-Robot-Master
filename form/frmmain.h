#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QWidget>

namespace Ui {
class frmMain;
}

class frmMain : public QWidget
{
    Q_OBJECT

public:
    explicit frmMain(QWidget *parent = nullptr);
    ~frmMain();
protected:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void initForm();
private:
    Ui::frmMain *ui;
};

#endif // FRMMAIN_H
