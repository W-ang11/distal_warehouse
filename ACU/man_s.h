#ifndef MAN_S_H
#define MAN_S_H

#include <QWidget>

namespace Ui {
class Man_S;
}

class Man_S : public QWidget
{
    Q_OBJECT

public:
    explicit Man_S(QWidget *parent = nullptr);
    ~Man_S();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::Man_S *ui;
};

#endif // MAN_S_H
