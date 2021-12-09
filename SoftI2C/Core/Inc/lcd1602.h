#include "IIC.h"



void delay_nus(unsigned int n);
void delay_nms(unsigned int n);
void delay_nus(unsigned int n); //N us延时函数
void Enable_LCD_write();
void LCD_Write_Com(unsigned char command);
void LCD_Init(void);
void LCD_Write_String(unsigned char x, unsigned char y, char *s);
void LCD_Clear(void);
