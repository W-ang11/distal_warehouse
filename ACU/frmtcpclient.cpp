#include "frmtcpclient.h"
#include "ui_frmtcpclient.h"
#include "quiwidget.h"
#include "pub_info.h"

extern SHOW show_info;
static QString url;                     //定义URL
int connect_flag = 0;                   //云连接
int OTA_flag = 0;                       //推送版本号
int Cloud_Subcribe_flag = 0;            //订阅主题
int Cloud_Subcribe_OTA_flag	= 0;        //订阅升级主题
int Cloud_Things_Post_flag	= 0;        //发布事件主题
int Cloud_OTA_Post_Progress_flag = 0;   //升级进度主题

frmTcpClient::frmTcpClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmTcpClient)
{
    ui->setupUi(this);
    this->initForm();
    this->initConfig();

    ui->pushButton->hide();     //隐藏连接阿里云
    ui->ProductKey->hide();     //隐藏设备三元组
    ui->DeviceName->hide();     //隐藏设备三元组
    ui->DeviceSecret->hide();   //隐藏设备三元组
    ui->label->hide();          //隐藏设备三元组
    ui->label_2->hide();        //隐藏设备三元组
    ui->label_3->hide();        //隐藏设备三元组
    ui->S_TOPIC_NAME->hide();   //隐藏订阅信息
    ui->P_TOPIC_NAME->hide();   //隐藏推送信息
    ui->Things->hide();         //隐藏事件
    ui->label_10->hide();       //隐藏事件label
    ui->label_11->hide();       //隐藏事件label
    ui->label_12->hide();       //隐藏事件label
    ui->Subscribe->hide();      //隐藏订阅按钮
    ui->Publish->hide();        //隐藏推送按钮
    ui->btnSave->hide();        //隐藏保存按钮
    ui->txtServerIP->hide();    //隐藏IP
    ui->txtServerPort->hide();  //隐藏端口号
    ui->labServerIP->hide();    //隐藏IP lab
    ui->labServerPort->hide();  //隐藏port lab
    ui->cboxInterval->hide();   //
    ui->ckAscii->hide();        //隐藏ascii
    ui->ckDebug->hide();        //隐藏模拟设备
    ui->ckAutoSend->hide();     //隐藏定时发送
}

frmTcpClient::~frmTcpClient()
{
    delete ui;
}

void frmTcpClient::initForm()
{
    isOk = false;
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(disconnected()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readData()));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_btnSend_clicked()));

    ui->cboxInterval->addItems(App::Intervals);
    ui->cboxData->addItems(App::Datas);
}

void frmTcpClient::initConfig()
{
    ui->ckHexSend->setChecked(App::HexSendTcpClient);
    connect(ui->ckHexSend, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));

    ui->ckHexReceive->setChecked(App::HexReceiveTcpClient);
    connect(ui->ckHexReceive, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));

    ui->ckAscii->setChecked(App::AsciiTcpClient);
    connect(ui->ckAscii, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));

    ui->ckDebug->setChecked(App::DebugTcpClient);
    connect(ui->ckDebug, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));

    ui->ckAutoSend->setChecked(App::AutoSendTcpClient);
    connect(ui->ckAutoSend, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));

    ui->cboxInterval->setCurrentIndex(ui->cboxInterval->findText(QString::number(App::IntervalTcpClient)));
    connect(ui->cboxInterval, SIGNAL(currentIndexChanged(int)), this, SLOT(saveConfig()));

//    ui->txtServerIP->setText(App::TcpServerIP);
//    connect(ui->txtServerIP, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));

    ui->txtServerPort->setText(QString::number(App::TcpServerPort));
    connect(ui->txtServerPort, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));

    timer->setInterval(App::IntervalTcpClient);
    App::AutoSendTcpClient ? timer->start() : timer->stop();
}

void frmTcpClient::saveConfig()
{
    App::HexSendTcpClient = ui->ckHexSend->isChecked();
    App::HexReceiveTcpClient = ui->ckHexReceive->isChecked();
    App::AsciiTcpClient = ui->ckAscii->isChecked();
    App::DebugTcpClient = ui->ckDebug->isChecked();
    App::AutoSendTcpClient = ui->ckAutoSend->isChecked();
    App::IntervalTcpClient = ui->cboxInterval->currentText().toInt();
    App::TcpServerIP = TCP_Client_IP;
    App::TcpServerPort = TCP_Client_Port;
    App::writeConfig();

    timer->setInterval(App::IntervalTcpClient);
    App::AutoSendTcpClient ? timer->start() : timer->stop();
}

