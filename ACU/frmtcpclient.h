#ifndef FRMTCPCLIENT_H
#define FRMTCPCLIENT_H

#include <QWidget>
#include <QtNetwork>
extern "C"{
#include "utils_hmac.h"
}
#include "httpwindow.h"
#include <synchapi.h>
#include <QThread>
#include "keyBoard.h"

#define  R_NUM               5     //接收缓冲区个数
#define  RBUFF_UNIT          300   //接收缓冲区长度
#define  T_NUM               5     //发送缓冲区个数
#define  TBUFF_UNIT          1024   //发送缓冲区长度
#define  C_NUM               5     //命令缓冲区个数
#define  CBUFF_UNIT          300   //命令缓冲区长度
#define  MQTT_TxData(x)       u2_TxData(x)                                         //串口2负责数据发送
#define  PRODUCTKEY           ui->ProductKey->text().toLocal8Bit().data()                                        //产品ID
#define  PRODUCTKEY_LEN       strlen(PRODUCTKEY)                                   //产品ID长度
#define  DEVICENAME           ui->DeviceName->text().toLocal8Bit().data()                                               //设备名
#define  DEVICENAME_LEN       strlen(DEVICENAME)                                   //设备名长度
#define  DEVICESECRE          ui->DeviceSecret->text().toLocal8Bit().data()        //设备秘钥
#define  DEVICESECRE_LEN      strlen(DEVICESECRE)                                  //设备秘钥长度
#define  S_TOPIC_NAME_         ui->S_TOPIC_NAME->text().toLocal8Bit().data()       //需要订阅的主题
#define  P_TOPIC_NAME_         ui->P_TOPIC_NAME->text().toLocal8Bit().data()       //需要发布的主题
/*2019.11.11所有连接信息，订阅信息，和推送信息，通过宏定义固定，使用时直接修改
不再使用lineEdit的连接和订阅
*/

namespace Ui {
class frmTcpClient;
}

class frmTcpClient : public QWidget
{
    Q_OBJECT

public:
    explicit frmTcpClient(QWidget *parent = 0);
    ~frmTcpClient();

private:
    Ui::frmTcpClient *ui;
    bool isOk;
    QTcpSocket *tcpSocket;
    QTimer *timer;
    QTimer *m_timer;
private:
    unsigned char  MQTT_RxDataBuf[R_NUM][RBUFF_UNIT];       //外部变量声明，数据的接收缓冲区,所有服务器发来的数据，存放在该缓冲区,缓冲区第一个字节存放数据长度
    unsigned char *MQTT_RxDataInPtr;                        //外部变量声明，指向缓冲区存放数据的位置
    unsigned char *MQTT_RxDataOutPtr;                       //外部变量声明，指向缓冲区读取数据的位置
    unsigned char *MQTT_RxDataEndPtr;                       //外部变量声明，指向缓冲区结束的位置
    unsigned char  MQTT_TxDataBuf[T_NUM][TBUFF_UNIT];       //外部变量声明，数据的发送缓冲区,所有发往服务器的数据，存放在该缓冲区,缓冲区第一个字节存放数据长度
    unsigned char *MQTT_TxDataInPtr;                        //外部变量声明，指向缓冲区存放数据的位置
    unsigned char *MQTT_TxDataOutPtr;                       //外部变量声明，指向缓冲区读取数据的位置
    unsigned char *MQTT_TxDataEndPtr;                       //外部变量声明，指向缓冲区结束的位置
    unsigned char  MQTT_CMDBuf[C_NUM][CBUFF_UNIT];          //外部变量声明，命令数据的接收缓冲区
    unsigned char *MQTT_CMDInPtr;                           //外部变量声明，指向缓冲区存放数据的位置
    unsigned char *MQTT_CMDOutPtr;                          //外部变量声明，指向缓冲区读取数据的位置
    unsigned char *MQTT_CMDEndPtr;                          //外部变量声明，指向缓冲区结束的位置

    char ClientID[128];                                          //存放客户端ID的缓冲区
    int  ClientID_len;                                           //存放客户端ID的长度

    char Username[128];                                          //存放用户名的缓冲区
    int  Username_len;											 //存放用户名的长度

