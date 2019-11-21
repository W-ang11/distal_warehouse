#ifndef WAR_LIMIT_H
#define WAR_LIMIT_H

#include <QWidget>
#include <QStandardItemModel>
#include "pub_info.h"

namespace Ui {
class War_Limit;
}

class War_Limit : public QWidget
{
    Q_OBJECT

public:
    explicit War_Limit(QWidget *parent = nullptr);
    ~War_Limit();
    void updata_widget_data(SHOW s);
    void ant_state(int value);
    void show_limit_info(int value);
    void show_waring_info(int value);
    void updata_get_sn_info(QString t);

private:
    Ui::War_Limit *ui;
    QStandardItemModel *item;
    void init_tableview();
};

#endif // WAR_LIMIT_H
