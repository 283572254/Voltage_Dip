#ifndef _OLED_H_
#define _OLED_H_
#define OLED_ADDRESS 0x78
#include "main.h"

void WriteCmd(unsigned char I2C_Command);
void WriteData(unsigned char I2C_Data);
void OLED_Init(void);
void OLED_SetPos(unsigned char x,unsigned char y);
void OLED_Fill(unsigned char Fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(unsigned char x,unsigned char y,unsigned char ch[],unsigned char TextSize);
void OLED_ShowCN(unsigned char x,unsigned char y,unsigned char N);
void OLED_ShowBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
void OLED_ShowNumber(unsigned char x, unsigned char y, int num, unsigned char TextSize);

#endif

