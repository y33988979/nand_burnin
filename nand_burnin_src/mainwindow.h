#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "common.h"
#include <QWidget>
#include <QDialog>
#include <QPainter>
#include <QBrush>
#include <QColor>
#include "HwInfoDialog.h"
#include "SerialControl.h"
#include "NandBanner.h"
#include "TestParamDialog.h"
#include "SerialDebugWidget.h"
#include "SerialConf.h"


namespace Ui {
class MainWindow;
}
extern char *g_nand_vendor[];

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int               current_event;
    QTimer           *timer;
    QTimer           *timer1;
    HwInfoDialog     *hwinfo_dialog;
    SerialControl    *serial_controler;
    NandBanner       *nand_banner;
    TestParamDialog  *testparam_dialog;
    SerialConfDial   *serialconf_dialog;


    HW_info           hw_info;

    SerialDebugWidget *serial_debug;

    int            nand_size;
    int            ewc_err_count;
    int            timer_count;

public:
    int get_nand_info();
    int get_hwinfo_process();
    int get_badblock_process();
    int nand_ewc_process();
public slots:
    int HwInfoBtn_clicked();
    int NandBadCheckBtn_clicked();
    int NandPerformanceBtn_clicked();
    int HelpBtn_clicked();
    int start_nand_test();
    int SerialBtn_clicked();
    int StopBtn_clicked();

    void update_window();

    int timer_handle();
    int serialconf_dialog_show();
    int send_cmd();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
