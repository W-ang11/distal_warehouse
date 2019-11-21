#ifndef PARA_ADJ_H
#define PARA_ADJ_H

#include <QWidget>
#include "para_inface.h"

#define PASSWORD 111111
namespace Ui {
class Para_Adj;
}

class Para_Adj : public QWidget
{
    Q_OBJECT

public:
    explicit Para_Adj(QWidget *parent = nullptr);
    ~Para_Adj();
    void password_deal();

private:
    Ui::Para_Adj *ui;
    Para_Inface para_inface;
};

#endif // PARA_ADJ_H
