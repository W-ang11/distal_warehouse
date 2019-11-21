#include "firware_upd.h"
#include "ui_firware_upd.h"

FirWare_Upd::FirWare_Upd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FirWare_Upd)
{
    ui->setupUi(this);
}

FirWare_Upd::~FirWare_Upd()
{
    delete ui;
}
