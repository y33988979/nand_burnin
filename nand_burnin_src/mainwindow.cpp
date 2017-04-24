#include "mainwindow.h"
#include "ui_mainwindow.h"

int Count_bad_blocks;

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    current_event = 0;
    ewc_err_count = 0;
    timer_count = 0;

    setFixedSize(500, 300);
    setWindowTitle(tr("nand测试工具v1.1"));

    QFont font;
    font.setBold(false);
    font.setPointSize(12);
    hwinfo_dialog = new HwInfoDialog();
    hwinfo_dialog->setFont(font);
    //hwinfo_dialog->show();


    serialconf_dialog = new SerialConfDial();
    serialconf_dialog->setFont(font);
    //serialconf_dialog->show();

    testparam_dialog = new TestParamDialog(this);
    testparam_dialog->setFont(font);
    //testparam_dialog->show();


    nand_banner = new NandBanner(this);
    nand_banner->setGeometry(0, 50, width(), 48);
    nand_banner->show();

    serial_debug = new SerialDebugWidget(this);
    serial_debug->setGeometry(0, 100, width(),200);
    serial_debug->show();

    connect(ui->Stop, SIGNAL(released()), this, SLOT(StopBtn_clicked()));
    connect(ui->SerialBtn, SIGNAL(released()), this, SLOT(SerialBtn_clicked()));
    connect(ui->HwInfoBtn, SIGNAL(released()), this, SLOT(HwInfoBtn_clicked()));
    connect(ui->NandBadCheckBtn, SIGNAL(released()), this, SLOT(NandBadCheckBtn_clicked()));
    connect(ui->NandPerformanceBtn, SIGNAL(released()), this, SLOT(NandPerformanceBtn_clicked()));
    connect(ui->HelpBtn, SIGNAL(released()), this, SLOT(HelpBtn_clicked()));

    connect(testparam_dialog, SIGNAL(startReady()), this, SLOT(start_nand_test()));
    connect(serial_debug->cmdlineEdit, SIGNAL(returnPressed()), this, SLOT(send_cmd()));
    connect(serial_debug->enterBtn, SIGNAL(released()), this, SLOT(send_cmd()));

    bool ok = connect(serialconf_dialog, SIGNAL(serial_config_done()), this, SLOT(update_window()));

    memset(&hw_info, 0, sizeof(HW_info));
    hw_info.nand_info.size = 0x20000000;
    hw_info.nand_info.erase_size = 0x20000;

    ui->HwInfoBtn->setEnabled(true);
    ui->NandBadCheckBtn->setEnabled(false);
    ui->NandPerformanceBtn->setEnabled(false);

    serial_controler = new SerialControl();

    if(!serial_controler->found_serial)
    {
        switch(QMessageBox::critical(this, tr("错误！"),
                              tr("未找到可用串口，请检查机器上是否存在串口！"),QMessageBox::Ok))
        {
            case QMessageBox::Ok:
            default:
                ui->NandBadCheckBtn->setEnabled(false);
                ui->HelpBtn->setEnabled(true);
                ui->HwInfoBtn->setEnabled(false);
                ui->NandPerformanceBtn->setEnabled(false);
                close();
        }
    }
/*******
    if(serial_controler->init() < 0)
    {

        qDebug()<<"serial_controler->init()\n";
        QMessageBox::critical(this, tr("错误！"),
             tr("无法打开串口! 请确认串口未被占用~"), QMessageBox::Ok);
        ui->NandBadCheckBtn->setEnabled(false);
        ui->HelpBtn->setEnabled(true);
        ui->HwInfoBtn->setEnabled(false);
        ui->NandPerformanceBtn->setEnabled(false);
        close();
    }
**/
    serial_controler->pTextEdit = serial_debug->outputEdit;


    timer = new QTimer(this);
    timer1 = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timer_handle()));
    connect(timer1, SIGNAL(timeout()), this, SLOT(serialconf_dialog_show()));
    timer1->start(200);

}

int MainWindow::get_nand_info()
{
    /* 先获取Nand信息 */
    current_event |= EVENT_GETINFO;
    serial_controler->send_cmd("getinfo nand\n");
    timer_count = 0;
    timer->start(200);
}

int MainWindow::StopBtn_clicked()
{
    qDebug()<<"Stop Btn\n";
    char cmdline[128] = {0};

    strcpy(cmdline, "stop\n");

    serial_controler->send_cmd(cmdline);

}

int MainWindow::serialconf_dialog_show()
{

        serialconf_dialog->show();

        timer1->stop();
}

int MainWindow::SerialBtn_clicked()
{
    qDebug()<<"Serial Choose Dialog\n";
    serialconf_dialog->show();
}


