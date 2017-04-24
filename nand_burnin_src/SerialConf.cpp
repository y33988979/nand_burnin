#include "SerialConf.h"
#include "ui_serialconf.h"
#include <QDesktopWidget>

int baudrate;
int stopbit;
int databit;
char serial_port[16];


SerialConfDial::SerialConfDial(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SerialConfDial)
{
    ui->setupUi(this);



    int width = 1024;
    int height = 768;

    setWindowTitle(QString("请设置串口！"));
    setGeometry(600, 300, 100, 150);

    width = QApplication::desktop()->width();
    height = QApplication::desktop()->height();

    setFixedSize(200, 200);

    QStringList baudrateList;
    baudrateList<<"115200"<<"9600"<<"14400"<<"19200"<<"38400"<<"57600";
    QStringList databitList;
    databitList<<"8"<<"5"<<"6"<<"7";
    QStringList stopbitList;
    stopbitList<<"1"<<"1.5"<<"2";


    ui->BaudrateComboBox->addItems(baudrateList);
    ui->DataBitComboBox->addItems(databitList);
    ui->StopBitComboBox->addItems(stopbitList);
    ui->VerifyComboBox->addItem("无");

    connect(ui->EnterButton, SIGNAL(released()), this, SLOT(EnterButton_clicked()));
    connect(ui->CancelButton, SIGNAL(released()), this, SLOT(CancelButton_clicked()));

    //查找可用的串口
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        QStringList comport;
        if(serial.open(QIODevice::ReadWrite))
        {
            comport += serial.portName();
            ui->ComComboBox->addItems(comport);
            serial.close();
        }
        else
        {
            ui->ComComboBox->addItem("无可用串口");
        }
    }

    //move(width/2-200, height/2-200);
    qDebug()<<"serial_conf init success!";
}

SerialConfDial::~SerialConfDial()
{
    delete ui;
}


int SerialConfDial::EnterButton_clicked()
{
    int   ret = -1;
    bool  isSuccess  = false;
    qDebug()<<"EnterButton clicked\n";
    printf("%s\n", ui->ComComboBox->currentText().toStdString().c_str());
    strcpy(serial_port, ui->ComComboBox->currentText().toStdString().c_str());
    baudrate = (int)(ui->BaudrateComboBox->currentText().toInt());
    databit = (int)(ui->DataBitComboBox->currentText().toInt());
    stopbit = (int)(ui->StopBitComboBox->currentText().toInt());

    qDebug()<<"serial port:"<<serial_port;
    qDebug()<<"serial baudrate:"<<baudrate;
    qDebug()<<"serial databit:"<<databit;
    qDebug()<<"serial stopbit:"<<stopbit;
    close();
    emit serial_config_done();
}

int SerialConfDial::CancelButton_clicked()
{
    qDebug()<<"CancelButton_clicked\n";
    close();
}