void frmTcpClient::append(int type, const QString &data, bool clear)
{
    static int currentCount = 0;
    static int maxCount = 100;

    if (clear) {
        ui->txtMain->clear();
        currentCount = 0;
        return;
    }

    if (currentCount >= maxCount) {
        ui->txtMain->clear();
        currentCount = 0;
    }

    if (ui->ckShow->isChecked()) {
        return;
    }

    //过滤回车换行符
    QString strData = data;
    strData = strData.replace("\r", "");
    strData = strData.replace("\n", "");

    //不同类型不同颜色显示
    QString strType;
    if (type == 0) {
        strType = "发送";
        ui->txtMain->setTextColor(QColor("darkgreen"));
    } else {
        strType = "接收";
        ui->txtMain->setTextColor(QColor("red"));
    }

    strData = QString("时间[%1] %2: %3").arg(TIMEMS).arg(strType).arg(strData);
    ui->txtMain->append(strData);
    currentCount++;
}

void frmTcpClient::connected()
{
    isOk = true;
    ui->btnConnect->setText("断开");
    append(0, "服务器连接");
}

void frmTcpClient::disconnected()
{
    isOk = false;
    tcpSocket->abort();
    ui->btnConnect->setText("连接");
    append(1, "服务器断开");
    connect_flag = 0;
}

void frmTcpClient::readData()
{
    QByteArray data = tcpSocket->readAll();
    QByteArray temp_data = data;
    QString strData;
    int len;

    if(temp_data.at(0) == 0x30 && temp_data.at(2) == 0x06)
    {
        temp_data.remove(0,5);
        qDebug()<<"temp_data:"<<temp_data;
        for (int i = 0;i < temp_data.length();i++)
        {
            if(temp_data[i] == 'u' && temp_data[i+1] == 'r' && temp_data[i+2] == 'l')
            {
                temp_data.remove(0,i+6);
            }
        }
        for (int t = 0;t < temp_data.length();t++)
        {
            if(temp_data[t] == '"' && temp_data[t+1] == ',')
            {
                len = t;
                url = temp_data.remove(t,temp_data.length());
                qDebug()<<"url:"<<url;
            }
        }
        ui->txtMain->setTextColor(QColor("black"));
        strData = QString("复制下载链接:");
        qDebug()<<"url:"<<url;
        ui->txtMain->append(strData);
        ui->txtMain->append(url);
    }
    else if(temp_data.at(0) == 0x20 && temp_data.at(1) == 0x02 && temp_data.at(2) == 0x00 && temp_data.at(3) == 0x00)
    {
//        Cloud_OTA_Post();//上传版本号后期置标制位
        OTA_flag = 1;
    }
    if (data.length() <= 0) {
        return;
    }

    QString buffer;
    if (App::HexReceiveTcpClient) {
        buffer = QUIHelper::byteArrayToHexStr(data);
    } else if (App::AsciiTcpClient) {
        buffer = QUIHelper::byteArrayToAsciiStr(data);
    } else {
        buffer = QString(data);
    }

    append(1, buffer);
    url_function(buffer);
    //自动回复数据,可以回复的数据是以;隔开,每行可以带多个;所以这里不需要继续判断
    if (App::DebugTcpClient) {
        int count = App::Keys.count();
        for (int i = 0; i < count; i++) {
            if (App::Keys.at(i) == buffer) {
                sendData(App::Values.at(i));
                break;
            }
        }
    }
}

void frmTcpClient::sendData(const QString &data)
{
    QByteArray buffer;
    if (App::HexSendTcpClient) {
        buffer = QUIHelper::hexStrToByteArray(data);
    } else if (App::AsciiTcpClient) {
        buffer = QUIHelper::asciiStrToByteArray(data);
    } else {
        buffer = data.toLatin1();
    }

    tcpSocket->write(buffer);
    append(0, data);
}

void frmTcpClient::on_btnConnect_clicked()
{
    int i = 0;
    if (ui->btnConnect->text() == "连接") {
        tcpSocket->abort();
        tcpSocket->connectToHost(App::TcpServerIP, App::TcpServerPort);
        while(1)
        {
            i++;
            if(i > 500)
            {
                ui->txtMain->setTextColor(QColor("black"));
                ui->txtMain->append("TCP connect success");
                break;
            }
        }
        Cloud_Connected();//连接云服务器
        InitTimer();
    } else {
        tcpSocket->abort();
        ui->txtMain->setTextColor(QColor("red"));
        ui->txtMain->append("TCP disconnect!");
    }
}