int MainWindow::HwInfoBtn_clicked()
{
    /* send cmd*/
    current_event |= EVENT_GETINFO;
    serial_controler->send_cmd("getinfo nand\n");
    qDebug()<<"getinfo nand\n";
    hwinfo_dialog->show();
    timer_count = 0;
    timer->start(500);
}

int MainWindow::NandBadCheckBtn_clicked()
{
    current_event |= EVENT_GETBAD;
    serial_controler->send_cmd("nand bad\n");
    timer_count = 0;
    timer->start(200);
}

int MainWindow::NandPerformanceBtn_clicked()
{
    testparam_dialog->updateChipSizeLabel();
    testparam_dialog->show();
}

int MainWindow::HelpBtn_clicked()
{
    QMessageBox::information(this, tr("提示"),
         tr("1.连接好串口线，并确保盒子中有对应boot程序。\n"
            "2.测试前先点击\"Serial\"按钮，并设置串口信息，然后点击\"硬件信息\"按钮，获取盒子nand信息。\n"
            "3.绿色矩形用来模拟nand flash。\n"
            "4.工具会自动检测串口设备，请确保系统中存在可用串口。"), QMessageBox::Ok);
}

int MainWindow::start_nand_test()
{
    unsigned int  startPercent, endPercent;
    char cmd[128] = {0};
    TEST_param  param = {0};

    testparam_dialog->get_param(&param);

    startPercent = (param.start_addr)/(testparam_dialog->chip_size/1000);
    endPercent = (param.start_addr+param.length)/(testparam_dialog->chip_size/1000);
    qDebug()<<"chip_size"<<testparam_dialog->chip_size;
    qDebug()<<"param.start_addr"<<param.start_addr;
    qDebug()<<"startPercent"<<startPercent;
    qDebug()<<"endPercent"<<endPercent;
    nand_banner->setRange(startPercent, endPercent);
    //return 0;

    if(param.need_verify)
    {
        current_event |= EVENT_ERASE_RW;
        sprintf(cmd, "nand ewc %x %x -t %d\n", param.start_addr, param.length, param.count);
    }
    else
    {
        current_event |= EVENT_ERASE_WRITE;
        sprintf(cmd, "nand ew %x %x -t %d\n", param.start_addr, param.length, param.count);
    }
    qDebug()<<"start_nand_test:"<<cmd;

    serial_controler->send_cmd(cmd);
    testparam_dialog->hide();
    timer_count = 0;
    timer->start(100);
}


int MainWindow::timer_handle()
{
    static int count = 0;

    if(current_event == 0)
        return 0;

    if(current_event & EVENT_GETINFO)
        get_hwinfo_process();
    if(current_event & EVENT_GETBAD)
        get_badblock_process();
    if(current_event & EVENT_ERASE_WRITE
            || current_event & EVENT_ERASE_RW)
    {
        nand_ewc_process();
    }

    if(current_event == 0)
        timer->stop();

    return 0;
}

int MainWindow::nand_ewc_process()
{
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_datetime = current_date_time.toString("yyyy-MM-dd hh:mm:ss");

    int   num = 0;
    bool  test_done  = false;
    char *pos1, *pos2;
    char  buffer[2048] = {0};
    SerialControl *sc = serial_controler;

    if(!sc->recv_line_done)
    {
        if(timer_count++ == 10)
        {
            //qDebug()<<"timer here\n";
            timer->stop();
            QMessageBox::information(this, tr("提示"),
                 tr("串口通讯超时，请检查机顶盒连接是否正常，并有对应boot程序！"), QMessageBox::Ok);
        }
        return 0;
    }

    timer_count = 11;

    if(sc->cmd_finish())
    {
        qDebug()<<"cmd_finish cmd_finish!!!";
        test_done = true;
    }

    if(sc->recv_line_done)
    {
        sc->read_data(buffer, sizeof(buffer));
        pos1 = buffer;
        while(pos1)
        {
            if((pos1 = strstr(pos1, "failed")))
            {
                ewc_err_count++;
                pos1++;
            }
        }

        if((pos1 = strstr(buffer, "Writing")) == NULL)
            return 0;

        pos2 = pos1;
        while(pos1)
        {
            pos2 = pos1;
            pos1 = strstr(pos1+1, "% ");
        }
        *pos2 = '\0';
        num = atoi(pos2-3);
        *pos2 = '%';
        qDebug()<<"i1="<<num<<"str:"<<pos2;

        /* 解析出正常的百分比 */
        if(num != 0)
        {
            nand_banner->percent_list.push_back(num);
            nand_banner->update();
        }
    }

    if(test_done)
    {
        qDebug()<<"The time is:"<<current_datetime;
        QDir dir;
        QString path;
        path = dir.currentPath();
        path += "/burn_log";
        qDebug()<<path;
        QDir *dir1 = new QDir;
        if(!dir1->exists(path))
            dir1->mkdir(path);
        path += "/NandPerformance_test_log.txt";
        QFile file(path);
        qDebug()<<path;

        if(file.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text))
        {
            qDebug()<<"open file succeed";
            QTextStream in(&file);
            QString result1 = "flash性能测试完毕，共发现";
            QString result2 = "个错误。";
            in<<current_datetime<<":"<<result1<<ewc_err_count<<result2<<"\n"<<"\n";
        }
        else
        {
            QMessageBox::warning(this, "提示", "Can't Open file!", QMessageBox::Ok);
            qDebug()<<"open file failed";
        }
        file.close();

        current_event = 0;
        timer->stop();
        QMessageBox::information(this, tr("提示"),
             tr("测试完成！共发现%1个错误。").arg(ewc_err_count), QMessageBox::Ok);
    }
}

