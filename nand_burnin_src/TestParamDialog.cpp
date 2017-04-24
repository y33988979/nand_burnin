#include "TestParamDialog.h"
#include "ui_TestParamDialog.h"

TestParamDialog::TestParamDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestParamDialog)
{
    ui->setupUi(this);

    test_param.start_addr = 0x100000;
    test_param.length = 0x200000;
    test_param.count = 1;
    test_param.need_verify = true;

    ui->startAddrLineEdit->setAlignment(Qt::AlignRight);
    ui->lengthLineEdit->setAlignment(Qt::AlignRight);
    ui->countLineEdit->setAlignment(Qt::AlignRight);

    ui->startAddrLineEdit->setText(tr("0x")+QString::number(test_param.start_addr, 16));
    ui->lengthLineEdit->setText(tr("0x")+QString::number(test_param.length, 16));
    ui->countLineEdit->setText(QString::number(test_param.count, 10));
    ui->wholeChipCheckBox->setChecked(false);
    ui->verifyCheckBox->setChecked(true);

    ui->startAddrSlider->setMaximum(64);
    ui->lengthSlider->setMaximum(64);

    chip_size = 0;//0x8000000;
    //slider_addr_step = chip_size / 64;
    min_start_addr = 0x100000;
    connect(ui->startAddrSlider, SIGNAL(valueChanged(int)), this, SLOT(startAddrUpdate(int)));
    connect(ui->lengthSlider, SIGNAL(valueChanged(int)), this, SLOT(lengthUpdate(int)));
    connect(ui->verifyCheckBox, SIGNAL(clicked(bool)), this, SLOT(verifyCheckBoxClicked(bool)));
    connect(ui->wholeChipCheckBox, SIGNAL(clicked(bool)), this, SLOT(wholeChipCheckBoxClicked(bool)));
    connect(ui->enterBtn, SIGNAL(clicked(bool)), this, SLOT(enterBtnClicked(bool)));

    setWindowTitle(tr("参数设置"));
    setFixedSize(240, 240);
}


int TestParamDialog::set_param(TEST_param *param)
{
    test_param.start_addr = param->start_addr;
    test_param.length = param->length;
    test_param.count = param->count;
    test_param.need_verify = param->need_verify;
    test_param.test_cholechip = param->test_cholechip;

    ui->startAddrLineEdit->setText(tr("0x")+QString::number(test_param.start_addr, 16));
    ui->lengthLineEdit->setText(tr("0x")+QString::number(test_param.length, 16));
    ui->countLineEdit->setText(QString::number(test_param.count, 10));
    ui->wholeChipCheckBox->setChecked(param->test_cholechip);
    ui->verifyCheckBox->setChecked(param->need_verify);

}

int TestParamDialog::get_param(TEST_param *param)
{
    bool ok;
    param->start_addr = ui->startAddrLineEdit->text().toLong(&ok, 0);
    if(!ok)
    {
        QMessageBox::warning(this, tr("参数错误"), tr("地址错误！请输入十六进制地址。例：0x100000为1MB"));
        return -1;
    }
    param->length = ui->lengthLineEdit->text().toLong(&ok, 0);
    if(!ok)
    {
        QMessageBox::warning(this, tr("参数错误"), tr("长度错误！请输入十六进制长度。例：0x100000为1MB"));
        return -1;
    }
    param->count = ui->countLineEdit->text().toInt(&ok, 0);
    if(!ok)
    {
        QMessageBox::warning(this, tr("参数错误"), tr("地址或长度错误！"));
        return -1;
    }
    param->test_cholechip = ui->wholeChipCheckBox->isChecked();
    param->need_verify = ui->verifyCheckBox->isChecked();
#if DEBUG
    qDebug()<<"startaddr"<<param->start_addr;
    qDebug()<<"length"<<param->length;
    qDebug()<<"count"<<param->count;
    qDebug()<<"test_cholechip"<<param->test_cholechip;
    qDebug()<<"need_verify"<<param->need_verify;
#endif
    return 0;
}

int TestParamDialog::startAddrUpdate(int slider_value)
{
    qDebug()<<"staraddr slider_value"<<slider_value;

    test_param.start_addr = min_start_addr + (slider_value*slider_addr_step);
    if(ui->startAddrSlider->value() == 64)
    {
        qDebug()<<"isMaximized";
        test_param.start_addr = chip_size;
    }
    ui->startAddrLineEdit->setText(tr("0x")+QString::number(test_param.start_addr, 16));
    ui->startMbLabel->setText(tr("%1MB").arg(QString::number(test_param.start_addr/0x100000)));

    test_param.length = chip_size - test_param.start_addr;
    ui->lengthLineEdit->setText(tr("0x")+QString::number(test_param.length, 16));
    ui->lengthMbLabel->setText(tr("%1MB").arg(QString::number(test_param.length/0x100000)));
    ui->lengthSlider->setValue(64);
}

int TestParamDialog::lengthUpdate(int slider_value)
{
    long length = chip_size - test_param.start_addr;

    qDebug()<<"length slider_value"<<slider_value;
    test_param.length = ((length/64)*slider_value);
    test_param.length &= ~0x3ffff;
    if(ui->lengthSlider->value() == 64)
    {
        qDebug()<<"isMaximized";
        test_param.length = length;
    }
    else if (ui->lengthSlider->value() == 0)
    {
        test_param.length = 0x200000;
    }
    ui->lengthLineEdit->setText(tr("0x")+QString::number(test_param.length, 16));
    ui->lengthMbLabel->setText(tr("%1MB").arg(QString::number(test_param.length/0x100000)));


}

int TestParamDialog::verifyCheckBoxClicked(bool checked)
{
    test_param.need_verify = checked;
}

int TestParamDialog::wholeChipCheckBoxClicked(bool checked)
{
    test_param.test_cholechip = checked;
    if(checked)
    {
        test_param.start_addr = min_start_addr;
        test_param.length = chip_size - min_start_addr;
        ui->startAddrLineEdit->setText(tr("0x")+QString::number(test_param.start_addr, 16));
        ui->startAddrSlider->setValue(0);
        ui->lengthLineEdit->setText(tr("0x")+QString::number(test_param.length, 16));
        ui->lengthSlider->setValue(ui->lengthSlider->maximum());
    }
}

int TestParamDialog::enterBtnClicked(bool checked)
{
    int   ret = 0;
    bool  ok;
    TEST_param  current_param = {0};

    if(get_param(&current_param) < 0)
        return -1;

    if(current_param.start_addr >= chip_size
            || current_param.start_addr <= 0)
        ret = -2;
    if(test_param.length > chip_size-current_param.start_addr
            || test_param.length <= 0)
        ret = -3;

    if(ret != 0)
    {
        QMessageBox::warning(this, tr("参数错误"), tr("地址或长度错误！"));
        return ret;
    }

    set_param(&current_param);
    emit startReady();

    return ret;
}

int TestParamDialog::updateChipSizeLabel()
{
    ui->chipRangeLabel->setText(tr("整片范围：0x00--")+tr("0x")+QString::number(chip_size, 16));

    /* 显示初始测试范围 */
    ui->startAddrLineEdit->setText(tr("0x")+QString::number(test_param.start_addr, 16));
    ui->startMbLabel->setText(tr("%1MB").arg(QString::number(test_param.start_addr/0x100000)));

    ui->lengthLineEdit->setText(tr("0x")+QString::number(test_param.length, 16));
    ui->lengthMbLabel->setText(tr("%1MB").arg(QString::number(test_param.length/0x100000)));
}


TestParamDialog::~TestParamDialog()
{
    delete ui;
}
