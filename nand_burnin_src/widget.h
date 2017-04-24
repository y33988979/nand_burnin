#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QtSerialPort/QtSerialPort>
#include <QTextCursor>
#include <QMessageBox>
#include <QFile>

#include "SerialControl.h"
#include "SerialConf.h"



namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    bool serial_initd;
    QString serial_name;
    char serial_buffer[2048];

    int create_conf_file();
    int save_conf_file(SerialConfig *serial_conf);
    int check_serial_and_init();
    int init_serial(QString name);
    int serial_conf_load();
    QSerialPort *serial;
    SerialConfDial  *serial_conf_form;
    SerialConfig *serial_conf;


public slots:
    int read_data();
    int write_data(char *buffer, int len);
    int slot1();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