int MainWindow::get_hwinfo_process()
{
    char  i, id[4] = {0};
    char *pos1, *pos2;
    char  buffer[512] = {0};
    SerialControl *sc = serial_controler;

    if(!sc->cmd_finish())
    {
        if(timer_count++ == 2)
        {
            //qDebug()<<"cmd no finish!\n";
            timer->stop();
            if(!hwinfo_dialog->isHidden())
                hwinfo_dialog->close();
                QMessageBox::information(this, tr("提示"),
                 tr("串口通讯超时，请检查机顶盒连接是否正常，并有对应boot程序！"), QMessageBox::Ok);
        }
        return 0;
    }
    timer_count = 6;

    if(sc->cmd_finish())
    {
        sc->read_data(buffer, sizeof(buffer));
        //qDebug()<<"getinfo: buffer"<<buffer;
        /* chip vendor */
        pos1 = strstr(buffer, "ID:");
        pos1[7] = '\0';
        strcpy(id, pos1+5);
        pos1[7] = ' ';

        for(i=0; i<11; i++)
        {
            if(strstr(g_nand_vendor[i], id))
                break;
        }
        if(i != 11)
            strcpy(hw_info.chip_name_str, g_nand_vendor[i]+5);
        else
            strcpy(hw_info.chip_name_str, "Unkown");
        strcat(hw_info.chip_name_str, "\n");

        /* nand size */
        pos1 = strstr(buffer, "Name:");
        pos2 = strstr(pos1, "\n");
        *(pos2+1) = '\0';
        strcpy(hw_info.nand_size_str, pos1+11);
        /* 保存chip信息 */
        if(testparam_dialog->chip_size == 0)
        {
            int   chip_size = 0;
            char  tmp[64] = {0};
            sscanf(pos1,"Name:\"NAND %dMiB%s", &chip_size, tmp);
            testparam_dialog->chip_size = chip_size *(1024*1024);
            testparam_dialog->slider_addr_step = testparam_dialog->chip_size / 64;
            qDebug()<<"chip_size"<<testparam_dialog->chip_size;
            ui->NandBadCheckBtn->setEnabled(true);
            ui->NandPerformanceBtn->setEnabled(true);
        }
        *(pos2+1) = '\n';

        /* nand info */
        pos1 = strstr(buffer, "Block:");
        pos2 = strstr(pos1, "\n");
        *(pos2+1) = '\0';
        pos2 = pos1;
        while(*pos1++)
        {
            if(*pos1 == ' ')
                *(pos1) = '\n';
        }

        strcpy(hw_info.nand_info_str, pos2);

        qDebug()<<"name:"<<hw_info.chip_name_str;
        qDebug()<<"size:"<<hw_info.nand_size_str;
        qDebug()<<"info:"<<hw_info.nand_info_str;

        sprintf(buffer, "%s%s\n\n%s%s\n\n%s%s\n\n", \
                tr("生产厂商：").toLocal8Bit().data(), hw_info.chip_name_str, \
                tr("存储大小：").toLocal8Bit().data(), hw_info.nand_size_str, \
                tr("芯片信息:").toLocal8Bit().data(), hw_info.nand_info_str);
        QString str;
        str.append(tr("生产厂商:"));
        str.append(hw_info.chip_name_str);
        str.append(tr("存储大小:"));
        str.append(hw_info.nand_size_str);
        str.append(tr("芯片信息:\n"));
        str.append(hw_info.nand_info_str);

        if(!hwinfo_dialog->isHidden())
            hwinfo_dialog->HWinfo_label->setText(str);
        current_event &= ~EVENT_GETINFO;
        timer->stop();
    }

    return 0;
}

