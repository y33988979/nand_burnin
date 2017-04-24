#include "widget.h"
#include "ui_widget.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QByteArray>
#include <QDebug>
#include <QThread>
#include <string.h>
#include <stdio.h>



Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    serial_initd = false;
    memset(serial_buffer, 0, sizeof(serial_buffer));

    /* serial config widget */
    //serial_conf_form = new SerialConfDial();

    check_serial_and_init();

    QObject::connect(serial, &QSerialPort::readyRead, this, &Widget::read_data);
    QObject::connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(slot1()));

    ui->lineEdit->setFocus();
    serial_conf_form->setWindowModality(Qt::WindowModal);
    //serial_conf_form->setWindowFlags(Qt::WindowStaysOnTopHint);
    serial_conf_form->raise();
    //serial_conf_form->show();
}

Widget::~Widget()
{
    delete serial_conf_form;
    delete ui;
}

int Widget::create_conf_file()
{
    char buffer[256] = {0};

    QMessageBox::StandardButton reply;
    QFile file(QString("./common.conf"));
    if(!file.open(QIODevice::ReadWrite))
    {
        reply = QMessageBox::critical(this, tr("错误！"),
               tr("无法创建配置文件\"common.conf\"."),\
               QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return -1;
    }

    strcpy(buffer, "port:COM1\n");
    strcat(buffer, "baudrate:115200\n");
    strcat(buffer, "databit:8\n");
    strcat(buffer, "stopbit:1\n");
    strcat(buffer, "verifybit:0\n");

    file.write(buffer, strlen(buffer));
    file.close();
}

int Widget::save_conf_file(SerialConfig *serial_conf)
{
    char buffer[256] = {0};

    QMessageBox::StandardButton reply;
    QFile file(QString("./common.conf"));
    if(!file.open(QIODevice::ReadWrite))
    {
        reply = QMessageBox::critical(this, tr("错误！"),
               tr("无法创建配置文件\"common.conf\"."),\
               QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return -1;
    }

    sprintf(buffer, "%s%s\n", "port:", serial_conf->port);
    strcat(buffer, "baudrate:115200\n");
    strcat(buffer, "databit:8\n");
    strcat(buffer, "stopbit:1\n");
    strcat(buffer, "verifybit:0\n");

    file.write(buffer, strlen(buffer));
    file.close();
}

int Widget::check_serial_and_init()
{

    int r = -1;
    r = serial_conf_load();
    r = init_serial("COM1");

    return r;
}

int Widget::serial_conf_load()
{

    char  *pos1 = NULL;
    char  *pos2 = NULL;
    char  conf_buf[1024];
    QByteArray buffer;
    QFile file(QString("./common.conf"));
    /**/
    if(!file.open(QIODevice::ReadWrite))
    {
        qDebug()<<"open conf file error!";
        return -1;
    }

    //serial_conf = &serial_conf_form->serial_config;
    buffer = file.readAll();
    strcpy(conf_buf, buffer.data());
    pos1 = strstr(conf_buf, "port:");
    pos2 = strstr(pos1, "\n");
    *pos2 = '\0';
    strcpy(serial_conf->port, pos1+strlen("port:"));
    *pos2 = '\n';

    pos1 = strstr(conf_buf, "baudrate:");
    pos2 = strstr(pos1, "\n");
    *pos2 = '\0';
    serial_conf->baudrate = atoi(pos1+strlen("baudrate:"));
    *pos2 = '\n';

    pos1 = strstr(conf_buf, "databit:");
    pos2 = strstr(pos1, "\n");
    *pos2 = '\0';
    serial_conf->databit = atoi(conf_buf+strlen("databit:"));
    *pos2 = '\n';

    pos1 = strstr(conf_buf, "stopbit:");
    pos2 = strstr(pos1, "\n");
    *pos2 = '\0';
    serial_conf->stopbit = atoi(pos1+strlen("stopbit:"));
    *pos2 = '\n';

    pos1 = strstr(conf_buf, "verifybit:");
    pos2 = strstr(pos1, "\n");
    *pos2 = '\0';
    serial_conf->verifybit = atoi(pos1+strlen("verifybit:"));
    *pos2 = '\n';

    file.close();
    qDebug()<<"serial_conf:";
    qDebug()<<"port="<<serial_conf->port;
    qDebug()<<"baudrate="<<serial_conf->baudrate;
    qDebug()<<"databit="<<serial_conf->databit;
    qDebug()<<"stopbit="<<serial_conf->stopbit;
    qDebug()<<"verifybit="<<serial_conf->verifybit;
}

int Widget::init_serial(QString name)
{

    bool ret = true;
    int BaudRate = 115200;

    if(serial_initd)
    {
        qDebug()<<"serial have already inited "<<serial_initd;
        return 0;
    }

    serial = new QSerialPort;
    //设置串口名
    serial->setPortName(name);
    //设置波特率
    ret &= serial->setBaudRate(QSerialPort::Baud115200);
    //设置数据位数
    ret &= serial->setDataBits(QSerialPort::Data8);
    //设置奇偶校验
    ret &=serial->setParity(QSerialPort::NoParity);
    //设置停止位
    ret &= serial->setStopBits(QSerialPort::OneStop);
    //设置流控制
    ret &= serial->setFlowControl(QSerialPort::NoFlowControl);

    if(!ret)
    {
        qDebug()<<"set params error: "<<serial->errorString();
        return 0;
    }
    //打开串口
    ret = serial->open(QIODevice::ReadWrite);
    if(ret == false)
    {
        qDebug()<<"open com1 error: "<<serial->errorString();
        return 0;
    }

    qDebug()<<name<<"init success!";
    serial_initd = true;
}


int Widget::slot1()
{
    char buffer[128] = {0};

    //qDebug()<<"enter finished!";
    strcpy(buffer, ui->lineEdit->text().toLocal8Bit().data());
    strcat(buffer, "\n");
    write_data(buffer, strlen(buffer));
}

int Widget::read_data()
{
    QByteArray  buf;
    buf = serial->readAll();

    qDebug()<<"read: len="<<buf.length();

    //放入串口数据缓存区，u-boot串口返回的QByteArray.data前2个字节为\r\n，代表先换行再输出
    strcat(serial_buffer, buf.data());
    qDebug()<<"len"<<strlen(serial_buffer)<<"::"<<QString(serial_buffer);
    serial_buffer[0] = 0;

    ui->textEdit->insertPlainText(QString(buf));
    //ui->textEdit->append(QString(buf));

    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textEdit->setTextCursor(cursor);

#if 0
    if(buf.contains("\n"))
    {
        //行缓冲，打印一行
        qDebug()<<"11"<<QString(serial_buffer);
        //输出数据到text
        ui->textEdit->append(QString(buf));
        //新行置顶
        strcpy(serial_buffer, serial_buffer+buf.indexOf("\n")+1);
    }
#endif
}

int Widget::write_data(char *buffer, int len)
{
    int ret = -1;
    ret = serial->write(buffer, len);
    ui->lineEdit->clear();
    return ret;
}


void Widget::on_pushButton_clicked()
{
    int nread;
    char buffer[1024];
    init_serial("");

#if 0
    while(serial_initd)
    {
        nread = serial->read(buffer, 512);
        if(nread == 0)
        {
//            usleep(100*1000);
            QThread::msleep(1000);
            qDebug()<<"nread1= "<<nread;
            continue;
        }
        qDebug()<<"nread= "<<nread;
    }
#endif
}

void Widget::on_pushButton_2_clicked()
{
    serial_initd = false;
    serial->close();
    qDebug()<<"close serial!\n";
}