void frmTcpClient::on_btnSave_clicked()
{
    QString data = ui->txtMain->toPlainText();
    if (data.length() <= 0) {
        return;
    }

    QString fileName = QString("%1/%2.txt").arg(QUIHelper::appPath()).arg(STRDATETIME);
    QFile file(fileName);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        file.write(data.toUtf8());
        file.close();
    }

    on_btnClear_clicked();
}

void frmTcpClient::on_btnClear_clicked()
{
    append(0, "", true);
}

void frmTcpClient::on_btnSend_clicked()
{
    if (!isOk) {
        return;
    }

    QString data = ui->cboxData->currentText();
    if (data.length() <= 0) {
        return;
    }

    sendData(data);
}

void frmTcpClient::on_pushButton_clicked()      //连接阿里云服务器
{
    AliIoT_Parameter_Init();
    MQTT_Buff_Init();
    MQTT_ConectPack();
    QString data;
    QString data1;
    QByteArray buffer(temp_buff,Fixed_len+16+ClientID_len+Username_len+Passward_len);
    QByteArray buf = buffer.toHex(' ');
//    QByteArray buf1 = buf.toHex(' ');
//    qDebug() << buf1 << buffer.size();
    sendData(buf);
    qDebug() <<"buf :" <<buf;

//    Cloud_OTA_Post();
//    Sleep(200);
//    Cloud_Subcribe();
}
/*----------------------------------------------------------*/
/*函数名：初始化接收,发送,命令数据的 缓冲区 以及各状态参数  */
/*参  数：无                                                */
/*返回值：无                                                */
/*----------------------------------------------------------*/
void frmTcpClient::MQTT_Buff_Init(void)
{
    MQTT_RxDataInPtr=MQTT_RxDataBuf[0];               //指向发送缓冲区存放数据的指针归位
    MQTT_RxDataOutPtr=MQTT_RxDataInPtr;               //指向发送缓冲区读取数据的指针归位
    MQTT_RxDataEndPtr=MQTT_RxDataBuf[R_NUM-1];        //指向发送缓冲区结束的指针归位

    MQTT_TxDataInPtr=MQTT_TxDataBuf[0];               //指向发送缓冲区存放数据的指针归位
    MQTT_TxDataOutPtr=MQTT_TxDataInPtr;               //指向发送缓冲区读取数据的指针归位
    MQTT_TxDataEndPtr=MQTT_TxDataBuf[T_NUM-1];        //指向发送缓冲区结束的指针归位

    MQTT_CMDInPtr=MQTT_CMDBuf[0];                     //指向命令缓冲区存放数据的指针归位
    MQTT_CMDOutPtr=MQTT_CMDInPtr;                     //指向命令缓冲区读取数据的指针归位
    MQTT_CMDEndPtr=MQTT_CMDBuf[C_NUM-1];              //指向命令缓冲区结束的指针归位

    MQTT_ConectPack();                                //发送缓冲区添加连接报文
    MQTT_Subscribe(S_TOPIC_NAME_,0);	                  //发送缓冲区添加订阅topic，等级0

    Ping_flag = ConnectPack_flag = SubcribePack_flag = 0;  //各个参数清零
}
/*----------------------------------------------------------*/
/*函数名：阿里云初始化参数，得到客户端ID，用户名和密码      */
/*参  数：无                                                */
/*返回值：无                                                */
/*----------------------------------------------------------*/
void frmTcpClient::AliIoT_Parameter_Init(void)
{
    char temp[128];                                                       //计算加密的时候，临时使用的缓冲区

    memset(ClientID,0,128);                                               //客户端ID的缓冲区全部清零
    sprintf(ClientID,"%s|securemode=3,signmethod=hmacsha1|",FIX_DEVICENAME);  //构建客户端ID，并存入缓冲区
    ClientID_len = strlen(ClientID);                                      //计算客户端ID的长度

    memset(Username,0,128);                                               //用户名的缓冲区全部清零
    sprintf(Username,"%s&%s",FIX_DEVICENAME,FIX_PRODUCTKEY);                      //构建用户名，并存入缓冲区
    Username_len = strlen(Username);                                      //计算用户名的长度
    memset(Passward,0,128);
    memset(temp,0,128);                                                                      //临时缓冲区全部清零
    sprintf(temp,"clientId%sdeviceName%sproductKey%s",FIX_DEVICENAME,FIX_DEVICENAME,FIX_PRODUCTKEY);     //构建加密时的明文
    utils_hmac_sha1(temp,strlen(temp),Passward,FIX_DEVICESECERT,FIX_DEVICESECERT_LEN);                 //以DeviceSecret为秘钥对temp中的明文，进行hmacsha1加密，结果就是密码，并保存到缓冲区中
    Passward_len = strlen(Passward);                                                         //计算用户名的长度                                                                     //服务器端口号1883
}
/*----------------------------------------------------------*/
/*函数名：连接服务器报文                                    */
/*参  数：无                                                */
/*返回值：无                                                */
/*----------------------------------------------------------*/
void frmTcpClient::MQTT_ConectPack(void)
{
    int temp,Remaining_len;
    Fixed_len = 1;                                                        //连接报文中，固定报头长度暂时先=1
    Variable_len = 10;                                                    //连接报文中，可变报头长度=10
    Payload_len = 2 + ClientID_len + 2 + Username_len + 2 + Passward_len; //连接报文中，负载长度
    Remaining_len = Variable_len + Payload_len;                           //剩余长度=可变报头长度+负载长度

    temp_buff[0]=0x10;                       //固定报头第1个字节 ：固定0x01
    do{                                      //循环处理固定报头中的剩余长度字节，字节量根据剩余字节的真实长度变化
        temp = Remaining_len%128;            //剩余长度取余128
        Remaining_len = Remaining_len/128;   //剩余长度取整128
        if(Remaining_len>0)
            temp |= 0x80;                    //按协议要求位7置位
        temp_buff[Fixed_len] = temp;         //剩余长度字节记录一个数据
        Fixed_len++;	                     //固定报头总长度+1
    }while(Remaining_len>0);                 //如果Remaining_len>0的话，再次进入循环

    temp_buff[Fixed_len+0]=0x00;    //可变报头第1个字节 ：固定0x00
    temp_buff[Fixed_len+1]=0x04;    //可变报头第2个字节 ：固定0x04
    temp_buff[Fixed_len+2]=0x4D;	//可变报头第3个字节 ：固定0x4D
    temp_buff[Fixed_len+3]=0x51;	//可变报头第4个字节 ：固定0x51
    temp_buff[Fixed_len+4]=0x54;	//可变报头第5个字节 ：固定0x54
    temp_buff[Fixed_len+5]=0x54;	//可变报头第6个字节 ：固定0x54
    temp_buff[Fixed_len+6]=0x04;	//可变报头第7个字节 ：固定0x04
    temp_buff[Fixed_len+7]=0xC2;	//可变报头第8个字节 ：使能用户名和密码校验，不使用遗嘱，不保留会话
    temp_buff[Fixed_len+8]=0x00; 	//可变报头第9个字节 ：保活时间高字节 0x00
    temp_buff[Fixed_len+9]=0x64;	//可变报头第10个字节：保活时间高字节 0x64   100s
//    qDebug("temp_buff1:%x",temp_buff);
    /*     CLIENT_ID      */
    temp_buff[Fixed_len+10] = ClientID_len/256;                			  			    //客户端ID长度高字节
    temp_buff[Fixed_len+11] = ClientID_len%256;               			  			    //客户端ID长度低字节
    memcpy(&temp_buff[Fixed_len+12],ClientID,ClientID_len);                 			//复制过来客户端ID字串
    /*     用户名        */
    temp_buff[Fixed_len+12+ClientID_len] = Username_len/256; 				  		    //用户名长度高字节
    temp_buff[Fixed_len+13+ClientID_len] = Username_len%256; 				 		    //用户名长度低字节
    memcpy(&temp_buff[Fixed_len+14+ClientID_len],Username,Username_len);                //复制过来用户名字串
    /*      密码        */
    temp_buff[Fixed_len+14+ClientID_len+Username_len] = Passward_len/256;			    //密码长度高字节
    temp_buff[Fixed_len+15+ClientID_len+Username_len] = Passward_len%256;			    //密码长度低字节
    memcpy(&temp_buff[Fixed_len+16+ClientID_len+Username_len],Passward,Passward_len);   //复制过来密码字串
    temp_buff[Fixed_len+16+ClientID_len+Username_len+Passward_len+1] = '\0';
    qDebug("%s",temp_buff);
    qDebug("num:%d",Fixed_len+16+ClientID_len+Username_len+Passward_len);
    for(int i = 0;i < (Fixed_len+16+ClientID_len+Username_len+Passward_len);i++){
    qDebug("temp_buff:%x,%d",temp_buff[i],i);}
//    TxDataBuf_Deal(temp_buff, Fixed_len + Variable_len + Payload_len);                  //加入发送数据缓冲区
}
/*----------------------------------------------------------*/
/*函数名：SUBSCRIBE订阅topic报文                            */
/*参  数：QoS：订阅等级                                     */
/*参  数：topic_name：订阅topic报文名称                     */
/*返回值：无                                                */
/*----------------------------------------------------------*/
void frmTcpClient::MQTT_Subscribe(char *topic_name, int QoS)
{
    Fixed_len = 2;                              //SUBSCRIBE报文中，固定报头长度=2
    Variable_len = 2;                           //SUBSCRIBE报文中，可变报头长度=2
    Payload_len = 2 + strlen(topic_name) + 1;   //计算有效负荷长度 = 2字节(topic_name长度)+ topic_name字符串的长度 + 1字节服务等级

    temp_buff[0]=0x82;                                    //第1个字节 ：固定0x82
    temp_buff[1]=Variable_len + Payload_len;              //第2个字节 ：可变报头+有效负荷的长度
    temp_buff[2]=0x00;                                    //第3个字节 ：报文标识符高字节，固定使用0x00
    temp_buff[3]=0x01;		                              //第4个字节 ：报文标识符低字节，固定使用0x01
    temp_buff[4]=strlen(topic_name)/256;                  //第5个字节 ：topic_name长度高字节
    temp_buff[5]=strlen(topic_name)%256;		          //第6个字节 ：topic_name长度低字节
    memcpy(&temp_buff[6],topic_name,strlen(topic_name));  //第7个字节开始 ：复制过来topic_name字串
    temp_buff[6+strlen(topic_name)]=QoS;                  //最后1个字节：订阅等级

    //TxDataBuf_Deal(temp_buff, Fixed_len + Variable_len + Payload_len);  //加入发送数据缓冲区
}
/*----------------------------------------------------------*/
/*函数名：PING报文，心跳包                                  */
/*参  数：无                                                */
/*返回值：无                                                */
/*----------------------------------------------------------*/
void frmTcpClient::MQTT_PingREQ(void)
{
    temp_buff[0]=0xC0;              //第1个字节 ：固定0xC0
    temp_buff[1]=0x00;              //第2个字节 ：固定0x00

    //TxDataBuf_Deal(temp_buff, 2);   //加入数据到缓冲区
}
/*----------------------------------------------------------*/
/*函数名：等级0 发布消息报文                                */
/*参  数：topic_name：topic名称                             */
/*参  数：data：数据                                        */
/*参  数：data_len：数据长度                                */
/*返回值：无                                                */
/*----------------------------------------------------------*/
void frmTcpClient::MQTT_PublishQs0(char *topic, char *data, int data_len)
{
    int temp,Remaining_len;

    Fixed_len = 1;                              //固定报头长度暂时先等于：1字节
    Variable_len = 2 + strlen(topic);           //可变报头长度：2字节(topic长度)+ topic字符串的长度
    Payload_len = data_len;                     //有效负荷长度：就是data_len
    Remaining_len = Variable_len + Payload_len; //剩余长度=可变报头长度+负载长度

    temp_buff[0]=0x30;                       //固定报头第1个字节 ：固定0x30
    do{                                      //循环处理固定报头中的剩余长度字节，字节量根据剩余字节的真实长度变化
        temp = Remaining_len%128;            //剩余长度取余128
        Remaining_len = Remaining_len/128;   //剩余长度取整128
        if(Remaining_len>0)
            temp |= 0x80;                    //按协议要求位7置位
        temp_buff[Fixed_len] = temp;         //剩余长度字节记录一个数据
        Fixed_len++;	                     //固定报头总长度+1
    }while(Remaining_len>0);                 //如果Remaining_len>0的话，再次进入循环

    temp_buff[Fixed_len+0]=strlen(topic)/256;                      //可变报头第1个字节     ：topic长度高字节
    temp_buff[Fixed_len+1]=strlen(topic)%256;		               //可变报头第2个字节     ：topic长度低字节
    memcpy(&temp_buff[Fixed_len+2],topic,strlen(topic));           //可变报头第3个字节开始 ：拷贝topic字符串
    memcpy(&temp_buff[Fixed_len+2+strlen(topic)],data,data_len);   //有效负荷：拷贝data数据

    //TxDataBuf_Deal(temp_buff, Fixed_len + Variable_len + Payload_len);  //加入发送数据缓冲区
}
/*----------------------------------------------------------*/
/*函数名：处理服务器发来的等级0的推送                       */
/*参  数：redata：接收的数据                                */
/*返回值：无                                                */
/*----------------------------------------------------------*/
void frmTcpClient::MQTT_DealPushdata_Qs0(unsigned char *redata)
{
    int  re_len;               	           //定义一个变量，存放接收的数据总长度
    int  pack_num;                         //定义一个变量，当多个推送一起过来时，保存推送的个数
    int  temp,temp_len;                    //定义一个变量，暂存数据
    int  totle_len;                        //定义一个变量，存放已经统计的推送的总数据量
    int  topic_len;              	       //定义一个变量，存放推送中主题的长度
    int  cmd_len;                          //定义一个变量，存放推送中包含的命令数据的长度
    int  cmd_loca;                         //定义一个变量，存放推送中包含的命令的起始位置
    int  i;                                //定义一个变量，用于for循环
    int  local,multiplier;
    unsigned char tempbuff[RBUFF_UNIT];	   //临时缓冲区
    unsigned char *data;                   //redata过来的时候，第一个字节是数据总量，data用于指向redata的第2个字节，真正的数据开始的地方

    re_len = redata[0]*256+redata[1];                               //获取接收的数据总长度
    data = &redata[2];                                              //data指向redata的第2个字节，真正的数据开始的
    pack_num = temp_len = totle_len = temp = 0;                	    //各个变量清零
    local = 1;
    multiplier = 1;
    do{
        pack_num++;                                     			//开始循环统计推送的个数，每次循环推送的个数+1
        do{
            temp = data[totle_len + local];
            temp_len += (temp & 127) * multiplier;
            multiplier *= 128;
            local++;
        }while ((temp & 128) != 0);
        totle_len += (temp_len + local);                          	//累计统计的总的推送的数据长度
        re_len -= (temp_len + local) ;                              //接收的数据总长度 减去 本次统计的推送的总长度
        local = 1;
        multiplier = 1;
        temp_len = 0;
    }while(re_len!=0);                                  			//如果接收的数据总长度等于0了，说明统计完毕了

    temp_len = totle_len = 0;                		            	//各个变量清零
    local = 1;
    multiplier = 1;
    for(i=0;i<pack_num;i++){                                        //已经统计到了接收的推送个数，开始for循环，取出每个推送的数据
        do{
            temp = data[totle_len + local];
            temp_len += (temp & 127) * multiplier;
            multiplier *= 128;
            local++;
        }while ((temp & 128) != 0);
        topic_len = data[local+totle_len]*256+data[local+1+totle_len] + 2;    //计算本次推送数据中主题占用的数据量
        cmd_len = temp_len-topic_len;                               //计算本次推送数据中命令数据占用的数据量
        cmd_loca = totle_len + local +  topic_len;                  //计算本次推送数据中命令数据开始的位置
        memcpy(tempbuff,&data[cmd_loca],cmd_len);                   //命令数据拷贝出来
        CMDBuf_Deal(tempbuff, cmd_len);                             //加入命令到缓冲区
        totle_len += (temp_len+local);                              //累计已经统计的推送的数据长度
        local = 1;
        multiplier = 1;
        temp_len = 0;
    }
}
/*----------------------------------------------------------*/
/*函数名：处理发送缓冲区                                    */
/*参  数：data：数据                                        */
/*参  数：size：数据长度                                    */
/*返回值：无                                                */
/*----------------------------------------------------------*/
void frmTcpClient::TxDataBuf_Deal(unsigned char *data, int size)
{
    memcpy(&MQTT_TxDataInPtr[2],data,size);      //拷贝数据到发送缓冲区
    MQTT_TxDataInPtr[0] = size/256;              //记录数据长度
    MQTT_TxDataInPtr[1] = size%256;              //记录数据长度
    MQTT_TxDataInPtr+=TBUFF_UNIT;                 //指针下移
    if(MQTT_TxDataInPtr==MQTT_TxDataEndPtr)      //如果指针到缓冲区尾部了
        MQTT_TxDataInPtr = MQTT_TxDataBuf[0];    //指针归位到缓冲区开头
}
/*----------------------------------------------------------*/
/*函数名：处理命令缓冲区                                    */
/*参  数：data：数据                                        */
/*参  数：size：数据长度                                    */
/*返回值：无                                                */
/*----------------------------------------------------------*/
void frmTcpClient::CMDBuf_Deal(unsigned char *data, int size)
{
    memcpy(&MQTT_CMDInPtr[2],data,size);      //拷贝数据到命令缓冲区
    MQTT_CMDInPtr[0] = size/256;              //记录数据长度
    MQTT_CMDInPtr[1] = size%256;              //记录数据长度
    MQTT_CMDInPtr[size+2] = '\0';             //加入字符串结束符
    MQTT_CMDInPtr+=CBUFF_UNIT;                 //指针下移
    if(MQTT_CMDInPtr==MQTT_CMDEndPtr)         //如果指针到缓冲区尾部了
        MQTT_CMDInPtr = MQTT_CMDBuf[0];       //指针归位到缓冲区开头
}


