/*
 * IIC.c
 *
 *
 *https://www.cnblogs.com/Lxk0825/p/15358846.html
 *  Created on: Sep 26, 2021
 *      Author: LK
 */
#include "IIC.h"

/*
 * myiic.c
 *
 *  Created on: Aug 30, 2021
 *      Author: along
 */
#include <stdio.h>

/*******************************************************************************
 * Function Name  : delay_us
 * Description    : Simulation IIC Timing series delay
 * Input          : None
 * Output         : None
 * Return         : None
 ****************************************************************************** */
void delay_us(uint32_t us) {
	uint32_t i = 0;
	for (i = 0; i < us; i++) {
		uint8_t a = 10;
		while (a--)
			;
	}
}

void my_SDA_IN(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Pin = SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
void my_SDA_OUT(void) {
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void IIC_Init(void) {
	SDA_H;
	SCL_H;
}

/*******************************************************************************
 * Function Name  : IIC_Stop
 * Description    : Master Stop Simulation IIC Communication
 * Input          : None
 * Output         : None
 * Return         : None
 ****************************************************************************** */
void IIC_Stop(void) {
	SDA_OUT(); //sda线输出
	SCL_L;
	SDA_L; //STOP:when CLK is high DATA change form low to high
	delay_us(4);
	SCL_H;
	SDA_H; //发送I2C总线结束信号
	delay_us(4);
}

/*******************************************************************************
 * Function Name  : IIC_Wait_Ack
 * Description    : Master Reserive Slave Acknowledge Single
 * Input          : None
 * Output         : None
 * Return         : Wheather     Reserive Slave Acknowledge Single  1，接收应答失败;0，接收应答成功
 ****************************************************************************** */
uint8_t IIC_Wait_Ack(void) {
	uint8_t ucErrTime = 0;
	SDA_IN();      //SDA设置为输入
	SDA_H;
	delay_us(1);
	SCL_H;
	delay_us(1);
	while (SDA_read) {
		ucErrTime++;
		if (ucErrTime > 250) {
			IIC_Stop();
			return 1;
		}
	}
	SCL_L;      //时钟输出0
	return 0;
}

/*******************************************************************************
 * Function Name  : IIC_Ack
 * Description    : Master Send Acknowledge Single
 * Input          : None
 * Output         : None
 * Return         : None
 ****************************************************************************** */
void IIC_Ack(void) {
	SCL_L;
	SDA_OUT();
	SDA_L;
	delay_us(2);
	SCL_H;
	delay_us(2);
	SCL_L;
}

/*******************************************************************************
 * Function Name  : IIC_NAck
 * Description    : Master Send No Acknowledge Single
 * Input          : None
 * Output         : None
 * Return         : None
 ****************************************************************************** */
void IIC_NAck(void) {
	SCL_L;
	SDA_OUT();
	SDA_H;
	delay_us(2);
	SCL_H;
	delay_us(2);
	SCL_L;
}

/*******************************************************************************
 * Function Name  : IIC_Send_Byte
 * Description    : Master Send a Byte to Slave
 * Input          : Will Send Date
 * Output         : None
 * Return         : None
 ****************************************************************************** */
void IIC_Send_Byte(uint8_t txd) {
	uint8_t t;
	SDA_OUT();
	SCL_L;      //拉低时钟开始数据传输
	for (t = 0; t < 8; t++) {
		//IIC_SDA=(txd&0x80)>>7;
		if ((txd & 0x80) >> 7)
			SDA_H;
		else
			SDA_L;
		txd <<= 1;
		delay_us(2);   //对TEA5767这三个延时都是必须的
		SCL_H;
		delay_us(2);
		SCL_L;
		delay_us(2);
	}
}
/*******************************************************************************
 * Function Name  : I2C_RadeByte
 * Description    : Master Reserive a Byte From Slave
 * Input          : ack=1时，发送ACK，ack=0，发送nACK
 * Output         : None
 * Return         : Date From Slave
 ****************************************************************************** */
uint8_t IIC_Read_Byte(unsigned char ack) {
	unsigned char i, receive = 0;
	SDA_IN();   //SDA设置为输入
	for (i = 0; i < 8; i++) {
		SCL_L;
		delay_us(2);
		SCL_H;
		receive <<= 1;
		if (SDA_read)
			receive++;
		delay_us(1);
	}
	if (!ack)
		IIC_NAck();   //发送nACK
	else
		IIC_Ack(); //发送ACK
	return receive;
}

/**************************实现函数********************************************
 *函数原型:        uint8_t IICreadBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data)
 *功　　能:        读取指定设备 指定寄存器的 length个值
 输入    dev  目标设备地址
 reg      寄存器地址
 length 要读的字节数
 *data  读出的数据将要存放的指针
 返回   读出来的字节数量
 *******************************************************************************/
uint8_t IICreadBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data) {
	uint8_t count = 0;

	IIC_Start();
	IIC_Send_Byte(dev << 1);       //发送写命令
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);   //发送地址
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte((dev << 1) + 1);  //进入接收模式
	IIC_Wait_Ack();

	for (count = 0; count < length; count++) {

		if (count != length - 1)
			data[count] = IIC_Read_Byte(1);  //带ACK的读数据
		else
			data[count] = IIC_Read_Byte(0);     //最后一个字节NACK
	}
	IIC_Stop();     //产生一个停止条件
	return count;
}

/*******************************************************************************
 * Function Name  : IIC_Start
 * Description    : Master Start Simulation IIC Communication
 * Input          : None
 * Output         : None
 * Return         : Wheather     Start
 ****************************************************************************** */
void IIC_Start(void) {
	SDA_OUT();     //sda线输出
	SDA_H;
	SCL_H;
	delay_us(5);
	SDA_L;     //START:when CLK is high,DATA change form high to low
	delay_us(5);
	SCL_L;     //钳住I2C总线，准备发送或接收数据
}

/**************************实现函数********************************************
 *函数原型:        uint8_t IICwriteBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data)
 *功　　能:        将多个字节写入指定设备 指定寄存器
 输入    dev  目标设备地址
 reg      寄存器地址
 length 要写的字节数
 *data  将要写的数据的首地址
 返回   返回是否成功
 *******************************************************************************/
uint8_t IICwriteBytes(uint8_t iicDeviceAddress, uint8_t reg, uint8_t length,
		uint8_t *data) {

	uint8_t count = 0;
	IIC_Start();

	uint8_t writeAddress = iicDeviceAddress << 1;
	//发送写命令
	IIC_Send_Byte(writeAddress);
	IIC_Wait_Ack();

//    //发送地址
//    IIC_Send_Byte(reg);
//    IIC_Wait_Ack();

	for (count = 0; count < length; count++) {
		uint8_t byte = data[count];
		IIC_Send_Byte(byte);
		IIC_Wait_Ack();
	}
	IIC_Stop();     //产生一个停止条件

	return 1; //status == 0;

}

uint8_t IICWriteByte(uint8_t iicDeviceAddress, uint8_t byte) {
	IIC_Start();

	uint8_t writeAddress = iicDeviceAddress << 1;
	//发送写命令地址
	IIC_Send_Byte(writeAddress);
	IIC_Wait_Ack();

	IIC_Send_Byte(byte);
	IIC_Wait_Ack();

	IIC_Stop(); //产生一个停止条件
	return 1; //status == 0;
}



