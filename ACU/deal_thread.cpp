#include "deal_thread.h"
#include <QDebug>

extern char buf1[200];
SHOW show_info;
BZ_MAGN bz_magn;
PLACE_INFO place;
GENERAL general;
ADVANCE advance;
//War_Limit *w_l=nullptr;   //保存页面对象指针
Deal_Thread::Deal_Thread(QObject *parent) : QThread(parent)
{

    //加这一句主要是自定义数据类型在通过信号与槽传递时,需要使用qRegisterMetaType（）注册一下否则会报错
    qRegisterMetaType<SHOW>("SHOW");
}


//c代码
char s[30][30];
extern "C" void data_dis_unsign(int i, int k,unsigned int len)
{
    char temp[10];
    unsigned int j;
    int x;
    memset(temp,0,10);
    for(x = i; x < k; x ++)
    {
        if(strlen(s[x]) < len)
        {
            for (j=0;j < len - strlen(s[x]);j++)
            {
                temp[j]='0';
            }
            strncat(temp,s[x],strlen(s[x]));
            memset(s[x],0,30);
            strncpy(s[x],temp,strlen(temp));
            memset(temp,0,10);
        }
        else if (strlen(s[x]) >len)
        {
            strncat(temp ,s[x],len);
            memset(s[x],0,10);
            strncpy(s[x],temp,len);
        }
    }
}

extern "C" void data_dis_sign(int i,int k,unsigned int len)
{

    char temp[10];
    double xx;
    unsigned int j;
    int x;
    memset(temp,0,10);
    for(x = i; x < k; x ++)
    {
        if(strlen(s[x]) <= len)
        {
            if(atof(s[x])>=0)
            {
                temp[0] = '+';
                for (j=1;j<len-strlen(s[x]);j++)
                {
                    temp[j]='0';
                }
                strncat(temp,s[x],strlen(s[x]));
                memset(s[x],0,30);
                strncpy(s[x],temp,strlen(temp));
                memset(temp,0,10);
            }
            else
            {
                xx= atof(s[x]) * -1;
                memset(s[x],0,30);
                sprintf(s[x],"%.2f",double(xx));
                temp[0]='-';
                for (j=1;j<len - strlen(s[x]);j++)
                {
                    temp[j]='0';
                }
                strncat(temp,s[x],len);
                memset(s[x],0,30);
                strncpy(s[x],temp,strlen(temp));
                memset(temp,0,10);

            }

        }
        else
        {
            strncat(temp ,s[x],len);
            memset(s[x],0,10);
            strncpy(s[x],temp,len);
        }
    }

}

extern "C" void data_dis_sign_r(int i,int k,unsigned int len)
{
    char temp[10];
    double xx;
    unsigned int j;
    int x;
    memset(temp,0,10);
    for(x = i; x < k; x ++)
    {
        if(strlen(s[x]) <= len)
        {
            if(atof(s[x])>=0)
            {
               // temp[0] = '+';
                for (j=0;j<len-strlen(s[x]);j++)
                {
                    temp[j]='0';
                }
                strncat(temp,s[x],len);
                memset(s[x],0,30);
                strncpy(s[x],temp,strlen(temp));
                memset(temp,0,10);
            }
            else
            {
                xx= atof(s[x]) * -1;
                memset(s[x],0,30);
                if(xx >=100.00)
                {
                    sprintf(s[x],"%.1f",xx);
                }
                else
                {
                    sprintf(s[x],"%.2f",xx);
                }
                temp[0]='-';
                for (j=1; j<len - strlen(s[x]);j++)
                {
                    temp[j]='0';
                }
                strncat(temp,s[x],len);
                memset(s[x],0,30);
                strncpy(s[x],temp,strlen(temp));
                memset(temp,0,10);

            }

        }
        else
        {
            strncat(temp ,s[x],len);
            memset(s[x],0,10);
            strncpy(s[x],temp,len);
        }
    }
}

extern "C" void init_show()
{
   memset(show_info.Pre_Azm,0,10); memset(show_info.Pre_El,0,10);memset(show_info.Pre_Pol,0,10);
   memset(show_info.Cur_Azm,0,10);memset(show_info.Cur_El,0,10);memset(show_info.Cur_Pol,0,10);
   show_info.Ant_Stat = 0;
   memset(show_info.Car_A,0,10);memset(show_info.Car_E,0,10);memset(show_info.Car_P,0,10);
   memset(show_info.longitude,0,10);memset(show_info.attitude,0,10);
   show_info.Gps_Stat = 0;
   show_info.Limit_Info = 0;
   memset(show_info.AGC,0,10);
   show_info.Alarm_Info = 0;
}

//end
#include "pub_info.h"

