#ifndef PARA_INFACE_H
#define PARA_INFACE_H

#include <QWidget>

struct BZ_MAGN{
    char bz[10];     //本振
    char magnification[6]; //放大倍数
};
struct PLACE_INFO{
    char longitude[10];
    char attitude[10];
    char course[10];
};
struct GENERAL{
    char azm_search_range[10];
    char azm_star_compensation[10];
    char el_star_compensation[10];
    char pol_star_compensation[10];
};
struct ADVANCE{
    char azm_zero_compensation[10];char el_zero_compensation[10]; char pol_zero_compensation[10];
    char azm_reset_position[10];char el_reset_position[10];char pol_reset_position[10];
    char el_down_angle[10]; char el_search_range[10];char el_search_step[10];char azm_move_rightlimit[10];
    char azm_move_leftlimit[10];char el_move_uplimit[10];char el_move_downlimit[10];char pol_move_islimit[10];
    char pol_move_thelimit[10];char lp_el_compensation[10];char lp_roll_compensation[10];
    char lp_course_compensation[10];char downlimit_offset_rangle[8]; char azm_search_zero[6];
};

namespace Ui {
class Para_Inface;
}

class Para_Inface : public QWidget
{
    Q_OBJECT

public:
    explicit Para_Inface(QWidget *parent = nullptr);
    ~Para_Inface();
    void updata_rec_type(int i);
    void updata_limit_state(bool stat);
    void updata_bz_magn(BZ_MAGN info);
    void updata_place_info(PLACE_INFO p);
    void updata_general_par(GENERAL gel);
    void updata_advance_par(ADVANCE ad);

private slots:
    void on_checkBox_xb_pressed();

    void on_checkBox_dvb_pressed();

    void on_button_open_limit_clicked();

    void on_button_close_limit_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::Para_Inface *ui;    
};

#endif // PARA_INFACE_H
