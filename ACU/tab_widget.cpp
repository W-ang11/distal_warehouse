#include "tab_widget.h"
#include "ui_tab_widget.h"


#include <QMessageBox>

//创建一个全局的串口对象
QSerialPort serial;

Tab_Widget::Tab_Widget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Tab_Widget)
{
    ui->setupUi(this);

    my_timer = new QTimer(this);
    config_wigdet = new Par_Config;  //配置类的对象

    this->setWindowTitle("天线控制器");
    ui->tabWidget->setCurrentIndex(0); //这个主要是用于默认选择第一个

    ui->connect->setEnabled(true);
    ui->disconnect->setEnabled(false);

    connect(my_timer,&QTimer::timeout,this,&Tab_Widget::my_timerout);
    connect(ui->configure,&QAction::triggered,config_wigdet,&Par_Config::show);
    connect(ui->connect,&QAction::triggered,this,&Tab_Widget::connect_serialport);
    connect(ui->disconnect,&QAction::triggered,this,&Tab_Widget::disconnect_serialport);

    connect(&serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
                this, &Tab_Widget::handleError);
}

Tab_Widget::~Tab_Widget()
{
    delete ui;
}

void Tab_Widget::connect_serialport()
{

    //初始化串口类的对象
    Par_Config::Settings p=config_wigdet->cursettings();
    serial.setPortName(p.name);
    serial.setBaudRate(p.baudRate);
    serial.setDataBits(p.dataBits);
    serial.setStopBits(p.stopBits);
    serial.setParity(p.parity);
    if(serial.open(QIODevice::ReadWrite))
    {
        ui->connect->setEnabled(false);
        ui->disconnect->setEnabled(true);
        ui->configure->setEnabled(false);
        ui->statusbar->showMessage(tr("succeed connect to : %1 , %2, %3, %4, %5")
                                   .arg(p.name).arg(p.baudRate).arg(p.dataBits).arg(p.stopBits).arg(p.parity));
        //连接成功之后,需要发送一个查询版本信息的命令,和一些其他需要当连接上串口就需要发送的指令
        //同时需要开启一个定时器,用于循环发送get show信息,当关闭串口的时候关闭定时器
        my_timer->start(500); //500ms发送一次get show信息
        QString str = "";
        str = QString("$cmd,get sn,*21\r\n");
        serial.write(str.toLatin1().data());
        str.clear();
        str =QString("$cmd,get rec type,*70\r\n");
        serial.write(str.toLatin1().data());
        str.clear();
    }
    else
    {
        QMessageBox::critical(this, tr("Error"), serial.errorString());

        ui->statusbar->showMessage(tr("Open error"));
    }

}
void Tab_Widget::disconnect_serialport()
{
    if (serial.isOpen())
            serial.close();
        ui->connect->setEnabled(true);
        ui->disconnect->setEnabled(false);
        ui->configure->setEnabled(true);
        ui->statusbar->showMessage(tr("Disconnected"));
        my_timer->stop();

}
//定时器超时处理,用于循环发送get show信息
void Tab_Widget::my_timerout()
{
    QString str = QString("$cmd,get show,*3f\r\n");
    serial.write(str.toLatin1().data());
}

void Tab_Widget::handleError(QSerialPort::SerialPortError error)
{

    if (error == QSerialPort::ResourceError) {
        //当资源不可用时，例如设备意外地从系统中删除时，就会发生I/O错误
            QMessageBox::critical(this, tr("Critical Error"), serial.errorString());
            //槽函数可以显示的调用是吧!
            disconnect_serialport();
        }
}




