#ifndef HWINFODIALOG_H
#define HWINFODIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QDebug>

namespace Ui {
class HwInfoDialog;
}

class HwInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HwInfoDialog(QWidget *parent = 0);
    ~HwInfoDialog();

    QLabel *HWinfo_label;
    QPushButton *closeBtn;

private:
    Ui::HwInfoDialog *ui;
};

#endif // HWINFODIALOG_H
