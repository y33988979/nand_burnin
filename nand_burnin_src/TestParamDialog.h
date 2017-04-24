#ifndef TESTPARAMDIALOG_H
#define TESTPARAMDIALOG_H

#include <QDialog>
#include "common.h"

namespace Ui {
class TestParamDialog;
}

typedef struct _test_param
{
    unsigned long   start_addr;
    unsigned long   length;
    unsigned int    count;
    bool            need_verify;
    bool            test_cholechip;

}TEST_param;

class TestParamDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TestParamDialog(QWidget *parent = 0);
    ~TestParamDialog();


    unsigned long  chip_size;
    unsigned int   min_start_addr;
    unsigned int   slider_addr_step;
    TEST_param     test_param;

public:
    int set_param(TEST_param *param);
    int get_param(TEST_param *param);
    int updateChipSizeLabel();
signals:
    startReady();
public slots:
    int startAddrUpdate(int slider_value);
    int lengthUpdate(int slider_value);
    int verifyCheckBoxClicked(bool);
    int wholeChipCheckBoxClicked(bool);
    int enterBtnClicked(bool);

private:
    Ui::TestParamDialog *ui;
};

#endif // TESTPARAMDIALOG_H
