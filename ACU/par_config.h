#ifndef PAR_CONFIG_H
#define PAR_CONFIG_H

#include <QWidget>
#include "QDebug"
#include "QSerialPort"
#include "QSerialPortInfo"

namespace Ui {
class Par_Config;
}

class Par_Config : public QWidget
{
    Q_OBJECT

public:
    struct Settings {
            QString name;
            qint32 baudRate;
            QSerialPort::DataBits dataBits;
            QSerialPort::Parity parity;
            QSerialPort::StopBits stopBits;
        };

    explicit Par_Config(QWidget *parent = nullptr);
    ~Par_Config();
    Settings cursettings() const;


private slots:
    void baud_custom(int idx);
    void apply_button_set();

private:   
    Ui::Par_Config *ui;
    Settings settings;       //定义一个Settings 结构体的变量

    void updateSettings();
};

#endif // PAR_CONFIG_H
