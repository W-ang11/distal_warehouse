#include "auto_widget.h"
#include "ui_auto_widget.h"
#include <QVector>
#include <QTimer>
#include <QTime>
#include "deal_thread.h"

extern Auto_Widget *a_w;
Auto_Widget::Auto_Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Auto_Widget)
{
    ui->setupUi(this);
    setupRealtimeDataDemo(ui->customPlot_3);
    ui->customPlot_3->replot();
    ui->checkBox_current_3->setChecked(true);
    ui->checkBox_reference_3->setChecked(true);
    a_w=this;
}

Auto_Widget::~Auto_Widget()
{
    delete ui;
}

void Auto_Widget::updata_widget_data(SHOW s)
{
    ui->label_cur_azm_4->setText(s.Cur_Azm);
    ui->label_cur_el_4->setText(s.Cur_El);
    ui->label_cur_pol_4->setText(s.Cur_Pol);
    ui->label_per_azm_4->setText(s.Pre_Azm);
    ui->label_per_el_4->setText(s.Pre_El);
    ui->label_per_pol_4->setText(s.Pre_Pol);
}

//画图初始化
void Auto_Widget::setupRealtimeDataDemo(QCustomPlot *customPlot)

{

//#if QT_VERSION < QT_VERSION_CHECK(4, 7, 0)

  //QMessageBox::critical(this, "", "You're using Qt < 4.7, the realtime data demo needs functions that are available with Qt 4.7 to work properly");

//#endif

  //demoName = "Real Time Data Demo";



  // include this section to fully disable antialiasing for higher performance:

  /*

  customPlot->setNotAntialiasedElements(QCP::aeAll);

  QFont font;

  font.setStyleStrategy(QFont::NoAntialias);

  customPlot->xAxis->setTickLabelFont(font);

  customPlot->yAxis->setTickLabelFont(font);

  customPlot->legend->setFont(font);

  */

  customPlot->addGraph(); // blue line

  customPlot->graph(0)->setPen(QPen(Qt::blue));

  customPlot->graph(0)->setName("AGC current");

  //customPlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));

  //customPlot->graph(0)->setAntialiasedFill(false);

  customPlot->addGraph(); // red line

  customPlot->graph(1)->setPen(QPen(Qt::red));

  customPlot->graph(1)->setName("AGC reference");

  //customPlot->graph(0)->setChannelFillGraph(customPlot->graph(1));



  customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);

  customPlot->xAxis->setDateTimeFormat("hh:mm:ss");

  customPlot->xAxis->setAutoTickStep(false);


  customPlot->xAxis->setTickStep(2);

  customPlot->axisRect()->setupFullAxesBox();



  // make left and bottom axes transfer their ranges to right and top axes:

  //connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));

  //connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));



  // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:

  connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));

  dataTimer.start(10); // Interval 0 means to refresh as fast as possible

  customPlot->legend->setVisible(true);

}


extern struct STAR star;
void Auto_Widget::realtimeDataSlot()

{
    //key的单位是秒

    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

    //qsrand(QTime::currentTime().msec() + QTime::currentTime().second() * 10000);

    //使用随机数产生两条曲线

    double value0 = atof(show_info.AGC);
    //double value0 = qrand()%10;

    double value1 = atof(star.limit_info);

    if (ui->checkBox_current_3->isChecked())

        ui->customPlot_3->graph(0)->addData(key, value0);//添加数据1到曲线1

    if (ui->checkBox_reference_3->isChecked())

        ui->customPlot_3->graph(1)->addData(key, value1);//添加数据2到曲线2

    //删除8秒之前的数据。这里的8要和下面设置横坐标宽度的8配合起来

    //才能起到想要的效果，可以调整这两个值，观察显示的效果。

    ui->customPlot_3->graph(0)->removeDataBefore(key-8);


    ui->customPlot_3->graph(1)->removeDataBefore(key-8);

      //自动设定graph(1)曲线y轴的范围，如果不设定，有可能看不到图像
    //也可以用ui->customPlot->yAxis->setRange(up,low)手动设定y轴范围
    ui->customPlot_3->graph(0)->rescaleValueAxis();

    ui->customPlot_3->graph(1)->rescaleValueAxis(true);



    //这里的8，是指横坐标时间宽度为8秒，如果想要横坐标显示更多的时间

    //就把8调整为比较大到值，比如要显示60秒，那就改成60。

    //这时removeDataBefore(key-8)中的8也要改成60，否则曲线显示不完整。

    ui->customPlot_3->xAxis->setRange(key+0.25, 8, Qt::AlignRight);//设定x轴的范围

    ui->customPlot_3->replot();

}


#include <QSerialPort>
extern QSerialPort serial;
void Auto_Widget::on_pushButton_23_clicked()
{
    //组包发送,复位
    QString str =QString("$cmd,reset,*1f\r\n");
    serial.write(str.toLatin1().data());
}

void Auto_Widget::on_pushButton_24_clicked()
{
    //组包发送,对星
    QString str =QString("$cmd,search,*64\r\n");
    serial.write(str.toLatin1().data());
}

void Auto_Widget::on_pushButton_21_clicked()
{
    //组包发送,收藏
    QString str =QString("$cmd,stow,*75\r\n");
    serial.write(str.toLatin1().data());
}

void Auto_Widget::on_pushButton_22_clicked()
{
    //组包发送,展开
    //这里没有展开指令
}

void Auto_Widget::on_pushButton_19_clicked()
{
    //组包发送,停止
    QString str =QString("$cmd,stop,*72\r\n");
    serial.write(str.toLatin1().data());
}
