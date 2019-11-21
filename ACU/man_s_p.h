#ifndef MAN_S_P_H
#define MAN_S_P_H

#include <QWidget>

namespace Ui {
class Man_S_P;
}

class Man_S_P : public QWidget
{
    Q_OBJECT

public:
    explicit Man_S_P(QWidget *parent = nullptr);
    ~Man_S_P();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Man_S_P *ui;
};

#endif // MAN_S_P_H