void frmTcpClient::on_Subscribe_clicked()      //订阅报文
{
    MQTT_Subscribe(S_TOPIC_NAME_,0);
    QString data;
    QString data1;
    QByteArray buffer(temp_buff,Fixed_len + Variable_len + Payload_len);
    QByteArray buf = buffer.toHex(' ');
    sendData(buf);
    qDebug() <<"subscribe :" <<buf;
}

void frmTcpClient::on_Publish_clicked()         //发布报文
{
    char temp[TBUFF_UNIT];
    QString data;
    QString data1;
    sprintf(temp,ui->Things->text().toLocal8Bit().data());
    MQTT_PublishQs0(P_TOPIC_NAME_,temp,strlen(temp));

    QByteArray buffer(temp_buff,Fixed_len + Variable_len + Payload_len);
    QByteArray buf = buffer.toHex(' ');
    sendData(buf);
    qDebug() <<"publish :" <<buf;
}

void frmTcpClient::url_function(QString buffer)
{
    //qDebug()<<buffer;
    char *data;
    QByteArray ba = buffer.toLatin1();
    data = ba.data();
    data = data + 12;
    qDebug()<<"data:"<<(data+12);

}
void frmTcpClient::Cloud_Connected()              //连接云服务器
{
    AliIoT_Parameter_Init();
    MQTT_Buff_Init();
    MQTT_ConectPack();
    QString data;
    QString data1;
    QByteArray buffer(temp_buff,Fixed_len+16+ClientID_len+Username_len+Passward_len);
    QByteArray buf = buffer.toHex(' ');
    sendData(buf);
    qDebug() <<"buf :" <<buf;

    connect_flag = 1;
    ui->txtMain->setTextColor(QColor("black"));
    ui->txtMain->append("IOT connect success!");
}
void frmTcpClient::Cloud_OTA_Post()               //版本号推送???
{
    char temp[TBUFF_UNIT];
    char version[TBUFF_UNIT] = "{\"id\": 1568876629092,\"params\": {\"version\": \"v1.0\"}}";
    sprintf(temp,version);
    MQTT_PublishQs0(FIX_VERSION,temp,strlen(temp));
//    qDebug() <<"temp :" <<temp<<"FIX_VERSION:"<<FIX_VERSION;
    QByteArray buffer(temp_buff,Fixed_len + Variable_len + Payload_len);
    QByteArray buf = buffer.toHex(' ');
    sendData(buf);

//    qDebug() <<"publish :" <<buf;
    ui->txtMain->setTextColor(QColor("black"));
    ui->txtMain->append("vision post success");
}
void frmTcpClient::Cloud_Subcribe()               //订阅主题
{
    MQTT_Subscribe(FIX_SUBSCRIBE_TOPIC,0);
    QByteArray buffer(temp_buff,Fixed_len + Variable_len + Payload_len);
    QByteArray buf = buffer.toHex(' ');
    sendData(buf);
    qDebug() <<"subscribe1 :" <<buf;
    ui->txtMain->setTextColor(QColor("black"));
    ui->txtMain->append("SUBSCRIBE success");
}
void frmTcpClient::Cloud_Subcribe_OTA()           //订阅升级版本主题
{
    MQTT_Subscribe(FIX_SUBSCRIBE_OTA_TOPIC,0);
    QByteArray buffer1(temp_buff,Fixed_len + Variable_len + Payload_len);
    QByteArray buf1 = buffer1.toHex(' ');
    sendData(buf1);
    qDebug() <<"subscribe1 :" <<buf1;
    ui->txtMain->setTextColor(QColor("black"));
    ui->txtMain->append("SUBSCRIBE OTA success");
}

