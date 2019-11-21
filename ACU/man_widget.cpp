#include "man_widget.h"
#include "ui_man_widget.h"
#include "sql_oper.h"

extern Man_Widget *m_w;
Man_Widget::Man_Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Man_Widget)
{

    ui->setupUi(this);
    ui->tabWidget_1->setCurrentIndex(0);
    m_w =this;
}

void Man_Widget:: ant_state(int value)
{
    //天线状态我就使用switch语句来判断吧
    switch(value)
    {
        //复位
        case 17:ui->label_antenna_state->setText("复位开始");break;
        case 18:ui->label_antenna_state->setText("复位失败");break;
        case 19:ui->label_antenna_state->setText("复位完成");break;
        case 20:ui->label_antenna_state->setText("复位中断");break;
        //收藏
        case 33:ui->label_antenna_state->setText("收藏开始");break;
        case 34:ui->label_antenna_state->setText("收藏失败");break;
        case 35:ui->label_antenna_state->setText("收藏完成");break;
        case 36:ui->label_antenna_state->setText("收藏中断");break;
        //对星
        case 49:ui->label_antenna_state->setText("对星开始");break;
        case 50:ui->label_antenna_state->setText("对星失败");break;
        case 51:ui->label_antenna_state->setText("对星完成");break;
        case 52:ui->label_antenna_state->setText("对星中断");break;
        //罗盘
        case 65:ui->label_antenna_state->setText("罗盘标定开始");break;
        case 66:ui->label_antenna_state->setText("罗盘标定中");break;
        case 67:ui->label_antenna_state->setText("罗盘标定完成");break;
        case 68:ui->label_antenna_state->setText("罗盘标定失败");break;
        case 69:ui->label_antenna_state->setText("罗盘标定中断");break;
        //跟踪
        case 81:ui->label_antenna_state->setText("跟踪中");break;
        case 83:ui->label_antenna_state->setText("跟踪完成");break;
        case 84:ui->label_antenna_state->setText("跟踪中断");break;

        case 97:ui->label_antenna_state->setText("手动速度");break;
        case 98:ui->label_antenna_state->setText("手动位置");break;
        case 144:ui->label_antenna_state->setText("天线空闲");break;
        case 238:ui->label_antenna_state->setText("主机重启");break;
        default : break;
    }
}

//这里更新的是卫星参数
void Man_Widget::updata_antenna_select(STAR s)
{

    ui->lineEdit_name->setText(s.name);
    ui->lineEdit_pl->setText(s.xb_pl);
    ui->lineEdit_zb_pl->setText(s.zb_pl);
    ui->lineEdit_zb_sl->setText(s.zb_sl);
    ui->lineEdit_longitude->setText(s.longitude);
    ui->lineEdit_pol_model->setText(s.pol_model);
}

//这里更新的是信息参数
void Man_Widget::updata_widget_data(SHOW s)
{
    ui->label_agc->setText(s.AGC);
    //天线状态
    ant_state(s.Ant_Stat);
    ui->label_cur_azm_5->setText(s.Cur_Azm);
    ui->label_cur_el_5->setText(s.Cur_El);
    ui->label_cur_pol_5->setText(s.Cur_Pol);
    ui->label_per_azm_5->setText(s.Pre_Azm);
    ui->label_per_el_5->setText(s.Pre_El);
    ui->label_per_pol_5->setText(s.Pre_Pol);
}

//在这里处理卫星选择操作
void Man_Widget::on_button_select_antenna_clicked()
{
    Sql_Oper *sql_oper = new Sql_Oper();
    sql_oper->setWindowTitle("卫星选择");
    //设置它为模态窗口,当该窗口打开时其它窗口不接受输入,必须将该窗口关闭才可以
    sql_oper->setWindowModality(Qt::ApplicationModal);
    //手动销毁该窗口
    connect(sql_oper,&Sql_Oper::destroyed,[=](){delete sql_oper;});
    sql_oper->show();
}
