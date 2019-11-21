#include "man_s.h"
#include "ui_man_s.h"

Man_S::Man_S(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Man_S)
{
    ui->setupUi(this);
}

Man_S::~Man_S()
{
    delete ui;
}

#include <QDebug>
#include "pub_info.h"
#include <QSerialPort>
extern QSerialPort serial;
void Man_S::on_pushButton_clicked()
{
    //组包发送,停止
    QString str =QString("$cmd,stop,*72\r\n");
    serial.write(str.toLatin1().data());
}

void Man_S::on_pushButton_2_clicked()
{
    //方位角增加
    //组包发送
    QString str =QString("$cmd,manual,1,%1,*hh\r\n").arg(ui->doubleSpinBox->text());
    char *pstr = str.toLatin1().data();
    Add_JY(pstr);
    serial.write(pstr);
    qDebug()<<pstr;
}

void Man_S::on_pushButton_3_clicked()
{
    //方位角减小
    //组包发送
    QString str =QString("$cmd,manual,2,%1,*hh\r\n").arg(ui->doubleSpinBox->text());
    char *pstr = str.toLatin1().data();
    Add_JY(pstr);
    serial.write(pstr);
    qDebug()<<pstr;
}

void Man_S::on_pushButton_4_clicked()
{
    //俯仰角增加
    //组包发送
    QString str =QString("$cmd,manual,3,%1,*hh\r\n").arg(ui->doubleSpinBox->text());
    char *pstr = str.toLatin1().data();
    Add_JY(pstr);
    serial.write(pstr);
    qDebug()<<pstr;
}

void Man_S::on_pushButton_5_clicked()
{
    //俯仰角减小
    //组包发送
    QString str =QString("$cmd,manual,4,%1,*hh\r\n").arg(ui->doubleSpinBox->text());
    char *pstr = str.toLatin1().data();
    Add_JY(pstr);
    serial.write(pstr);
    qDebug()<<pstr;
}

void Man_S::on_pushButton_6_clicked()
{
    //极化角增加
    //组包发送
    QString str =QString("$cmd,manual,5,%1,*hh\r\n").arg(ui->doubleSpinBox->text());
    char *pstr = str.toLatin1().data();
    Add_JY(pstr);
    serial.write(pstr);
    qDebug()<<pstr;
}

void Man_S::on_pushButton_7_clicked()
{
    //极化角减小
    //组包发送
    QString str =QString("$cmd,manual,6,%1,*hh\r\n").arg(ui->doubleSpinBox->text());
    char *pstr = str.toLatin1().data();
    Add_JY(pstr);
    serial.write(pstr);
    qDebug()<<pstr;
}
