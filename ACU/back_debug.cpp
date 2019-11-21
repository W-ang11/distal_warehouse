#include "back_debug.h"
#include "ui_back_debug.h"
#include <QDebug>
#include <QSerialPort>
#include "tab_widget.h"

War_Limit *w_l=nullptr;   //保存页面对象指针
Man_Widget *m_w=nullptr;
Auto_Widget *a_w=nullptr;
Para_Inface *p_i=nullptr;

Back_Debug::Back_Debug(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Back_Debug)

{
    ui->setupUi(this);
    QPalette p = palette();   //创建一个调色板对象
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::green);
    ui->plainTextEdit->setPalette(p);

    thread = new Deal_Thread(this);
    connect(thread,&Deal_Thread::deal_finish,this ,&Back_Debug::close_thread);
    connect(this,&Back_Debug::destroyed,[=](){ this->thread->quit();this->thread->wait();});
    connect(thread,&Deal_Thread::once_show_ok,this,&Back_Debug::updata_widget);
    connect(thread,&Deal_Thread::once_get_sn_ok,this,&Back_Debug::updata_get_sn_info);
    connect(thread,&Deal_Thread::once_get_rec_type,this,&Back_Debug::updata_rec_type);
    connect(thread,&Deal_Thread::once_get_limit_state,this,&Back_Debug::updata_limit_state);
    connect(thread,&Deal_Thread::once_get_bz_magn,this,&Back_Debug::updata_bz_magn);
    connect(thread,&Deal_Thread::once_get_place,this,&Back_Debug::updata_place_info);
    connect(thread,&Deal_Thread::once_get_general_par,this,&Back_Debug::updata_general_par);
    connect(thread,&Deal_Thread::once_get_advance_par,this,&Back_Debug::updata_advance_par);

     //时间戳这个选项在这里暂时不用因为在用的时候有一点问题似乎要等数据读完,要用的时候删除这句话就可以
    ui->checkBox_4->setCheckable(false);

    ui->checkbox_1->setChecked(true);
    connect(&serial, &QSerialPort::readyRead, this, &Back_Debug::readData);
    connect(this, &Back_Debug::getData, this, &Back_Debug::writedata);

    //清楚接受窗口
    connect(ui->clearn_button,&QPushButton::clicked,this,&Back_Debug::clearrecive_window);
}

Back_Debug::~Back_Debug()
{
    delete ui;
}

void Back_Debug::updata_widget(SHOW s)
{
    w_l->updata_widget_data(s);
    m_w->updata_widget_data(s);
    a_w->updata_widget_data(s);
}

//这里是跟新天线软硬件版本号
void Back_Debug::updata_get_sn_info(QString t)
{
    w_l->updata_get_sn_info(t);
}

//-----------------------------operation para_inface
void Back_Debug::updata_rec_type(int i)
{
    p_i->updata_rec_type(i);
}

void Back_Debug::updata_limit_state(bool stat)
{
    p_i->updata_limit_state(stat);
}
void Back_Debug::updata_bz_magn(BZ_MAGN bz_info)
{
    p_i->updata_bz_magn(bz_info);
}
void Back_Debug::updata_place_info(PLACE_INFO pl)
{
    p_i->updata_place_info(pl);
}
void Back_Debug::updata_general_par(GENERAL g)
{
    p_i->updata_general_par(g);
}
void Back_Debug::updata_advance_par(ADVANCE adv)
{
    p_i->updata_advance_par(adv);
}
//-------------------------------end
void Back_Debug::keyPressEvent(QKeyEvent *evt)
{

    //如果按下回车键，发送信号
    if(evt->key() == Qt::Key_Return)
    {
       emit getData(ui->lineEdit->text().toLocal8Bit());
    }

}

void Back_Debug::on_senddata_button_clicked()
{
    //这里发送信号就可以了
    emit getData(ui->lineEdit->text().toLocal8Bit());
}


