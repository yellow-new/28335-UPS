/*
 * oled.c
 *
 *  Created on: 2021年5月5日
 *      Author: 黄新
 *
 *  ********************************************************************************************
 *  @version           V1.0
 *  @date               2021-xx-xx
 *  @brief               oled显示函数接口 SCL ->33 SDA->32
 *  *******************************************************************************************
 */

#include "oledfont.h"
#include "oled.h"



unsigned char OLED_GRAM[128][8];//OLED的显存数据组


//开启OLED显示
void OLED_Display_On(void)
{
    OLED_WR_Byte(0X8D,OLED_CMD);  /* 设置充电泵（和下个命令结合使用） */
    OLED_WR_Byte(0X14,OLED_CMD);  /* 0x14 使能充电泵， 0x10 是关闭 */
    OLED_WR_Byte(0XAF,OLED_CMD);   /* 打开OLED面板 */
}
//关闭OLED显示
void OLED_Display_Off(void)
{
    OLED_WR_Byte(0X8D,OLED_CMD);   /* 设置充电泵（和下个命令结合使用）*/
    OLED_WR_Byte(0X10,OLED_CMD);  /* 0x14 使能充电泵，0x10 是关闭 */
    OLED_WR_Byte(0XAE,OLED_CMD);   /* 关闭OLED面板 */
}

//m^n函数
Uint32 oled_pow(unsigned char m,unsigned char n)
{
    Uint32 result=1;
    while(n--)result*=m;
    return result;
}
/*
 *显示数字
 *x,y :起点坐标
 *num:要显示的数字
 *len :数字的位数
 *sizey:字体的大小
 *mode:0反白显示，1正常显示
 */
void OLED_ShowNum(unsigned char x,unsigned char y,Uint32 num,unsigned char len,unsigned char size1,unsigned char mode)
{
    unsigned char t,temp,m=0;
    if(size1==8)m=2;
    for(t=0;t<len;t++)
    {
        temp=(num/oled_pow(10,len-t-1))%10;
            if(temp==0)
            {
                OLED_ShowChar(x+(size1/2+m)*t,y,'0',size1,mode);
      }
            else
            {
              OLED_ShowChar(x+(size1/2+m)*t,y,temp+'0',size1,mode);
            }
  }
    OLED_Refresh();
}

/*
 * 在指定的位置显示一个字符，包括部分字符
 * x:0~127
 * y:0~63
 * chr：要显示的字符
 * sizey:选择字体 6×8 6×12 8×16  12×24
 * mode:0反白显示，1正常显示
 */
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr,unsigned char size1,unsigned char mode)
{
    unsigned char i,m,temp,size2,chr1;
    unsigned char x0=x,y0=y;
    if(size1==8)size2=6;
    else size2=(size1/8+((size1%8)?1:0))*(size1/2);  //得到一个字符对应点阵所占用的字节数
    chr1=chr-' ';  //计算偏移后的值
    for(i=0;i<size2;i++)
    {
        if(size1==8)
              {temp=asc2_0806[chr1][i];} //调用0806字体
//        else if(size1==12)
//        {temp=asc2_1206[chr1][i];} //调用1206字体
        else if(size1==16)
        {temp=asc2_1608[chr1][i];} //调用1608字体
//        else if(size1==24)
//        {temp=asc2_2412[chr1][i];} //调用2412字体
        else return;
        for(m=0;m<8;m++)
        {
            if(temp&0x01)OLED_DrawPoint(x,y,mode);
            else OLED_DrawPoint(x,y,!mode);
            temp>>=1;
            y++;
        }
        x++;
        if((size1!=8)&&((x-x0)==size1/2))
        {x=x0;y0=y0+8;}
        y=y0;
  }
}


/*
 * 显示字符串
 * x,y:起点坐标
 * size1:字体大小
 * *chr:字符串起始地址
 * mode:0反白显示，1正常显示
 */
