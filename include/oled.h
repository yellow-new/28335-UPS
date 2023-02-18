/*
 * oled.h
 *
 *  Created on: 2021年5月5日
 *      Author: huangxin
 */

#ifndef OLED_OLED_H_
#define OLED_OLED_H_

#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "math.h"

#include "iic.h"

#define OLED_CMD    0   //写数据
#define OLED_DATA   1   //写命令

void OLED_Init(void);
void OLED_WR_Byte(unsigned char dat,unsigned char mode);
void OLED_ColorTurn(unsigned char i);
void OLED_DisplayTurn(unsigned char i);
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr,unsigned char size1,unsigned char mode);
void OLED_DisplayTurn(unsigned char i);
//void OLED_ShowChinese(unsigned char x,unsigned char y,unsigned char num,unsigned char size1,unsigned char mode);
void OLED_ShowNum(unsigned char x,unsigned char y,Uint32 num,unsigned char len,unsigned char size1,unsigned char mode);
Uint32 oled_pow(unsigned char m,unsigned char n);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_ShowString(unsigned char x,unsigned char  y,unsigned char  *chr,unsigned char  size1,unsigned char mode);
void OLED_DrawPoint(unsigned char x,unsigned char y,unsigned char t);
void OLED_Refresh(void);
//void OLED_ShowPicture(unsigned char x,unsigned char y,unsigned char sizex,unsigned char sizey,unsigned char BMP[],unsigned char mode);
void OLED_Clear(void);
void OLED_SetContrast(unsigned char ucValue);
//void OLED_ScrollDisplay(unsigned char num,unsigned char space,unsigned char mode);
void OLED_DrawLine(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char mode);
//void Boot_Animation(void);//开机动画
//void OLED_Draw_Waveform(unsigned char x1,unsigned char x2,unsigned char y,unsigned char z,unsigned char data,unsigned char mode);//画波形
//void Gui_Fill_Rect(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char show);
//void Gui_Draw_Rect(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1);
//void OLED_Dram_XY(void);
//void Draw_Circle(unsigned char x0,unsigned char y0,unsigned char r,unsigned char mode); //圆心(x0,y0),半径r
//void Draw_fill_circle(unsigned char x0,unsigned char y0,unsigned char r,unsigned char mode);//写画实心圆心(x0,y0),半径r
void Run_xy(unsigned char x0,unsigned char x1,unsigned char y0,unsigned char num,unsigned char run);


#endif /* OLED_OLED_H_ */
