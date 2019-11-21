#ifndef MAN_P_H
#define MAN_P_H

#include <QWidget>

namespace Ui {
class Man_P;
}

class Man_P : public QWidget
{
    Q_OBJECT

public:
    explicit Man_P(QWidget *parent = nullptr);
    ~Man_P();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::Man_P *ui;
};

#endif // MAN_P_H
