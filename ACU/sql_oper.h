#ifndef SQL_OPER_H
#define SQL_OPER_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>


//卫星信息结构体
struct STAR{

    char name[12];         //卫星名称
    char longitude[10];    //卫星经度
    char xb_pl[10];        //载波频率
    char zb_pl[10];
    char zb_sl[10];
    char pol_model[2];
    char limit_info[4];

};

namespace Ui {
class Sql_Oper;
}

class Sql_Oper : public QWidget
{
    Q_OBJECT

public:
    explicit Sql_Oper(QWidget *parent = 0);
    ~Sql_Oper();

private slots:
    void on_buttonadd_clicked();
    void on_buttonsure_clicked();
    void on_buttoncancel_clicked();
    void on_buttondelete_clicked();
    void on_buttonselect_clicked();

    void on_buttonquit_clicked();

private:
    Ui::Sql_Oper *ui;
    QSqlDatabase database;
    QSqlTableModel *mode;
};

#endif // SQL_OPER_H
