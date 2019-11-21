#include "war_limit.h"
#include "ui_war_limit.h"

extern War_Limit *w_l;

static char LIMIT[32][32] ={
    {""},{""},{""},{""},{""},{""},
    {"极化零位"},{"俯仰零位"},{"方位零位"},{"极化逆限位"},{"极化顺限位"},
    {"俯仰下线"},{"俯仰上线"},{"方位左限"},{"方位右限"},{""}
};

static char WARING[32][32]={
    {"方位驱动异常"},{"俯仰驱动异常"},{"极化驱动异常"},{"GPS未连接"},{"罗盘未连接"},{"信标接收机未连接"},
    {"DVB未连接"},{"极化找零失败"},{"方位找零失败"},{"俯仰找零失败"},{"发送命令频繁"},{"数据越界３"},
    {"命令不符"},{"校验和错误"},{"ZTC未连接"},{"未定义"},{"数组越界１"},{"数据越界１"},{"数据越界２"},
    {"硬件错误"},{""},{""},{""}
};
//#include "string.h"



War_Limit::War_Limit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::War_Limit)
{
    ui->setupUi(this);
    item = new QStandardItemModel(this);
    init_tableview();
    w_l =this;
}

War_Limit::~War_Limit()
{
    delete ui;
}

#include <QDateTime>
void War_Limit::show_limit_info(int value)
{

    int a[32];
    int c = 0x00000001;
    QString str="";
    memset(a,0,32*4);
    for(int i = 0 ;i < 32 ; i++)
    {
        if((value&c) == c)
        {
            a[i] = 1;
        }
        else
        {
            a[i] = 0;
        }
        c =  c << 1;
    }
    for(int j=0 ;j < 32 ;j++)
    {
        if(a[j] == 1)
        {
            //str.append(LIMIT[j]);
            str += QString(LIMIT[j]).append(QDateTime::currentDateTime().toString(" :yyyy-MM-dd:hh:mm:ss\r\n"));
        }
        else //当对应位不为１则相当于没有相应的限位信息不处理就可以啦
        {

        }
    }
    ui->textBrowser_2->setText(str);
}
void War_Limit::show_waring_info(int value)
{
    int a[32];
    int c = 0x00000001;
    QString str="";
    memset(a,0,32*4);
    for(int i = 0 ;i < 32 ; i++)
    {
        if((value&c) == c)
        {
            a[i] = 1;
        }
        else
        {
            a[i] = 0;
        }
        c =  c << 1;
    }
    for(int j=0 ;j < 32 ;j++)
    {
        if(a[j] == 1)
        {
            //str.append(WARING[j]); //这个没有显示时间戳
            str += QString(WARING[j]).append(QDateTime::currentDateTime().toString(" :yyyy-MM-dd:hh:mm:ss\r\n"));
        }
        else //当对应位不为１则相当于没有相应的告警信息不处理就可以啦
        {

        }
    }
    ui->textBrowser->setText(str);
}
void War_Limit:: ant_state(int value)
{
    //天线状态我就使用switch语句来判断吧
    switch(value)
    {
        //复位
        case 17:item->setItem(6, 1, new QStandardItem("复位开始"));break;
        case 18:item->setItem(6, 1, new QStandardItem("复位失败"));break;
        case 19:item->setItem(6, 1, new QStandardItem("复位完成"));break;
        case 20:item->setItem(6, 1, new QStandardItem("复位中断"));break;
        //收藏
        case 33:item->setItem(6, 1, new QStandardItem("收藏开始"));break;
        case 34:item->setItem(6, 1, new QStandardItem("收藏失败"));break;
        case 35:item->setItem(6, 1, new QStandardItem("收藏完成"));break;
        case 36:item->setItem(6, 1, new QStandardItem("收藏中断"));break;
        //对星
        case 49:item->setItem(6, 1, new QStandardItem("对星开始"));break;
        case 50:item->setItem(6, 1, new QStandardItem("对星失败"));break;
        case 51:item->setItem(6, 1, new QStandardItem("对星完成"));break;
        case 52:item->setItem(6, 1, new QStandardItem("对星中断"));break;
        //罗盘
        case 65:item->setItem(6, 1, new QStandardItem("罗盘标定开始"));break;
        case 66:item->setItem(6, 1, new QStandardItem("罗盘标定中"));break;
        case 67:item->setItem(6, 1, new QStandardItem("罗盘标定完成"));break;
        case 68:item->setItem(6, 1, new QStandardItem("罗盘标定失败"));break;
        case 69:item->setItem(6, 1, new QStandardItem("罗盘标定中断"));break;
        //跟踪
        case 81:item->setItem(6, 1, new QStandardItem("跟踪中"));break;
        case 83:item->setItem(6, 1, new QStandardItem("跟踪完成"));break;
        case 84:item->setItem(6, 1, new QStandardItem("跟踪中断"));break;

        case 97:item->setItem(6, 1, new QStandardItem("手动速度"));break;
        case 98:item->setItem(6, 1, new QStandardItem("手动位置"));break;
        case 144:item->setItem(6, 1, new QStandardItem("天线空闲"));break;
        case 238:item->setItem(6, 1, new QStandardItem("主机重启"));break;
        default : break;
    }
}
void War_Limit::updata_widget_data(SHOW s)
{
    //设置tableview
    item->setItem(0, 1, new QStandardItem(s.Pre_Azm));
    item->setItem(1, 1, new QStandardItem(s.Pre_El));
    item->setItem(2, 1, new QStandardItem(s.Pre_Pol));
    item->setItem(3, 1, new QStandardItem(s.Cur_Azm));
    item->setItem(4, 1, new QStandardItem(s.Cur_El));
    item->setItem(5, 1, new QStandardItem(s.Cur_Pol));
    item->setItem(7, 1, new QStandardItem(s.Car_A));
    item->setItem(8, 1, new QStandardItem(s.Car_E));
    item->setItem(9, 1, new QStandardItem(s.Car_P));
    item->setItem(10, 1, new QStandardItem(s.longitude));
    item->setItem(11, 1, new QStandardItem(s.attitude));
    switch(s.Gps_Stat)
    {
        case 0: item->setItem(12, 1, new QStandardItem("GPS无效"));break;
        case 1: item->setItem(12, 1, new QStandardItem("GPS锁定"));break;
        case 2: item->setItem(12, 1, new QStandardItem("GPS未锁定"));break;
        default : break;
    }
    //天线状态
    ant_state(s.Ant_Stat);
    item->setItem(13, 1, new QStandardItem(s.AGC));
    //设置textBrowser
    show_limit_info(s.Limit_Info);
    show_waring_info(s.Alarm_Info);

}


