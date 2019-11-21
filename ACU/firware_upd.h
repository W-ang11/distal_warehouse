#ifndef FIRWARE_UPD_H
#define FIRWARE_UPD_H

#include <QWidget>

namespace Ui {
class FirWare_Upd;
}

class FirWare_Upd : public QWidget
{
    Q_OBJECT

public:
    explicit FirWare_Upd(QWidget *parent = nullptr);
    ~FirWare_Upd();

private:
    Ui::FirWare_Upd *ui;
};

#endif // FIRWARE_UPD_H
