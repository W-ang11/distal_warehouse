#ifndef TAB_WIDGET_H
#define TAB_WIDGET_H

#include <QMainWindow>
#include <QTimer>
#include "par_config.h"

namespace Ui {
class Tab_Widget;
}

extern QSerialPort serial;

class Tab_Widget : public QMainWindow
{
    Q_OBJECT

public:
    explicit Tab_Widget(QWidget *parent = nullptr);
    ~Tab_Widget();

private slots:
    void connect_serialport();
    void disconnect_serialport();
    void my_timerout();
    void handleError(QSerialPort::SerialPortError error);

private:
    Ui::Tab_Widget *ui;
    QTimer *my_timer;
    Par_Config *config_wigdet;
};

#endif // TAB_WIDGET_H
