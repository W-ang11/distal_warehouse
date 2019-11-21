#ifndef AUTO_WIDGET_H
#define AUTO_WIDGET_H
#include <QWidget>
#include <QTimer>
#include "qcustomplot.h"
#include "pub_info.h"

namespace Ui {
class Auto_Widget;
}

class Auto_Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Auto_Widget(QWidget *parent = nullptr);
    ~Auto_Widget();
    void updata_widget_data(SHOW s);
    void setupRealtimeDataDemo(QCustomPlot *customPlot);
signals:

public slots:
    void realtimeDataSlot();

private slots:
    void on_pushButton_23_clicked();

    void on_pushButton_24_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_22_clicked();

    void on_pushButton_19_clicked();

private:
    Ui::Auto_Widget *ui;

    //定时器，周期调用realtimeDataSlot槽，实现动态数据添加到曲线
    QTimer dataTimer;

};


#endif // AUTO_WIDGET_H
