#include "OLED.h"
#include "i2c.h"
#include"codetab.h"
#include "bmp.h"
#include "math.h"


void WriteCmd(unsigned char I2C_Command)
{
		HAL_I2C_Mem_Write(&hi2c1,OLED_ADDRESS,0x00,1,&I2C_Command,1,1000);
}
//写数据
void WriteData(unsigned char I2C_Data)
{
		HAL_I2C_Mem_Write(&hi2c1,OLED_ADDRESS,0x40,1,&I2C_Data,1,1000);
}

//厂家初始化代码
void OLED_Init(void)
{
		HAL_Delay(100);
		WriteCmd(0xAE); //display off
		WriteCmd(0x20);	//Set Memory Addressing Mode	
		WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
		WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
		WriteCmd(0xc8);	//Set COM Output Scan Direction
		WriteCmd(0x00); //---set low column address
		WriteCmd(0x10); //---set high column address
		WriteCmd(0x40); //--set start line address
		WriteCmd(0x81); //--set contrast control register
		WriteCmd(0xff); //áá?èμ÷?ú 0x00~0xff
		WriteCmd(0xa1); //--set segment re-map 0 to 127
		WriteCmd(0xa6); //--set normal display
		WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
		WriteCmd(0x3F); //
		WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
		WriteCmd(0xd3); //-set display offset
		WriteCmd(0x00); //-not offset
		WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
		WriteCmd(0xf0); //--set divide ratio
		WriteCmd(0xd9); //--set pre-charge period
		WriteCmd(0x22); //
		WriteCmd(0xda); //--set com pins hardware configuration
		WriteCmd(0x12);
		WriteCmd(0xdb); //--set vcomh
		WriteCmd(0x20); //0x20,0.77xVcc
		WriteCmd(0x8d); //--set DC-DC enable
		WriteCmd(0x14); //
		WriteCmd(0xaf); //--turn on oled panel
}

//设置光标起始坐标（x,y）
void OLED_SetPos(unsigned char x,unsigned char y)
{
		WriteCmd(0xb0+y);
		WriteCmd( (x & 0xf0) >> 4 | 0x10 );
		WriteCmd( (x & 0x0f) | 0x01 );
}

//填充整个屏幕
void OLED_Fill(unsigned char Fill_Data)
{
		unsigned char m,n;
		
		for(m=0;m<8;m++)
		{
				WriteCmd(0xb0+m);
				WriteCmd(0x00);
				WriteCmd(0x10);
				
				for(n=0;n<128;n++)
				{
						WriteData(Fill_Data);
				}
		}
}

//清屏
void OLED_CLS(void)
{
		OLED_Fill(0x00);
}

//将OLED从休眠中唤醒
void OLED_ON(void)
{
		WriteCmd(0xAF);
		WriteCmd(0x8D);
		WriteCmd(0x14);
}

//让OLED休眠 -- 休眠模式下,OLED功耗不到10uA
void OLED_OFF(void)
{
		WriteCmd(0xAE);
		WriteCmd(0x8D);
		WriteCmd(0x10);
}
void OLED_ShowNumber(unsigned char x, unsigned char y, int num, unsigned char TextSize)
{
    // 将数字转换为字符串
    char str[12]; // 最大为10位数字，再加上负号和结束符号
    sprintf(str, "%d", num);

    // 调用显示字符串的函数显示数字
    OLED_ShowStr(x, y, str, TextSize);
}


void OLED_ShowStr(unsigned char x,unsigned char y,unsigned char ch[],unsigned char TextSize)
{
		unsigned char c = 0,i = 0,j = 0;
		
		switch(TextSize)
		{
			case 1:
			{
					while(ch[j] != '\0')
					{
							c = ch[j] - 32;
							if(x>126)
							{
									x = 0;
									y++;
							}
							
							OLED_SetPos(x,y);
							
							for(i=0;i<6;i++)
							{
									WriteData(F6x8[c][i]);
							}
							x+=6;
							j++;
					}
			}
			break;
			
			case 2:
			{
					while(ch[j] != '\0')
					{
							c = ch[j] - 32;
							
							if(x>120)
							{
									x = 0;
									y++;
							}
							
							OLED_SetPos(x,y);
							
							for(i=0;i<8;i++)
							{
									WriteData(F8X16[c*16+i]);	
							}
							
							OLED_SetPos(x,y+1);
							
							for(i=0;i<8;i++)
							{
									WriteData(F8X16[c*16+i+8]);
							}
							x+=8;
							j++;		
					}
			}
			break;
		}
}

void OLED_ShowCN(unsigned char x,unsigned char y,unsigned char N)
{
		unsigned char i = 0;
		unsigned char addr = 32*N;
		
		OLED_SetPos(x,y);
		
		for(i=0;i<16;i++)
		{
				WriteData(F16X16[addr]);
				addr += 1;
		}
		
		OLED_SetPos(x,y+1);
		
		for(i=0;i<16;i++)
		{
				WriteData(F16X16[addr]);
				addr += 1;
		}
}
#if 1
void OLED_ShowBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
		unsigned char x,y;
		unsigned int j = 0;
		
		if(y1 % 8 == 0)
		{
				y = y1 / 8;
		}
		else
		{
				y = y1 / 8+1;
		}
		
		for(y=y0;y<y1;y++)
		{
				OLED_SetPos(x0,y);

				for(x=x0;x<x1;x++)
				{
						WriteData(BMP[j++]);
				}
		}
}
#endif

