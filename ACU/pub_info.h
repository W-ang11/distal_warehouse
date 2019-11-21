#ifndef PUB_INFO_H
#define PUB_INFO_H
//extern struct SHOW a;
typedef  struct {
    char Pre_Azm[10],Pre_El[10], Pre_Pol[10];  //预置角度
    char Cur_Azm[10],Cur_El[10],Cur_Pol[10];   //当前角度
    int Ant_Stat;                              //天线状态
    char Car_A[10],Car_E[10],Car_P[10];        //载体航向,俯仰,横滚
    char longitude[10],attitude[10];           //经度,纬度
    int Gps_Stat;                              //gps状态
    int Limit_Info;             //限位信息
    char AGC[10];
    int Alarm_Info;                   //告警信息
}SHOW;


extern "C" void Hex2Ascii(char xx);
extern "C" char Compre_JY(char *ss);//对比校验字符,正确返回1
extern "C" void Add_JY(char *s); //为字符串增加校验从第二个字符开始

#endif // PUB_INFO_H