void frmTcpClient::Cloud_Things_Post()            //发布事件推送
{
    unsigned char data[30];
    char temp[TBUFF_UNIT];

    memset(temp,0,TBUFF_UNIT);
//sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"AZ\":%s,\"EL\":%s,\"PL\":%s},\"version\":\"1.0.0\"}",show_info.Pre_Azm,show_info.Pre_El,show_info.Pre_Pol);
//  sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"He\":%s,\"EL\":%s,\"PL\":%s},\"version\":\"1.0.0\"}",show_info.Car_A,show_info.Pre_El,show_info.Pre_Pol);

    sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Carrier_Head\":%s,\"Carrier_EL\":%s,\"Carrier_Roll\":%s,\"T_AZ\":%s,\"T_EL\":%s,\"T_PL\":%s,\"state\":%d,\"Carrier_Head\":%s,\"Carrier_EL\":%s,\"Carrier_Roll\":%s,\"Longitude\":%s,\"Latitude\":%s,\"GPS\":%d,\"Limit\":%d,\"AGC\":%s,\"Alarm\":%d},\"version\":\"1.0.0\"}"
            ,show_info.Pre_Azm,show_info.Pre_El,show_info.Pre_Pol,show_info.Cur_Azm
            ,show_info.Cur_El,show_info.Cur_Pol,show_info.Ant_Stat,show_info.Car_A
            ,show_info.Car_E,show_info.Car_P,show_info.longitude,show_info.attitude
            ,show_info.Gps_Stat,show_info.Limit_Info,show_info.AGC,show_info.Alarm_Info);
    qDebug()<<temp;
    MQTT_PublishQs0(FIX_POST_TOPIC,temp,strlen(temp));

    QByteArray buffer(temp_buff,Fixed_len + Variable_len + Payload_len);
    QByteArray buf = buffer.toHex(' ');
    sendData(buf);
    qDebug() <<"publish :" <<buf;
    ui->txtMain->setTextColor(QColor("black"));
    ui->txtMain->append("POST success");
}
void frmTcpClient::Cloud_OTA_Post_Progress()      //发布升级进度推送
{

}
void frmTcpClient::Cloud_Ping()
{
    MQTT_PingREQ();
    sendData(temp_buff);
}