void Deal_Thread::run()
{

    //不需要事件循环吧！
    //有没有这样一种情况,我这个线程还没有跑完结果在另一个线程中吧buff1中的数据给修改了
    //也许不会出现,毕竟串口发送指令需要时间,同时串口接受也需要时间，而我的代码运行要快得多
    //qDebug()<<buf1;

    int i,j=0;
    int d_len=0;
    unsigned int s_len=0;
    char *p;

    //判断校验,正确校验返回1，错误返回0
    if(Compre_JY(buf1) == 0)
    {
        //校验错误,直接返回
        return;

    }
    for (j=0;j<30;j++) {
        memset(s[j],0,30);
    }
    j=0;
    //这里是按逗号分割show信息
    p = strtok(buf1,",");
    while (p)
    {

        strcpy(s[j],p);
        j++;
        p = strtok(nullptr,",");
    }
    j=0;

    if(strcmp(s[0],"$SHOW") == 0)//这里我只解析show信息,如果还需要其它信息可以在后判断
    {
        i = 1;
        d_len = 2;
        s_len = 6;
        data_dis_unsign(i,d_len,s_len);  //预置方位
        i = 2;
        d_len = 3;
        s_len = 6;
        data_dis_sign(i,d_len,s_len);   //预置俯仰
        i = 3;
        d_len = 4;
        s_len = 6;
        data_dis_sign_r(i,d_len,s_len);  //预置极化
        i = 4;
        d_len = 5;
        s_len = 6;

        data_dis_unsign(i,d_len,s_len);  //当前方位
        i = 5;
        d_len = 6;
        s_len = 6;
        data_dis_sign(i,d_len,s_len);   //当前俯仰
        i = 6;
        d_len = 7;
        s_len = 6;
        data_dis_sign_r(i,d_len,s_len);  //当前极化

        i = 8;
        d_len = 9;
        s_len = 6;
        data_dis_unsign(i,d_len,s_len);  //载体航向
        i = 9;
        d_len = 11;
        s_len = 6;
        data_dis_sign(i,d_len,s_len);   //载体俯仰,横滚
        i = 11;
        d_len = 13;
        s_len = 7;
        data_dis_sign(i,d_len,s_len);  //经纬度

        init_show();                  //这里每次将show变量清空一下
        strncpy(show_info.Pre_Azm,s[1],6);
        strncpy(show_info.Pre_El,s[2],6);
        strncpy(show_info.Pre_Pol,s[3],6);
        strncpy(show_info.Cur_Azm,s[4],6);
        strncpy(show_info.Cur_El,s[5],6);
        strncpy(show_info.Cur_Pol,s[6],6);
        show_info.Ant_Stat =atoi(s[7]);
        strncpy(show_info.Car_A,s[8],6);      //载体方位
        strncpy(show_info.Car_E,s[9],6);      //载体俯仰
        strncpy(show_info.Car_P,s[10],6);     //载体横滚
        strncpy(show_info.longitude,s[11],7); //经度
        strncpy(show_info.attitude,s[12],7);  //纬度
        show_info.Gps_Stat = atoi(s[13]);     //gps状态
        show_info.Limit_Info = atoi(s[14]);   //限位信息
        show_info.Alarm_Info = atoi(s[15]);   //告警xinxi
        strncpy(show_info.AGC,s[16],4);       //AGC电平
        emit once_show_ok(show_info);
        //w_l->updata_widget_data(show_info);
    }
    else if(strncmp(s[1],"sn",2) == 0) //这里是处理天线的软硬件版本号
    {
        QString str = QString(s[2]);
        emit once_get_sn_ok(str);

    }
    else if(strncmp(s[1],"rec type",8) == 0) //rec type
    {
        if(strcmp(s[2],"beacon") == 0)
        {
            emit once_get_rec_type(0);
        }
        else if(strcmp(s[2],"dvb") == 0)
        {
            emit once_get_rec_type(1);
        }
    }
    else if(strncmp(s[1],"xwen",4)) //xwen
    {
        if(strncmp(s[1],"xwen on",7) == 0)
        {
            emit once_get_limit_state(true);
        }
        else if(strncmp(s[1],"xwen off",8) == 0)
        {
            emit once_get_limit_state(false);
        }
    }
    else if((strncmp(s[1],"beacon",6) == 0) ||(strncmp(s[1],"dvb",3) == 0))//beacon||dvb
    {
        memset(&bz_magn,0,sizeof(BZ_MAGN));
        strcpy(bz_magn.bz,s[2]);
        strcpy(bz_magn.magnification,s[3]);
        emit once_get_bz_magn(bz_magn);
    }
    else if(strncmp(s[1],"place",5) == 0)//get place
    {
        memset(&place,0,sizeof(PLACE_INFO));
        strcpy(place.course,s[2]);
        strcpy(place.attitude,s[3]);
        strcpy(place.course,s[4]);
        emit once_get_place(place);

    }
    else if(strncmp(s[1],"general",7) == 0)//general
    {
        memset(&general,0,sizeof(GENERAL));
        strcpy(general.azm_search_range,s[2]);
        strcpy(general.azm_star_compensation,s[3]);
        strcpy(general.el_star_compensation,s[4]);
        strcpy(general.pol_star_compensation,s[5]);
        emit once_get_general_par(general);

    }
    else if(strncmp(s[1],"advance",7) == 0)//advance
    {
        //清零之后拷贝数据
        memset(&advance,0,sizeof(ADVANCE));
        strcpy(advance.azm_zero_compensation,s[2]);
        strcpy(advance.el_zero_compensation,s[3]);
        strcpy(advance.pol_zero_compensation,s[4]);
        strcpy(advance.azm_reset_position,s[5]);
        strcpy(advance.el_reset_position,s[6]);
        strcpy(advance.pol_reset_position,s[7]);
        strcpy(advance.el_down_angle,s[8]);
        strcpy(advance.el_search_range,s[9]);
        strcpy(advance.el_search_step,s[10]);
        strcpy(advance.azm_move_rightlimit,s[11]);
        strcpy(advance.azm_move_leftlimit,s[12]);
        strcpy(advance.el_move_uplimit,s[13]);
        strcpy(advance.el_move_downlimit,s[14]);
        strcpy(advance.pol_move_islimit,s[15]);
        strcpy(advance.pol_move_thelimit,s[16]);
        strcpy(advance.lp_el_compensation,s[17]);
        strcpy(advance.lp_roll_compensation,s[18]);
        strcpy(advance.lp_course_compensation,s[19]);
        strcpy(advance.downlimit_offset_rangle,s[20]);
        strcpy(advance.azm_search_zero,s[21]);
        emit once_get_advance_par(advance);

    }
    memset(buf1,0,200);
    //当线程处理完成之后发送一个信号,然后在槽函数中关闭该线程
    emit deal_finish();
}
