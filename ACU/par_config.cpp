#include "par_config.h"
#include "ui_par_config.h"

Par_Config::Par_Config(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Par_Config)
{
    ui->setupUi(this);

    this->setWindowTitle("通信参数配置");
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->serial_select->addItem(info.portName());
    }
    //连接
    connect(ui->apply_button,SIGNAL(clicked()),this,SLOT(apply_button_set()));
    connect(ui->baud_select,SIGNAL(currentIndexChanged(int)),this,SLOT(baud_custom(int)));

    updateSettings();  //在这里初始化一下,防止未初始化导致不能连接吧
}

Par_Config::~Par_Config()
{
    delete ui;
}

void Par_Config::apply_button_set()
{

   updateSettings();
   this->hide();
}


void Par_Config::baud_custom(int idx)
{
    bool isEdit=false;
    if(idx==ui->baud_select->count()-1)
    {
        isEdit =true;
    }
    ui->baud_select->setEditable(isEdit);
}


//相当于初始化settings结构体变量
void Par_Config::updateSettings()
{

       settings.name = ui->serial_select->currentText();
       settings.baudRate = ui->baud_select->currentText().toInt();  //波特率
       switch (ui->databits_select->currentText().toInt())
       {
           case 8: settings.dataBits = QSerialPort::Data8; break;
           case 7: settings.dataBits = QSerialPort::Data7; break;
           case 6: settings.dataBits = QSerialPort::Data6; break;
           default: break;
       }
       switch(ui->stopbits_select->currentIndex())
       {
           case 0:settings.stopBits = QSerialPort::OneStop;break;
           case 1:settings.stopBits =QSerialPort::OneAndHalfStop;break;
           case 2:settings.stopBits =QSerialPort::TwoStop;break;
           default: break;
       }
       switch (ui->parity_select->currentIndex())
       {
           case 0:settings.parity = QSerialPort::NoParity;break;
           case 1:settings.parity =QSerialPort::EvenParity; break;
           case 2:settings.parity =QSerialPort::OddParity;break;
           default: break;
       }
}

Par_Config::Settings Par_Config::cursettings() const
{
    return settings;
}