    char Passward[128];                                          //存放密码的缓冲区
    int  Passward_len;											 //存放密码的长度

    char ServerIP[128];                                          //存放服务器IP或是域名
    int  ServerPort;                                             //存放服务器的端口号

    int   Fixed_len;                       					     //固定报头长度
    int   Variable_len;                     					 //可变报头长度
    int   Payload_len;                       					 //有效负荷长度
    char  temp_buff[TBUFF_UNIT];						 //临时缓冲区，构建报文用

    char Ping_flag;           //ping报文状态      0：正常状态，等待计时时间到，发送Ping报文
                              //ping报文状态      1：Ping报文已发送，当收到 服务器回复报文的后 将1置为0
    char Connect_flag;        //同服务器连接状态  0：还没有连接服务器  1：连接上服务器了
    char ConnectPack_flag;    //CONNECT报文状态   1：CONNECT报文成功
    char SubcribePack_flag;   //订阅报文状态      1：订阅报文成功

private slots:
    void initForm();
    void initConfig();
    void saveConfig();
    void append(int type, const QString &data, bool clear = false);

    void connected();
    void disconnected();
    void readData();
    void sendData(const QString &data);

private slots:
    void on_btnConnect_clicked();
    void on_btnSave_clicked();
    void on_btnClear_clicked();
    void on_btnSend_clicked();
    void on_pushButton_clicked();

    void on_Subscribe_clicked();

    void on_Publish_clicked();
    void TimerTimeout();


private:
    void MQTT_Buff_Init(void);
    void AliIoT_Parameter_Init(void);
    void MQTT_ConectPack(void);
    void MQTT_Subscribe(char *, int);
    void MQTT_PingREQ(void);
    void MQTT_PublishQs0(char *, char *, int);
    void MQTT_DealPushdata_Qs0(unsigned char *);
    void TxDataBuf_Deal(unsigned char *, int);
    void CMDBuf_Deal(unsigned char *, int);
    void url_function(QString buffer);
    void InitTimer();

private:
    void Cloud_Connected();              //连接云服务器
    void Cloud_OTA_Post();               //版本号推送
    void Cloud_Subcribe();               //订阅主题
    void Cloud_Subcribe_OTA();           //订阅版本主题
    void Cloud_Things_Post();            //发布事件推送
    void Cloud_OTA_Post_Progress();      //发布升级进度推送
    void Cloud_Ping();                   //心跳包
    /*设备三元组，productkey*/
    char FIX_PRODUCTKEY[20] = "a1gf7MN0U03";
    size_t FIX_PRODUCTKEYL_LEN = strlen(FIX_PRODUCTKEY);
    /*设备三元组，devicename*/
    char FIX_DEVICENAME[20] = "Cont";
    size_t FIX_DEVICENAME_LEN = strlen(FIX_DEVICENAME);
    /*设备三元组，devicesecert*/
    char FIX_DEVICESECERT[50] = "unWlF8hnimruqvfBza2COVYcK82JnCqd";
    size_t FIX_DEVICESECERT_LEN  =  strlen(FIX_DEVICESECERT);
    /*需要订阅的主题*/
    char FIX_SUBSCRIBE_TOPIC[200] = "/sys/a1gf7MN0U03/Cont/thing/service/property/set";
    /*订阅的设备升级主题*/
    char FIX_SUBSCRIBE_OTA_TOPIC[200] = "/ota/device/upgrade/a1gf7MN0U03/Cont";
    /*需要发布的主题*/
    char FIX_POST_TOPIC[200] = "/sys/a1gf7MN0U03/Cont/thing/event/property/post";
    /*OTA升级进度上报*/
    char FIX_OTA_PROGRAM[200] = "/ota/device/progress/a1gf7MN0U03/Cont";
    /*OTA版本号的推送*/
    char FIX_VERSION[200] = "/ota/device/inform/a1gf7MN0U03/Cont";
    /*ip and port*/
    QString TCP_Client_IP = "a1gf7MN0U03.iot-as-mqtt.cn-shanghai.aliyuncs.com";
    int TCP_Client_Port = 1883;
public:
        void Cloud_main();
};
#endif // FRMTCPCLIENT_H