void frmTcpClient ::Cloud_main()
{
    qDebug()<<"thread test...";

//    while(1)
//    {
//        qDebug()<<"Thread test...";
//    }
}
/*定时器*/
void frmTcpClient ::InitTimer()
{
         m_timer = new QTimer;   //设置定时器是否为单次触发。默认为 false 多次触发
         m_timer->setSingleShot(false);   //启动或重启定时器, 并设置定时器时间：毫秒
         m_timer->start(500);   //定时器触发信号槽
         connect(m_timer, SIGNAL(timeout()), this, SLOT(TimerTimeout()));
         qDebug()<<"timer start...";
}
/*超时处理*/
void frmTcpClient ::TimerTimeout()
{
    //判断定时器是否运行
    if(m_timer->isActive())
    {
//        m_timer->stop();   //停止定时器
//执行定时器触发时需要处理的业务
//        qDebug()<<"stop timer";
        if(connect_flag == 1)
        {
            if(OTA_flag == 1)
            {
                Cloud_OTA_Post();
                OTA_flag = 0;
                qDebug()<<"OTA TEST";
            }
            if(Cloud_Subcribe_flag == 0)
            {
                Cloud_Subcribe();
                Cloud_Subcribe_flag = 1;
            }
            if(Cloud_Subcribe_OTA_flag == 0)
            {
                Cloud_Subcribe_OTA();
                Cloud_Subcribe_OTA_flag = 1;
            }
            Cloud_Things_Post();
//           Cloud_Ping();
            qDebug()<<"DATA POST";
        }
    }
}

