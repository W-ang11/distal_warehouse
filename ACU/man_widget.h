#ifndef MAN_WIDGET_H
#define MAN_WIDGET_H

#include <QWidget>
#include "pub_info.h"
#include "sql_oper.h"

namespace Ui {
    class Man_Widget;
}

class Man_Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Man_Widget(QWidget *parent = nullptr);
    void updata_widget_data(SHOW s);
    void updata_antenna_select(STAR s);
    void ant_state(int value);

signals:

public slots:
    //void select_antenna_widget();

private slots:

    void on_button_select_antenna_clicked();

private:
    Ui::Man_Widget *ui;
};

#endif // MAN_WIDGET_H
