#ifndef SERIALCONF_DIALOG_H
#define SERIALCONF_DIALOG_H

#include "common.h"
#include <QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QComboBox>
#include <QMessageBox>
#include "globalvariables.h"
#include "SerialControl.h"



namespace Ui {
class SerialConfDial;
}


class SerialConfDial : public QDialog
{
    Q_OBJECT

public:
    explicit SerialConfDial(QWidget *parent = 0);
    ~SerialConfDial();


    QLabel *ComPortLabel;
    QLabel *BaudRateLabel;
    QLabel *DataBitLabel;
    QLabel *StopBitLabel;
    QLabel *VerifyLabel;
    QComboBox *ComComboBox;
    QComboBox *BaudRateComboBox;
    QComboBox *DataBitComboBox;
    QComboBox *StopBitComboBox;
    QComboBox *VerifyComboBox;
    QPushButton *EnterButton;
    QPushButton *CancelButton;




signals:
    void serial_config_done();


public slots:
    int EnterButton_clicked();
    int CancelButton_clicked();
private:
    Ui::SerialConfDial *ui;
};

#endif // SERIALCONF_DIALOG_H
