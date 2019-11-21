#include "para_inface.h"
#include "ui_para_inface.h"

extern Para_Inface *p_i;
int  def_rec_type = 0,rec_type = 0;  //接收机类型 0 - deacon ; 1 - dvb ;
bool def_limit_state = false;  //限位状态
struct BZ_MAGN df_bz ={"11300","1.0"};
struct PLACE_INFO def_place = {"108.90","34.10","180.04"};  //位置信息
struct GENERAL def_general = {"40.00","0.00","2.00","0.00"};
struct ADVANCE def_advance = {
    "0.00","0.00","0.00","180.00","36.00","90.00","-63.50","3.00","0.50","385.00","-25.00","90.00"
    ,"10.00","95.00","-95.00","0.00","0.00","0.00","2.00","1.00"
};
Para_Inface::Para_Inface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Para_Inface)
{
    ui->setupUi(this);
    p_i = this;
}

Para_Inface::~Para_Inface()
{
    delete ui;
}

//接收机类型
 void Para_Inface::updata_rec_type(int i)
 {

     if(i == 0)
     {
         rec_type = 0;
         ui->checkBox_xb->setChecked(true);
         ui->checkBox_dvb->setChecked(false);
     }
     else if( i == 1)
     {
         rec_type = 1;
         ui->checkBox_xb->setChecked(false);
         ui->checkBox_dvb->setChecked(true);
     }
 }
 void Para_Inface::updata_limit_state(bool stat)
{
    if(stat)
    {
        ui->radioButton->setChecked(true);
    }
    else
    {
        ui->radioButton->setChecked(false);
    }
}

void Para_Inface::updata_bz_magn(BZ_MAGN info)
{
    ui->lineEdit_4->setText(QString(info.bz));
    ui->lineEdit_5->setText(QString(info.magnification));
}

void Para_Inface::updata_place_info(PLACE_INFO p)
{
    ui->lineEdit->setText(QString(p.longitude));
    ui->lineEdit_2->setText(QString(p.attitude));
    ui->lineEdit_3->setText(QString(p.course));
}

void Para_Inface::updata_general_par(GENERAL gel)
{
   ui->lineEdit_6->setText(QString(gel.azm_search_range));
   ui->lineEdit_7->setText(QString(gel.azm_star_compensation));
   ui->lineEdit_8->setText(QString(gel.el_star_compensation));
   ui->lineEdit_9->setText(QString(gel.pol_star_compensation));
}

void Para_Inface::updata_advance_par(ADVANCE ad)
{
    //高级参数
    ui->lineEdit_10->setText(QString(ad.azm_zero_compensation));
    ui->lineEdit_11->setText(QString(ad.el_zero_compensation));
    ui->lineEdit_12->setText(QString(ad.pol_zero_compensation));
    ui->lineEdit_13->setText(QString(ad.azm_reset_position));
    ui->lineEdit_14->setText(QString(ad.el_reset_position));
    ui->lineEdit_15->setText(QString(ad.pol_reset_position));
    ui->lineEdit_16->setText(QString(ad.el_down_angle));
    ui->lineEdit_17->setText(QString(ad.el_search_range));
    ui->lineEdit_18->setText(QString(ad.el_search_step));

    ui->lineEdit_19->setText(QString(ad.azm_move_rightlimit));
    ui->lineEdit_20->setText(QString(ad.azm_move_leftlimit));
    ui->lineEdit_21->setText(QString(ad.el_move_uplimit));
    ui->lineEdit_22->setText(QString(ad.el_move_downlimit));
    ui->lineEdit_23->setText(QString(ad.pol_move_islimit));
    ui->lineEdit_24->setText(QString(ad.pol_move_thelimit));

    ui->lineEdit_25->setText(QString(ad.lp_el_compensation));
    ui->lineEdit_26->setText(QString(ad.lp_roll_compensation));
    ui->lineEdit_27->setText(QString(ad.lp_course_compensation));
    ui->lineEdit_28->setText(QString(ad.downlimit_offset_rangle));
    ui->lineEdit_29->setText(QString(ad.azm_search_zero));
}

#include <QDebug>
#include <QMessageBox>
#include <QSerialPort>
#include "pub_info.h"
extern QSerialPort serial;
extern int rec_type;     //接收机类型
void Para_Inface::on_checkBox_xb_pressed()
{
    QMessageBox::StandardButton rb =QMessageBox::information(this,"提示","Do want Current Select?",QMessageBox::Yes|QMessageBox::No);
    if(rb ==QMessageBox::Yes)
    {
        ui->checkBox_xb->setChecked(true);
        ui->checkBox_dvb->setChecked(false);
        QString str =QString("$cmd,set rec,beacon,*74\r\n");
        serial.write(str.toLatin1().data());
    }
}

void Para_Inface::on_checkBox_dvb_pressed()
{
    QMessageBox::StandardButton rb =QMessageBox::information(this,"提示","Do want Current Select?",QMessageBox::Yes|QMessageBox::No);
    if(rb == QMessageBox::Yes)
    {
        ui->checkBox_dvb->setChecked(true);
        ui->checkBox_xb->setChecked(false);
        QString str =QString("$cmd,set rec,dvb,*78\r\n");
        serial.write(str.toLatin1().data());
    }
}

void Para_Inface::on_button_open_limit_clicked()
{
    QMessageBox::StandardButton rb =QMessageBox::information(this,"提示","Do want Open Xwen?",QMessageBox::Yes|QMessageBox::No);
    if(rb == QMessageBox::Yes)
    {
        QString str =QString("$cmd,xwen,on,*43\r\n");
        serial.write(str.toLatin1().data());
    }
}

