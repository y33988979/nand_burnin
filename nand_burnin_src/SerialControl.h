#ifndef SERIALCONTROL_H
#define SERIALCONTROL_H

#include "common.h"
#include <QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "globalvariables.h"

#define    EVENT_GETINFO        0x00000001
#define    EVENT_GETBAD         0x00000002
#define    EVENT_ERASE          0x00000004
#define    EVENT_READ           0x00000008
#define    EVENT_WRITE          0x00000010
#define    EVENT_ERASE_WRITE    0x00000020
#define    EVENT_ERASE_RW       0x00000040
extern int baudrate;
extern int stopbit;
extern int databit;
extern char serial_port[16];

typedef struct _nand_info
{
    unsigned long size;
    unsigned int  erase_size;
    unsigned int  writesize;
}Nand_info;

typedef struct _hw_info
{
    char chip_name_str[16];
    char ddr_size_str[32];
    char nand_vendor_str[32];
    char nand_size_str[32];
    char nand_info_str[128];
    Nand_info nand_info;
}HW_info;

typedef struct _serial_conf
{
    char port[16];
    int baudrate;
    int databit;
    int stopbit;
    int verifybit;
}SerialConfig;

class SerialControl : public QSerialPort
{
    Q_OBJECT
public:
    QByteArray    serial_buffer;
    SerialConfig  serial_conf;
    //SerialConfig1 serial_conf1;
    int     use_mode;
    bool    cmd_exec_done;
    bool    recv_line_done;

    bool    found_serial;
    bool    serial_initd;

    QTextEdit *pTextEdit;

    int init();
    int read_data(char *buffer, int len);
    int write_data(char *buffer, int len);
    int deinit();
    int reinit();

    int recv_parse();
    int get_hwinfo(HW_info *hw_info);
    int get_badblock(int *badblock_num);
    int nandtest();

    int send_cmd(char *cmd);
    int set_mode(int mode);

    bool cmd_finish() {return cmd_exec_done;}
public slots:
    int read_to_buffer();

public:
    SerialControl(QSerialPort *parent = 0);
    ~SerialControl();
};

#endif // SERIALCONTROL_H