void War_Limit::updata_get_sn_info(QString t)
{
    ui->label->setText(t);
}
void War_Limit::init_tableview()
{
    item->setItem(0, 0, new QStandardItem("预置方位"));item->setItem(0, 1, new QStandardItem("000.00"));item->setItem(1, 0, new QStandardItem("预置俯仰"));
    item->setItem(1, 1, new QStandardItem("000.00"));item->setItem(2, 0, new QStandardItem("预置极化"));item->setItem(2, 1, new QStandardItem("000.00"));
    item->setItem(3, 0, new QStandardItem("当前方位"));item->setItem(3, 1, new QStandardItem("000.00"));item->setItem(4, 0, new QStandardItem("当前俯仰"));
    item->setItem(4, 1, new QStandardItem("000.00"));item->setItem(5, 0, new QStandardItem("当前极化"));item->setItem(5, 1, new QStandardItem("000.00"));
    item->setItem(6, 0, new QStandardItem("天线状态"));item->setItem(6, 1, new QStandardItem(""));item->setItem(7, 0, new QStandardItem("载体航向"));
    item->setItem(7, 1, new QStandardItem("000.00"));item->setItem(8, 0, new QStandardItem("载体俯仰"));item->setItem(8, 1, new QStandardItem("000.00"));
    item->setItem(9, 0, new QStandardItem("载体横滚"));item->setItem(9, 1, new QStandardItem("000.00"));item->setItem(10, 0, new QStandardItem("经度"));
    item->setItem(10, 1, new QStandardItem("000.00"));item->setItem(11, 0, new QStandardItem("纬度"));item->setItem(11, 1, new QStandardItem("000.00"));
    item->setItem(12, 0, new QStandardItem("GPS状态"));item->setItem(12, 1, new QStandardItem("GPS无效"));item->setItem(13, 0, new QStandardItem("AGC电平"));
    item->setItem(13, 1, new QStandardItem("000.00"));
    ui->tableView->setModel(item);
}