void OLED_ShowString(unsigned char x,unsigned char  y,unsigned char  *chr,unsigned char  size1,unsigned char mode)
{
    while((*chr>=' ')&&(*chr<='~'))//判定是不是非法字符
        {
            OLED_ShowChar(x,y,*chr,size1,mode);
            if(size1==8)x+=6;
            else x+=size1/2;
            chr++;
      }
    OLED_Refresh();
}

/*
 *反显函数
 *i:0 正常显示，1反色显示
 */
void OLED_ColorTurn(unsigned char i)
{
    if(i==0)
        {
            OLED_WR_Byte(0xA6,OLED_CMD);
        }
    if(i==1)
        {
            OLED_WR_Byte(0xA7,OLED_CMD);
        }
}
/*
*********************************************************************************************************
*        函 数 名: OLED_SetContrast
*        功能说明: 设置对比度
*        形    参:  无
*        返 回 值: 无
*********************************************************************************************************
*/
void OLED_SetContrast(unsigned char ucValue)
{
    OLED_WR_Byte(0x81,OLED_CMD);        /* 设置对比度命令(双字节命令），第1个字节是命令，第2个字节是对比度参数0-255 */
    OLED_WR_Byte(ucValue,OLED_CMD);     /* 设置对比度参数,缺省CF */
}

/*
 * 屏幕旋转180度
 * i:0正常显示，1反转显示
 */
void OLED_DisplayTurn(unsigned char i)
{
    if(i==0)
        {
            OLED_WR_Byte(0xC8,OLED_CMD); /* C0 ：正常扫描,从COM0到COM63;  C8 : 反向扫描, 从 COM63至 COM0 */
            OLED_WR_Byte(0xA1,OLED_CMD); /* A0 ：列地址0映射到SEG0; A1 ：列地址127映射到SEG0 */
        }
    if(i==1)
        {
            OLED_WR_Byte(0xC0,OLED_CMD); /* C0 ：正常扫描,从COM0到COM63;  C8 : 反向扫描, 从 COM63至 COM0 */
            OLED_WR_Byte(0xA0,OLED_CMD);/* A0 ：列地址0映射到SEG0; A1 ：列地址127映射到SEG0 */
        }
}

void OLED_Init(void)
{

    OLED_WR_Byte(0xAE,OLED_CMD);/*关闭OLED面板显示（休眠）*/
    OLED_WR_Byte(0x00,OLED_CMD);/*设置列地址低4bit*/
    OLED_WR_Byte(0x10,OLED_CMD);/*设置列地址高4bit*/
    OLED_WR_Byte(0x40,OLED_CMD);/*设置起始行地址（低5bit 0-63）， 硬件相关*/
    OLED_WR_Byte(0x81,OLED_CMD);/* 设置对比度命令(双字节命令），第1个字节是命令，第2个字节是对比度参数0-255 */
    OLED_WR_Byte(0xCF,OLED_CMD);/* 设置对比度参数,缺省CF */
    OLED_WR_Byte(0xA1,OLED_CMD);/* A0 ：列地址0映射到SEG0; A1 ：列地址127映射到SEG0 */
    OLED_WR_Byte(0xC8,OLED_CMD);/* C0 ：正常扫描,从COM0到COM63;  C8 : 反向扫描, 从 COM63至 COM0 */
    OLED_WR_Byte(0xA6,OLED_CMD); /* A6 : 设置正常显示模式; A7 : 设置为反显模式 */
    OLED_WR_Byte(0xA8,OLED_CMD); /* 设置COM路数 */
    OLED_WR_Byte(0x3f,OLED_CMD); /* 1 ->（63+1）路 */
    OLED_WR_Byte(0xD3,OLED_CMD); /* 设置显示偏移（双字节命令）*/
    OLED_WR_Byte(0x00,OLED_CMD); /* 无偏移 */
    OLED_WR_Byte(0xd5,OLED_CMD);/* 设置显示时钟分频系数/振荡频率 */
    OLED_WR_Byte(0x80,OLED_CMD);/* 设置分频系数,高4bit是分频系数，低4bit是振荡频率 */
    OLED_WR_Byte(0xD9,OLED_CMD); /* 设置预充电周期 */
    OLED_WR_Byte(0xF1,OLED_CMD); /* [3:0],PHASE 1; [7:4],PHASE 2; */
    OLED_WR_Byte(0xDA,OLED_CMD); /* 设置COM脚硬件接线方式 */
    OLED_WR_Byte(0x12,OLED_CMD);
    OLED_WR_Byte(0xDB,OLED_CMD); /* 设置 vcomh 电压倍率 */
    OLED_WR_Byte(0x40,OLED_CMD); /* [6:4] 000 = 0.65 x VCC; 0.77 x VCC (RESET); 0.83 x VCC  */
    OLED_WR_Byte(0x20,OLED_CMD); /*有三种寻址模式，这里0x20和下面的0x02为确定页寻址模式*/
    OLED_WR_Byte(0x02,OLED_CMD);
    OLED_WR_Byte(0x8D,OLED_CMD);/* 设置充电泵（和下个命令结合使用） */
    OLED_WR_Byte(0x14,OLED_CMD);/* 0x14 使能充电泵， 0x10 是关闭 */
    OLED_WR_Byte(0xA4,OLED_CMD);
    OLED_WR_Byte(0xA6,OLED_CMD);
    OLED_WR_Byte(0x2E,OLED_CMD);/*禁用滚动*/
    OLED_Clear();//清屏
    OLED_WR_Byte(0xAF,OLED_CMD); /* 打开OLED面板 */
}
/*
 * 发送一个字节
 * mode：数据/命令 0为数据，1表示命令
 */