void Para_Inface::on_button_close_limit_clicked()
{
    QMessageBox::StandardButton rb =QMessageBox::information(this,"提示","Do want Close Xwen?",QMessageBox::Yes|QMessageBox::No);
    if(rb == QMessageBox::Yes)
    {
        QString str =QString("$cmd,xwen,off,*2d\r\n");
        serial.write(str.toLatin1().data());
    }
}

void Para_Inface::on_pushButton_9_clicked()
{
    QString str = "";
    QMessageBox::StandardButton rb =QMessageBox::information(this,"提示","Do want Current Select?",QMessageBox::Yes|QMessageBox::No);
    if(rb == QMessageBox::Yes)
    {
        //需要根据接收机类型来判断
        if(rec_type == 0) //信标机
        {
           str  =QString("$cmd,set beacon,%1,%2,*hh\r\n").
                    arg(ui->lineEdit_4->text()).arg(ui->lineEdit_5->text());
        }
        else if(rec_type == 1)
        {
           str =QString("$cmd,set dvb,%1,%2,*hh\r\n").
                    arg(ui->lineEdit_4->text()).arg(ui->lineEdit_5->text());
        }
        char *pstr = str.toLatin1().data();
        Add_JY(pstr);
        serial.write(pstr);
        qDebug()<<pstr;
    }

}

void Para_Inface::on_pushButton_8_clicked()
{

    QMessageBox::StandardButton rb =QMessageBox::information(this,"提示","Do want Current Select?",QMessageBox::Yes|QMessageBox::No);
    if(rb == QMessageBox::Yes)
    {
        QString str = QString("$cmd,place,%1,%2,%3,*hh\r\n").
                arg(ui->lineEdit->text()).arg(ui->lineEdit_2->text()).arg(ui->lineEdit_3->text());
        char *pstr = str.toLatin1().data();
        Add_JY(pstr);
        serial.write(pstr);
        qDebug()<<pstr;
    }
}

void Para_Inface::on_pushButton_10_clicked()
{
    QMessageBox::StandardButton rb =QMessageBox::information(this,"提示","Do want Current Select?",QMessageBox::Yes|QMessageBox::No);
    if(rb == QMessageBox::Yes)
    {
        QString str = QString("$cmd,set general,%1,%2,%3,%4,*hh\r\n").
                arg(ui->lineEdit_6->text()).arg(ui->lineEdit_7->text()).
                arg(ui->lineEdit_8->text()).arg(ui->lineEdit_9->text());
        char *pstr = str.toLatin1().data();
        Add_JY(pstr);
        serial.write(pstr);
        qDebug()<<pstr;
    }
}

void Para_Inface::on_pushButton_11_clicked()
{
    QMessageBox::StandardButton rb =QMessageBox::information(this,"提示","Do want Current Select?",QMessageBox::Yes|QMessageBox::No);
    if(rb == QMessageBox::Yes)
    {
        QString str = QString("$cmd,set advance,%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15,%16,%17,%18,%19,%20,*hh\r\n")
                .arg(ui->lineEdit_10->text()).arg(ui->lineEdit_11->text()).arg(ui->lineEdit_12->text()).arg(ui->lineEdit_13->text())
                .arg(ui->lineEdit_14->text()).arg(ui->lineEdit_15->text()).arg(ui->lineEdit_16->text()).arg(ui->lineEdit_17->text())
                .arg(ui->lineEdit_18->text()).arg(ui->lineEdit_19->text()).arg(ui->lineEdit_20->text()).arg(ui->lineEdit_21->text())
                .arg(ui->lineEdit_22->text()).arg(ui->lineEdit_23->text()).arg(ui->lineEdit_24->text()).arg(ui->lineEdit_25->text())
                .arg(ui->lineEdit_26->text()).arg(ui->lineEdit_27->text()).arg(ui->lineEdit_28->text()).arg(ui->lineEdit_29->text());
        char *pstr = str.toLatin1().data();
        Add_JY(pstr);
        serial.write(pstr);
        qDebug()<<pstr;
    }

}

void Para_Inface::on_pushButton_5_clicked()
{
    QMessageBox::StandardButton rb =QMessageBox::information(this,"提示","Do want Current Select?",QMessageBox::Yes|QMessageBox::No);
    if(rb == QMessageBox::Yes)
    {
        QString str =QString("$cmd,cps,auto,*29\r\n");
        serial.write(str.toLatin1().data());
    }
}

void Para_Inface::on_pushButton_6_clicked()
{
    QMessageBox::StandardButton rb =QMessageBox::information(this,"提示","Do want Current Select?",QMessageBox::Yes|QMessageBox::No);
    if(rb == QMessageBox::Yes)
    {
        QString str =QString("$cmd,cps,manustar,*25\r\n");
        serial.write(str.toLatin1().data());
    }
}

void Para_Inface::on_pushButton_7_clicked()
{
    QMessageBox::StandardButton rb =QMessageBox::information(this,"提示","Do want Current Select?",QMessageBox::Yes|QMessageBox::No);
    if(rb == QMessageBox::Yes)
    {
        QString str =QString("$cmd,cps,manuend,*5e\r\n");
        serial.write(str.toLatin1().data());
    }
}
//恢复出厂设置
void Para_Inface::on_pushButton_3_clicked()
{
    QMessageBox::StandardButton rb =QMessageBox::information(this,"提示","Do want Current Select?",QMessageBox::Yes|QMessageBox::No);
    if(rb == QMessageBox::Yes)
    {
        QString str =QString("$cmd,restore,*02\r\n");
        serial.write(str.toLatin1().data());

        //发送完复位命令后还需要从新查询一遍有关信息吧
        str.clear();
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
}

void Para_Inface::on_pushButton_4_clicked()
{
    this->hide();
}

