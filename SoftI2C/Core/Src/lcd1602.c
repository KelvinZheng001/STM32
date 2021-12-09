#include "lcd1602.h"



uint8_t iicAddress = 0b0100111;
uint8_t LCD_data = 0x08;



/*------------------------------------------------
 uS延时函数，含有输入参数 unsigned char t，无返回值
 unsigned char 是定义无符号字符变量，其值的范围是
 0~255 这里使用晶振12M，精确延时请使用汇编,大致延时
 长度如下 T=tx2+5 uS
------------------------------------------------*/
void delay_nus(unsigned int n) //N us延时函数
{
    unsigned int i;
        for (i=0;i<n;i++);
}


///*------------------------------------------------
// uS延时函数，含有输入参数 unsigned char t，无返回值
// unsigned char 是定义无符号字符变量，其值的范围是
// 0~255 这里使用晶振12M，精确延时请使用汇编,大致延时
// 长度如下 T=tx2+5 uS
//------------------------------------------------*/
//void delay_nus(unsigned int n) //N us延时函数
//{
//    unsigned int i=0;
//        for (i=0;i<n;i++)
//            _nop_();
//}

/*------------------------------------------------
 mS延时函数，含有输入参数 unsigned char t，无返回值
 unsigned char 是定义无符号字符变量，其值的范围是
 0~255 这里使用晶振12M，精确延时请使用汇编
------------------------------------------------*/
void delay_nms(unsigned int n) //N ms延时函数
{
//	HAL_Delay(n);
    unsigned int i,j;
        for (i=0;i<n;i++)
            for (j=0;j<1140;j++);
}


//********************液晶屏使能********************
void Enable_LCD_write()
{
	LCD_data |= (1 << (3 - 1)); //E=1;
	IICWriteByte(iicAddress, LCD_data); //Write_Random_Address_Byte(adress_1602,LCD_data);  //
	delay_nus(2);
	LCD_data &= ~(1 << (3 - 1)); //E=0;
	IICWriteByte(iicAddress, LCD_data); //Write_Random_Address_Byte(adress_1602,LCD_data);
}

/*------------------------------------------------
 写入命令函数
 ------------------------------------------------*/
void LCD_Write_Com(unsigned char command)
{
	delay_nus(16);
	LCD_data &= ~(1 << (1 - 1)); //RS=0;
	LCD_data &= ~(1 << (2 - 1)); //RW=0;
	//LCD_data&=~(1<<(4-1));
	IICWriteByte(iicAddress, LCD_data); //Write_Random_Address_Byte(adress_1602,LCD_data);

	LCD_data &= 0X0f; //清高四位
	LCD_data |= command & 0xf0; //写高四位
	IICWriteByte(iicAddress, LCD_data); //Write_Random_Address_Byte(adress_1602,LCD_data);
	Enable_LCD_write();

	command = command << 4; //低四位移到高四位
	LCD_data &= 0x0f; //清高四位
	LCD_data |= command & 0xf0; //写低四位
	IICWriteByte(iicAddress, LCD_data); //Write_Random_Address_Byte(adress_1602,LCD_data);
	Enable_LCD_write();
}

/*------------------------------------------------
 写入数据函数
 ------------------------------------------------*/
void LCD_Write_Data(unsigned char value)
{
	delay_nus(16);
	LCD_data |= (1 << (1 - 1)); //RS=1;
	LCD_data &= ~(1 << (2 - 1)); //RW=0;
	IICWriteByte(iicAddress, LCD_data); //Write_Random_Address_Byte(adress_1602, LCD_data);

	LCD_data &= 0X0f; //清高四位
	LCD_data |= value & 0xf0; //写高四位
	IICWriteByte(iicAddress, LCD_data); //Write_Random_Address_Byte(adress_1602, LCD_data);
	Enable_LCD_write();

	value = value << 4; //低四位移到高四位
	LCD_data &= 0x0f; //清高四位
	LCD_data |= value & 0xf0; //写低四位
	IICWriteByte(iicAddress, LCD_data); //Write_Random_Address_Byte(adress_1602, LCD_data);
	Enable_LCD_write();
}

/*------------------------------------------------
 清屏函数
 ------------------------------------------------*/
void LCD_Clear(void)
{
	LCD_Write_Com(0x01);
	delay_nms(2);
}
/*------------------------------------------------
 写入字符串函数
 ------------------------------------------------*/
void LCD_Write_String(unsigned char x, unsigned char y, char *s)
{
	if (y == 0)
	{
		LCD_Write_Com(0x80 + x);     //表示第一行
	}
	else
	{
		LCD_Write_Com(0xC0 + x);      //表示第二行
	}
	while (*s)
	{
		LCD_Write_Data(*s);
		s++;
	}
}

void LCD_Init(void)
{
	LCD_Write_Com(0x02);
	delay_nus(40);
	LCD_Write_Com(0x28); //4位显示！！！！！！！！！！！！！！！！！！
	LCD_Write_Com(0x0c); //显示开
	LCD_Write_Com(0x01); //清屏
	delay_nms(10);
}

