#include "HwInfoDialog.h"
#include "ui_HwInfoDialog.h"

HwInfoDialog::HwInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HwInfoDialog)
{
    ui->setupUi(this);

    HWinfo_label = new QLabel(this);
    HWinfo_label->setText(tr("Waiting...\n"));
    HWinfo_label->setGeometry(18, 18, 200, 140);

    closeBtn = new QPushButton(this);
    closeBtn->setText(tr("关闭"));
    closeBtn->setGeometry(66, 174, 58, 28);
    connect(closeBtn, SIGNAL(released()), this, SLOT(close()));

    setWindowTitle(tr("Flash信息"));
    setFixedSize(180, 220);
}

HwInfoDialog::~HwInfoDialog()
{
    delete HWinfo_label;
    delete closeBtn;
    delete ui;
}
