#include "man_s_p.h"
#include "ui_man_s_p.h"

Man_S_P::Man_S_P(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Man_S_P)
{
    ui->setupUi(this);
}

Man_S_P::~Man_S_P()
{
    delete ui;
}

#include <QDebug>
#include "pub_info.h"
#include <QSerialPort>
extern QSerialPort serial;
void Man_S_P::on_pushButton_clicked()
{
    //手动位置+速度,点动or联动
    QString str="";
    char *pstr = NULL;
    int num = -1;
    num =ui->comboBox->currentIndex();
    qDebug()<<num;
    if( num == 0)
    {
        //方位单轴运动,组包发送
        str = QString("$cmd,dirx,a,%1,%2,%3,%4,%5,%6,*hh\r\n").arg(ui->lineEdit->text()).arg(ui->doubleSpinBox->text())
                .arg(ui->lineEdit_2->text()).arg(ui->doubleSpinBox_2->text())
                .arg(ui->lineEdit_3->text()).arg(ui->doubleSpinBox_3->text());
    }
    else if(num == 1)
    {
        //俯仰单轴运动,组包发送
        str = QString("$cmd,dirx,e,%1,%2,%3,%4,%5,%6,*hh\r\n").arg(ui->lineEdit->text()).arg(ui->doubleSpinBox->text())
                .arg(ui->lineEdit_2->text()).arg(ui->doubleSpinBox_2->text())
                .arg(ui->lineEdit_3->text()).arg(ui->doubleSpinBox_3->text());
    }
    else if(num == 2)
    {
        //极化单轴运动,组包发送
        str = QString("$cmd,dirx,p,%1,%2,%3,%4,%5,%6,*hh\r\n").arg(ui->lineEdit->text()).arg(ui->doubleSpinBox->text())
                .arg(ui->lineEdit_2->text()).arg(ui->doubleSpinBox_2->text())
                .arg(ui->lineEdit_3->text()).arg(ui->doubleSpinBox_3->text());
    }
    else if(num == 3)
    {
        //三轴联动,组包发送
        str = QString("$cmd,dirx,l,%1,%2,%3,%4,%5,%6,*hh\r\n").arg(ui->lineEdit->text()).arg(ui->doubleSpinBox->text())
                .arg(ui->lineEdit_2->text()).arg(ui->doubleSpinBox_2->text())
                .arg(ui->lineEdit_3->text()).arg(ui->doubleSpinBox_3->text());
    }
    else
    {
        //错误
        return;
    }
    pstr = str.toLatin1().data();
    Add_JY(pstr);
    serial.write(pstr);
    qDebug()<<pstr;

}

void Man_S_P::on_pushButton_2_clicked()
{
    //组包发送,停止
    QString str =QString("$cmd,stop,*72\r\n");
    serial.write(str.toLatin1().data());
}
