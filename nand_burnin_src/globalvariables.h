#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H

typedef struct _serial_conf1
{
    char port[16];
    int baudrate;
    int databit;
    int stopbit;
    int verifybit;
}SerialConfig1;

extern SerialConfig1 serial_conf1;





#endif // GLOBALVARIABLES_H
