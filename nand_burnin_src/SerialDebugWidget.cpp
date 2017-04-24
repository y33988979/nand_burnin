#include "SerialDebugWidget.h"

SerialDebugWidget::SerialDebugWidget(QWidget *parent) : QWidget(parent)
{

    gLayout = new QGridLayout();
    gLayout->setSpacing(6);
    gLayout->setContentsMargins(11, 11, 11, 11);

    cmdlineEdit = new QLineEdit(this);
    enterBtn = new QPushButton(this);
    enterBtn->setText(tr("发送"));

    hLayout = new QHBoxLayout();
    hLayout->setSpacing(6);
    hLayout->addWidget(cmdlineEdit);
    hLayout->addWidget(enterBtn);

    outputEdit = new QTextEdit(this);

    gLayout->addWidget(outputEdit, 0, 0, 1, 1);
    gLayout->addLayout(hLayout, 1, 0, 1, 1);

    setLayout(gLayout);

}
