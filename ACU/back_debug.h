#ifndef BACK_DEBUG_H
#define BACK_DEBUG_H

#include <QWidget>
#include <QKeyEvent>
#include <QTime>
#include "deal_thread.h"

extern int flag,len;
extern char buf[200],buf1[200];
extern War_Limit *w_l;
extern Man_Widget *m_w;
extern Auto_Widget *a_w;
extern Para_Inface *p_i;
namespace Ui {
class Back_Debug;
}

class Back_Debug  :public QWidget
{
    Q_OBJECT

signals:
    void getData(const QByteArray &data);

public:
    explicit Back_Debug(QWidget *parent = nullptr);
    ~Back_Debug();
    void close_thread();

protected:
    void keyPressEvent(QKeyEvent *evt);

//该槽函数是go to slot 自动生成
private slots:
    void on_senddata_button_clicked();

    void on_save_button_clicked();

private:
    Ui::Back_Debug *ui;
    Deal_Thread *thread;
    void readData();
    void writedata(const QByteArray &data);
    void clearrecive_window();
    void updata_widget(SHOW s);
    void updata_get_sn_info(QString t);
    void updata_rec_type(int i);
    void updata_limit_state(bool stat);
    void updata_bz_magn(BZ_MAGN bz_info);
    void updata_place_info(PLACE_INFO pl);
    void updata_general_par(GENERAL g);
    void updata_advance_par(ADVANCE adv);

};

#endif // BACK_DEBUG_H
