#include "para_adj.h"
#include "ui_para_adj.h"
#include <QMessageBox>

Para_Adj::Para_Adj(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Para_Adj)
{
    ui->setupUi(this);
    para_inface.setWindowTitle("参数调整");
    //设置它为模态窗口,当该窗口打开时其它窗口不接受输入,必须将该窗口关闭才可以
    para_inface.setWindowModality(Qt::ApplicationModal);

    connect(ui->pushButton,&QPushButton::clicked,this,&Para_Adj::password_deal);

}

Para_Adj::~Para_Adj()
{
    delete ui;
}



#include <QDebug>
#include "pub_info.h"
#include <QSerialPort>
extern QSerialPort serial;
extern int rec_type;     //接收机类型
void Para_Adj::password_deal()
{

    //如果密码相同,假设密码是111111
    if(ui->lineEdit->text().toInt() == PASSWORD)
    {
        para_inface.show();
        //这里需要发送一堆查询命令,同时还要更新参数调整页的值和状态
        QString str = "";
        str = QString("$cmd,get rec type,*70\r\n");
        serial.write(str.toLatin1().data());    //1.发送查询当前接收机
        qDebug()<<str;
        str.clear();
        str = QString("$cmd,get xwen,*38\r\n");
        serial.write(str.toLatin1().data());    //2.发送查询当前限位状态
        qDebug()<<str;
        str.clear();
        //这里应该是根据接收机的类型发送不同的查询指令
        //$cmd,get beacon,*38\r\n或$cmd,get dvb,*4c\r\n
        if(rec_type == 0)
        {
            str = QString("$cmd,get beacon,*38\r\n");
        }
        else if(rec_type == 1)
        {
            str = QString("$cmd,get dvb,*4c\r\n");
        }
        serial.write(str.toLatin1().data());    //3.发送查询当前本振和放大倍数
        qDebug()<<str;
        str.clear();
        str =QString("$cmd,get place,*47\r\n");
        serial.write(str.toLatin1().data());    //4.发送查询当前位置信息
        qDebug()<<str;
        str.clear();
        str =QString("$cmd,get general,*4a\r\n");
        serial.write(str.toLatin1().data());    //5.发送查询一般参数
        qDebug()<<str;
        str.clear();
        str = QString("$cmd,get advance,*46\r\n");
        serial.write(str.toLatin1().data());    //6.发送查询高级参数
        qDebug()<<str;
        str.clear();
        //暂时就发送这6条查询命令,后面如果需要可以继续增加
    }
    else
    {
        //密码不同时给出警告
        QMessageBox::warning(this,tr("警告!"),tr("密码输入错误!"),QMessageBox::Yes);
    }
}
