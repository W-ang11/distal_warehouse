#include "man_p.h"
#include "ui_man_p.h"

Man_P::Man_P(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Man_P)
{
    ui->setupUi(this);
}

Man_P::~Man_P()
{
    delete ui;
}

#include <QDebug>
#include "pub_info.h"
#include <QSerialPort>
extern QSerialPort serial;
void Man_P::on_pushButton_2_clicked()
{
    //停止
    //组包发送
    QString str =QString("$cmd,stop,*72\r\n");
    serial.write(str.toLatin1().data());
}

void Man_P::on_pushButton_clicked()
{
    //手动位置模式
    //组包发送
    QString str =QString("$cmd,dir,%1,%2,%3,*hh\r\n").arg(ui->lineEdit_per_Az->text())
            .arg(ui->lineEdit_per_El->text()).arg(ui->lineEdit_per_Pol->text());
    char *pstr = str.toLatin1().data();
    Add_JY(pstr);
    serial.write(pstr);
    qDebug()<<pstr;
}
