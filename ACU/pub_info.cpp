#include "pub_info.h"

char ch ,cl;
void Hex2Ascii(char xx)
{
    ch = xx >> 4;
    cl = xx & 0x0f;
    if(ch >9)
    {
        ch = ch + 0x57;
    }
    else
    {
        ch = ch + 0x30;
    }
    if(cl > 9)
    {
        cl = cl + 0x57;
    }
    else
    {
        cl = cl + 0x30;
    }

}
char Compre_JY(char *ss)//对比校验字符,正确返回1
{
    char xors;
    char i = 2;
    if(ss[0]!= '$')return 0;
    xors = ss[1];
    while(ss[i]!='*'&&(i < 200))
    {
        xors = ss[i++]^xors;
    }
    Hex2Ascii(xors);
    if((ss[i+1] == ch)&&(ss[i+2] == cl))
    {
        return 1;
    }
    else
    {
        return 0;
    }

}
void Add_JY(char *s) //为字符串增加校验从第二个字符开始
{

    char xors;
    char i=2;
    if(s[0]!='$')
    {
        return ;
    }
    xors = s[0];
    while((s[i]!='*')&&(i < 200))
    {
        xors = s[i++]^xors;
    }
    Hex2Ascii(xors);
    s[i+1] = ch;
    s[i+2] = cl;
}
