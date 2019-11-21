#ifndef DEAL_THREAD_H
#define DEAL_THREAD_H

#include <QObject>
#include <QThread>
#include "pub_info.h"
#include <QMetaType>
#include "war_limit.h"
#include "man_widget.h"
#include "auto_widget.h"
#include "para_inface.h"

//主要是为了消除警告
extern char s[30][30];
extern SHOW show_info;
//extern War_Limit *w_l;
class Deal_Thread : public QThread
{
    Q_OBJECT
public:
    explicit Deal_Thread(QObject *parent = nullptr);

protected:
    void run();

signals:
    void deal_finish();
    void once_show_ok(SHOW s);
    void once_get_sn_ok(QString t);
    void once_get_rec_type(int i);
    void once_get_limit_state(bool stat);
    void once_get_bz_magn(BZ_MAGN bz_magn);
    void once_get_place(PLACE_INFO p);
    void once_get_general_par(GENERAL gen);
    void once_get_advance_par(ADVANCE adv);

public slots:

};

#endif // DEAL_THREAD_H