void Back_Debug::writedata(const QByteArray &data)
{
    QString temp(data);
    QByteArray str="";
    //是否发送新行,默认情况下是发送新行的
    if(ui->checkbox_1->isChecked())
    {
        temp.append("\r\n");
    }
    else
    {
        //
    }

    //是否按十六进制发送
    if(ui->checkbox_2->isChecked())
    {
         str=temp.toUtf8().toHex();
    }
    else
    {
         str=temp.toUtf8();
    }
    serial.write(str);
}

#include <string.h>
int flag=0,len=0;
char buf[200]="",buf1[200]="";
extern "C" void analysis(char *str)
{
    for(unsigned int i=0;i < strlen(str);i++)
    {

        if(flag == 0 && str[i]== '$' )
        {
            flag = 1;
            buf[len++] = str[i];
        }
        else if(flag == 1)
        {
           buf[len++] = str[i];
           if(len == 4)
           {

               if((strncmp(buf,"$SHO",4) == 0) || (strncmp(buf,"$cmd",4) == 0))
               {
                    flag = 2;
               }
               else
               {
                   flag = 0;
                   len = 0;
                   memset(buf ,0, 200);
               }
           }
        }
        else if (flag == 2)
        {
            buf[len++] = str[i];
            if((str[i] == '\n')&&(len <200)) //一帧数据结束
            {
                flag =3;
                len = 0;
                strcpy(buf1,buf);
                memset(buf,0,200);
            }
            else if ( len > 200)
            {

                flag = 0;
                len =0;
                memset(buf,0,200);
            }

        }
    }
}
void Back_Debug::readData()
{

    QString temp = "",str="";
    QByteArray data = serial.readAll();  //读取设备所有剩余数据
    if (data.length() <= 0) {
            return;
        }
    //-------------------在这里判断处理吗---------------------------------
     char *str1 = data.data();
     analysis(str1);
     //我这里开一个线程来处理吗
     if(flag == 3)
     {
         thread->start(); //启动线程处理
        // memset(buf,0,200);
         flag = 0;
     }
    //------------------------------------------------------------------

    //是否16进制显示数据
    if(ui->checkBox_3->isChecked())
    {
        QString hex=data.toHex();;
        for (int i = 0; i < hex.length(); i = i + 2) {
                temp += hex.mid(i, 2) + " ";
            }
    }
    else
    {
       temp.prepend(data);
    }

    //是否显示时间戳
    if(ui->checkBox_4->isChecked())
    {
        QDateTime local(QDateTime::currentDateTime());
        str += QString(tr("%1 [%2]").arg(temp).arg(local.toString("yyyy-MM-dd:hh:mm:ss")));
    }
    else
    {
        str += QString(tr("%1").arg(temp));
    }
    ui->plainTextEdit->insertPlainText(str); //在当前光标处插入文本
}

void Back_Debug::clearrecive_window()
{
    ui->plainTextEdit->clear();
}

void Back_Debug::close_thread()
{

    this->thread->quit();
    this->thread->wait();
}

#include <QFile>
#include <QFileDialog>
void Back_Debug::on_save_button_clicked()
{
    QFile file;
    //将文本编辑后的文本作为纯文本返回
    QString str=ui->plainTextEdit->toPlainText();
    QString filename=QFileDialog::getSaveFileName(this, QString("保存路径"), QString("../"),QString("TEXT(*.txt)"));
    if(true ==filename.isEmpty())
    {
        return ;
    }
    else
    {

        file.setFileName(filename);
        if(true == file.open(QIODevice::WriteOnly))
        {
            file.write(str.toUtf8());
            file.close();
        }
        else
        {
            return ;
        }
    }
    //使用这个保存文件是有先买你一个警告但是不影响使用,百度到的答案也说了可以忽略
    //Gtk-Message: 19:22:27.106: GtkDialog mapped without a transient parent. This is discouraged.
}
