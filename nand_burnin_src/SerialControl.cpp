#include "SerialControl.h"
#include <QTextCursor>

char *g_nand_vendor[] =
{
    "0x98:TOSHIBA",
    "0xEC:SAMSUNG",
    "0x04:FUJITSU",
    "0x8F:NATIONAL",
    "0x07:RENESAS",
    "0x20:STMICRO",
    "0xAD:HYNIX",
    "0x2C:MICRON",
    "0x01:AMD",
    "0x92:EON",
    "0xC2:MICRON"
};

SerialControl::SerialControl(QSerialPort *parent) :
    QSerialPort(parent)
{
    pTextEdit = NULL;
    found_serial = false;
    //查找可用的串口
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            strcpy(serial_conf.port, serial.portName().toLocal8Bit().data());
            qDebug()<<"find serial:"<<serial.portName();
            serial.close();

        }
        found_serial = true;
    }

    //serial_conf.port[0] = 0;
    //serial_conf.baudrate = QSerialPort::Baud115200;
    //serial_conf.databit = QSerialPort::Data8;
    //serial_conf.stopbit = QSerialPort::OneStop;
    //serial_conf.verifybit = QSerialPort::NoParity;
    strcpy(serial_conf.port, serial_port);
    serial_conf.baudrate = baudrate;
    serial_conf.databit = databit;
    serial_conf.stopbit = stopbit;

    qDebug()<<"_____serial port_________:"<<serial_conf.port;
    qDebug()<<"_____serial baudrate_____:"<<serial_conf.baudrate;
    qDebug()<<"_____serial stopbit______:"<<serial_conf.stopbit;
    qDebug()<<"_____serial databit______:"<<serial_conf.databit;


    use_mode = 1;
    recv_line_done = false;
    cmd_exec_done = false;

    serial_initd = false;
    connect(this, SIGNAL(readyRead()), this, SLOT(read_to_buffer()));
}

SerialControl::~SerialControl()
{

}

int SerialControl::set_mode(int mode)
{
    use_mode = mode;
    cmd_exec_done = false;
    recv_line_done = false;
    serial_buffer.clear();
}


int SerialControl::read_data(char *buffer, int len)
{
    strcpy(buffer, serial_buffer.data());
    serial_buffer.clear();
    cmd_exec_done = false;
    recv_line_done = false;
}

int SerialControl::read_to_buffer()
{
    QByteArray  data;
    //qDebug()<<"len:"<<data.length();
    /* 命令交互模式 */
    if(use_mode == 1)
    {
        /* read data from serial */
        data = readAll();


        /* into buffer */
        serial_buffer.append(data);
        //qDebug()<<serial_buffer.data()<<"-----";
        if(strstr(serial_buffer.data(), "\n")
                || strstr(serial_buffer.data(), "\r"))
            recv_line_done = true;
        if(strstr(serial_buffer.data(), "fastboot#"))
            cmd_exec_done = true;

        /* 禁止缓冲区过大 */
        if(serial_buffer.length() > 10240)
        {
            cmd_exec_done = false;
            recv_line_done = false;
            serial_buffer.clear();
        }

        /* output textedit */
        if(pTextEdit)
        {
            pTextEdit->insertPlainText(QString(data));
            QTextCursor cursor = pTextEdit->textCursor();
            cursor.movePosition(QTextCursor::End);
            pTextEdit->setTextCursor(cursor);
        }
    }
    else
    {


    }
}

int SerialControl::init()
{
    bool ret = true;

    if(serial_initd)
    {
        qDebug()<<"serial have already inited "<<serial_initd;
        return 0;
    }

    setPortName(serial_port);


    //设置波特率
    //ret &= setBaudRate(QSerialPort::Baud115200);
    ret &= setBaudRate(baudrate);
    //设置数据位数
    switch(databit)
    {
        case 5:
            ret &= setDataBits(QSerialPort::Data5);
        break;

        case 6:
            ret &= setDataBits(QSerialPort::Data6);
        break;

        case 7:
            ret &= setDataBits(QSerialPort::Data7);
        break;

        default:
            ret &= setDataBits(QSerialPort::Data8);
        break;
    }

    //设置奇偶校验
    ret &= setParity(QSerialPort::NoParity);
    //设置停止位
    switch(stopbit)
    {
        case 1:
            ret &= setStopBits(QSerialPort::OneStop);
        break;
        case 2:
            ret &= setStopBits(QSerialPort::TwoStop);
        break;

        default:
            ret &= setStopBits(QSerialPort::OneAndHalfStop);
        break;
    }

    //设置流控制
    ret &= setFlowControl(QSerialPort::NoFlowControl);


    if(!ret)
    {
        qDebug()<<"set params error: "<<errorString();
        return -1;
    }
    //打开串口
    ret = open(QIODevice::ReadWrite);
    qDebug()<<ret;
    if(ret == false)
    {
        qDebug()<<"open "<<serial_conf.port<<"error: "<<errorString();
        return -1;
    }

    qDebug()<<serial_conf.port<<"init success!";
    serial_initd = true;
    return 0;
}

int SerialControl::deinit()
{
    serial_initd = false;
    close();
}

int SerialControl::reinit()
{
    serial_initd = false;
    init();
}


int SerialControl::write_data(char *buffer, int len)
{

}

int SerialControl::send_cmd(char *cmd)
{
    serial_buffer.clear();
    return write(cmd, strlen(cmd));
}

int SerialControl::recv_parse()
{

}

int SerialControl::get_hwinfo(HW_info *hw_info)
{
    int i = 0;
    int timeout = 20;
    char id[4] = {0};
    char buffer[256];
    char *pos1, *pos2;
    send_cmd("getinfo nand\n");

    while(timeout--)
    {
        if(cmd_exec_done)
            break;
        QThread::usleep(100*1000);
    }

    if(timeout == -1)
        return -1;
    read_data(buffer, sizeof(buffer));

    qDebug()<<"sdfdf"<<buffer;


    return 0;
}

int SerialControl::get_badblock(int *badblock_num)
{

}

int SerialControl::nandtest()
{

}

