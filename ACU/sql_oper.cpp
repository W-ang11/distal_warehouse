#include "sql_oper.h"
#include "ui_sql_oper.h"
#include <QMessageBox>
#include <QDebug>
#include <QSqlRecord>
#include "man_widget.h"
#include <QSerialPort>

Sql_Oper::Sql_Oper(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Sql_Oper)
{
    ui->setupUi(this);
    //打印一下Qt支持的数据库驱动
    //qDebug()<<QSqlDatabase::drivers();
    //默认连接失败的情况下创建一个Antenna_Info_DataBase.db
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        //添加sqlite数据库
        database = QSqlDatabase::addDatabase("QSQLITE");
        //设置数据库
        database.setDatabaseName("Antenna_Info_DataBase.db");
    }
    if (!database.open())
    {
        //当打开失败的时候给一个打开失败提示框吧
        QMessageBox::warning(this,"错误",database.lastError().text());
        return;
    }
    qDebug()<<"打开成功";
    //在这插入一个表头吧
    QSqlQuery query;
    query.exec("create table antenna_info(name text,longitude float,xb_pl float,zb_pl float,zb_sv float,pol_mode varchar(1),xw_info float);");

//    //设置模型
    mode =new QSqlTableModel(this);
    mode->setTable("antenna_info");//设置使用哪个表
    //把model放在view
    ui->tableView->setModel(mode);

    //显示model里的数据
    mode->select();

    //修改表头信息,
    mode->setHeaderData(0,Qt::Horizontal,"卫星名称");mode->setHeaderData(1,Qt::Horizontal,"卫星经度");
    mode->setHeaderData(2,Qt::Horizontal,"信标频率"); mode->setHeaderData(3,Qt::Horizontal,"载波频率");
    mode->setHeaderData(4,Qt::Horizontal,"载波速率");mode->setHeaderData(5,Qt::Horizontal,"极化方式");
    mode->setHeaderData(6,Qt::Horizontal,"锁定门限");

    //设置mode的编辑模式为手动提交
    mode->setEditStrategy(QSqlTableModel::OnManualSubmit);

    query.exec("select *from antenna_info");
    while(query.next())
    {
        qDebug()<<query.value(0).toString()
                <<query.value(1).toFloat()
                <<query.value(2).toFloat()
                <<query.value(3).toFloat()
                <<query.value(4).toFloat()
                <<query.value(5).toString()
                <<query.value(6).toFloat();
    }
}

Sql_Oper::~Sql_Oper()
{
    delete ui;
    //在这需要关闭数据库吧
    database.close();
}

void Sql_Oper::on_buttonadd_clicked()
{
    //增加空行
    QSqlRecord record =mode->record();//获取空记录
    //获取行号
    int row =mode->rowCount();
    mode->insertRecord(row,record);
}

void Sql_Oper::on_buttonsure_clicked()
{
    //提交所有动作
    mode->submitAll();
}

void Sql_Oper::on_buttoncancel_clicked()
{
    //取消所有动作
    mode->revertAll();
    //在提交
    mode->submitAll();
}

void Sql_Oper::on_buttondelete_clicked()
{
    //获取选择模型
    QItemSelectionModel *sele = ui->tableView->selectionModel();
    //取出模型中的索引
    QModelIndexList list = sele->selectedRows();
    //删除所有选中行
    for(int i=0 ; i < list.size();i++)
    {
        mode->removeRow(list.at(i).row());
    }
}

#include "pub_info.h"
extern Man_Widget *m_w;
extern QSerialPort serial;
//给卫星信息结构体设置一个默认值
struct STAR star;
void Sql_Oper::on_buttonselect_clicked()  //这个函数需要加很多限制,但是这里我先这么简单处理,后面再加限制
{
    //获取选择模型
    QItemSelectionModel *sele = ui->tableView->selectionModel();
    //取出模型中的索引
    QModelIndexList list = sele->selectedRows();

    //这个是需要使用的卫星也就是说,每次只能选一条记录,如果选择多条的话我直接就退出不做处理
    if(list.size() !=  1)
    {
        return;
    }
    //只有一条记录的话,那就是第0条记录里的行号,因为在这里我只选一行
    qDebug()<<list.at(0).row();
    QMessageBox::StandardButton rb =QMessageBox::information(this,"提示","Do want Current Select?",QMessageBox::Yes|QMessageBox::No);
    if(rb == QMessageBox::Yes)
    {
        //那我就需要得到这条索引里的各字段的值
        memset((char *)&star,0,sizeof(star));
        QAbstractItemModel *modessl = ui->tableView->model();
        QModelIndex indextemp = modessl->index(list.at(0).row(),0);//遍历第list.at(0).row()行的所有列
        QVariant datatemp = modessl->data(indextemp);
        strncpy(star.name,datatemp.toString().toLatin1().data(),strlen(datatemp.toString().toLatin1().data()));
        indextemp = modessl->index(list.at(0).row(),1);
        datatemp = modessl->data(indextemp);
        strncpy(star.longitude,datatemp.toString().toLatin1().data(),strlen(datatemp.toString().toLatin1().data()));
        indextemp = modessl->index(list.at(0).row(),2);
        datatemp = modessl->data(indextemp);
        strncpy(star.xb_pl,datatemp.toString().toLatin1().data(),strlen(datatemp.toString().toLatin1().data()));
        indextemp = modessl->index(list.at(0).row(),3);
        datatemp = modessl->data(indextemp);
        strncpy(star.zb_pl,datatemp.toString().toLatin1().data(),strlen(datatemp.toString().toLatin1().data()));
        indextemp = modessl->index(list.at(0).row(),4);
        datatemp = modessl->data(indextemp);
        strncpy(star.zb_sl,datatemp.toString().toLatin1().data(),strlen(datatemp.toString().toLatin1().data()));
        indextemp = modessl->index(list.at(0).row(),5);
        datatemp = modessl->data(indextemp);
        strncpy(star.pol_model,datatemp.toString().toLatin1().data(),strlen(datatemp.toString().toLatin1().data()));
        indextemp = modessl->index(list.at(0).row(),6);
        datatemp = modessl->data(indextemp);
        strncpy(star.limit_info,datatemp.toString().toLatin1().data(),strlen(datatemp.toString().toLatin1().data()));
        //更新卫星选择信息
        m_w->updata_antenna_select(star);
        //组包$cmd,sat,Sino5,12260.00,0,0,110.50,1,5.00,*hh发送
        QString str;
        if(strncmp(star.pol_model,"V",1) == 0)      //垂直
        {
            str = QString("$cmd,sat,%1,%2,%3,%4,%5,1,%6,*hh\r\n").arg(star.name).arg(star.xb_pl).
                    arg(star.zb_pl).arg(star.zb_sl).arg(star.longitude).arg(star.limit_info);
        }
        else if(strncmp(star.pol_model,"H",1) == 0)  //水平
        {
            str = QString("$cmd,sat,%1,%2,%3,%4,%5,0,%6,*hh\r\n").arg(star.name).arg(star.xb_pl).
                    arg(star.zb_pl).arg(star.zb_sl).arg(star.longitude).arg(star.limit_info);
        }
        else
        {
           //当你不是"V"或者"H"那我就不处理
            return;
        }
        //增加校验
        char *pstr = str.toLatin1().data();
        Add_JY(pstr);
        serial.write(pstr);
        qDebug()<<pstr;

    }
    else
    {
        //当我们按下No按钮那就退出吧
        return;
    }
}


void Sql_Oper::on_buttonquit_clicked()
{
    this->hide();
    //关闭数据库
    database.close();
}