void OLED_WR_Byte(unsigned char dat,unsigned char mode)
{
    IIC_Start();
    IIC_Send_Byte(0x78);//发送从机地址
    IIC_Wait_Ack();
    if(mode){IIC_Send_Byte(0x40);}//0x40是写数据 Co=0 D/C=1
  else{IIC_Send_Byte(0x00);}//0x00写命令Co=0 D/C=0
    IIC_Wait_Ack();
    IIC_Send_Byte(dat);
    IIC_Wait_Ack();
    IIC_Stop();
}

/*
 * 清空数组显存
 */
void OLED_Clear(void)
{
    unsigned char i,n;
        for(i=0;i<8;i++)
        {
           for(n=0;n<128;n++)
                {
                 OLED_GRAM[n][i]=0;//清楚所有数据
                }
      }
        OLED_Refresh();//更新显示
}
//画点
//x:0~127
//y:0~63
//t:1 正常显示 0,无论是什么数据都给清零
void OLED_DrawPoint(unsigned char x,unsigned char y,unsigned char t)
{
    unsigned char i,m,n;
    i=y/8;//确定在哪一页
    m=y%8;//确定在这一页的哪一位
    n=0x01<<m;//确定在哪一页显示后将1进行偏移m位即可
    if(x>127||y>63)return;//超出范围了
    if(t){OLED_GRAM[x][i]|=n;}
    else
    {
        OLED_GRAM[x][i]=~OLED_GRAM[x][i];//将i页的第x列的八位数据去反也就是0  1互换
        OLED_GRAM[x][i]|=n;//取反后将此位置的数据置1
        OLED_GRAM[x][i]=~OLED_GRAM[x][i];//在进行去反
        /*上面的三行代码作用就是无论是什么数据都给清零*/
    }
}


//更新显示到OLED
void OLED_Refresh(void)
{
    unsigned char i,n;
    for(i=0;i<8;i++)
    {
        OLED_WR_Byte(0xb0+i,OLED_CMD);//设置页地址（0~7）
        OLED_WR_Byte(0x00,OLED_CMD);/*设置列地址低4bit*/
        OLED_WR_Byte(0x10,OLED_CMD);/*设置列地址高4bit*/
        IIC_Start();
        IIC_Send_Byte(0x78);//从机地址
        IIC_Wait_Ack();
        IIC_Send_Byte(0x40);//写数据
        IIC_Wait_Ack();
        for(n=0;n<128;n++)
        {
            IIC_Send_Byte(OLED_GRAM[n][i]);
            IIC_Wait_Ack();
        }
        IIC_Stop();
  }
}