int MainWindow::get_badblock_process()
{
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_datetime = current_date_time.toString("yyyy-MM-dd hh:mm:ss");

    int   bad_index, percent = 0;

    char  cnt = 0;
    char *pos1, *pos2;
    char  buffer[20480] = {0};
    unsigned long   block_addr;

    SerialControl *sc = serial_controler;

    if(!sc->cmd_finish())
    {
        if(timer_count++ == 10)
        {
            timer->stop();
            QMessageBox::information(this, tr("提示"),
                 tr("串口通讯超时，请检查机顶盒连接是否正常，并有对应boot程序！"), QMessageBox::Ok);
        }
        return 0;
    }
    timer_count = 6;

    if(sc->cmd_finish())
    {
        sc->read_data(buffer, sizeof(buffer));
        //qDebug()<<"getbad:buffer"<<buffer;

        pos2 = pos1 = buffer;
        while(pos2)
        {
            if((pos2 = strstr(pos1, "  ")))
            {
                *pos2 = '0';
                *(pos2+1) = 'x';
                *(pos2+10) = '\0';

                QString str(pos2);
                bool  ok;
                block_addr = str.toInt(&ok, 16);
                bad_index = block_addr / hw_info.nand_info.erase_size;
                percent = (block_addr)/(hw_info.nand_info.size/1000);

                qDebug()<<"bad block:"<<bad_index;
                qDebug()<<"percent:"<<percent;
                *(pos2+10) = '\n';
                cnt++;

                nand_banner->badblock_list.push_back(percent);

            }

            pos1 = pos2+1;
        }
        Count_bad_blocks = nand_banner->badblock_list.count();

        QMessageBox::information(this, tr("提示！"),
             tr("flash坏块检测完成，共有%1块坏块！").arg(Count_bad_blocks), QMessageBox::Ok);

        qDebug()<<"The time is:"<<current_datetime;
        QString str = "flash坏块检测完毕，共有";
        QDir dir;
        QString path;
        path = dir.currentPath();
        path += "/burn_log";
        qDebug()<<path;
        QDir *dir1 = new QDir;
        if(!dir1->exists(path))
            dir1->mkdir(path);
        path += "/badblock_test_log.txt";
        QFile file(path);
        qDebug()<<path;

        if(file.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text))
        {
            qDebug()<<"open file succeed";
            QString result1 = "flash坏块检测完毕，共有";
            QString result2 = "块坏块！";
            QTextStream in(&file);
            in<<current_datetime<<":"<<result1<<Count_bad_blocks<<result2<<"\n"<<"\n";
        }
        else
        {
            QMessageBox::warning(this, "提示", "Can't Open file!", QMessageBox::Ok);
            qDebug()<<"open file failed";
        }
        file.close();

        current_event &= ~EVENT_GETBAD;
        timer->stop();
        nand_banner->update();

    }
}


int MainWindow::send_cmd()
{
    char cmdline[128] = {0};

    //qDebug()<<"enter finished!";
    strcpy(cmdline, serial_debug->cmdlineEdit->text().toLocal8Bit().data());
    strcat(cmdline, "\n");
    serial_debug->cmdlineEdit->clear();
    serial_controler->send_cmd(cmdline);
}


void MainWindow::update_window()
{
    int ret = 0;
    qDebug()<<"update_window\n";
    if(!serial_controler->found_serial)
    {
        {
            switch(QMessageBox::critical(this, tr("错误！"),
                                  tr("未找到可用串口，请检查机器上是否存在串口！"),QMessageBox::Ok))
            {
                case QMessageBox::Ok:
                default:
                    ui->NandBadCheckBtn->setEnabled(false);
                    ui->HelpBtn->setEnabled(true);
                    ui->HwInfoBtn->setEnabled(false);
                    ui->NandPerformanceBtn->setEnabled(false);
                    close();
            }
        }
    }
    ret = serial_controler->init();
    if(ret < 0)
    {
        QMessageBox::critical(this, tr("错误！"),
             tr("无法打开串口! 请确认串口未被占用~"), QMessageBox::Ok);
        ui->NandBadCheckBtn->setEnabled(false);
        ui->HelpBtn->setEnabled(true);
        ui->HwInfoBtn->setEnabled(false);
        ui->NandPerformanceBtn->setEnabled(false);
        close();
    }

}


MainWindow::~MainWindow()
{
    delete timer;
    delete hwinfo_dialog;
    delete ui;
}
