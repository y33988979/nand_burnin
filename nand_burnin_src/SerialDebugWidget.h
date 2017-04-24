#ifndef SERIALDEBUGWIDGET_H
#define SERIALDEBUGWIDGET_H

#include <QWidget>
#include "common.h"

class SerialDebugWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SerialDebugWidget(QWidget *parent = 0);

    QPushButton    *enterBtn;
    QTextEdit      *outputEdit;
    QLineEdit      *cmdlineEdit;

    QHBoxLayout    *hLayout;
    QGridLayout    *gLayout;
signals:

public slots:

};

#endif // SERIALDEBUGWIDGET_H
