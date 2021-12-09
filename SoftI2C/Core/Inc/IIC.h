/*
 * myiic.h
 *
 *  Created on: Aug 30, 2021
 *      Author: along
 */

#ifndef INC_MYIIC_H_
#define INC_MYIIC_H_
#include "main.h"
#include "stm32f1xx_hal.h"


#define FALSE 0
#define TRUE 1

#define SCL_H HAL_GPIO_WritePin(GPIOB, SCL_Pin, GPIO_PIN_SET)
#define SCL_L HAL_GPIO_WritePin(GPIOB, SCL_Pin, GPIO_PIN_RESET)

#define SDA_H HAL_GPIO_WritePin(GPIOB, SDA_Pin, GPIO_PIN_SET)
#define SDA_L HAL_GPIO_WritePin(GPIOB, SDA_Pin, GPIO_PIN_RESET)

//IO方向设置
#define SDA_IN()  my_SDA_IN()
#define SDA_OUT() my_SDA_OUT()


#define SDA_read HAL_GPIO_ReadPin(GPIOB, SDA_Pin)

void delay_us(uint32_t us);

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口
void IIC_Start(void);                //发送IIC开始信号
void IIC_Stop(void);                  //发送IIC停止信号
void IIC_Send_Byte(uint8_t txd);            //IIC发送一个字节
uint8_t IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
uint8_t IIC_Wait_Ack(void);                 //IIC等待ACK信号
void IIC_Ack(void);                    //IIC发送ACK信号
void IIC_NAck(void);                //IIC不发送ACK信号

void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);

unsigned char I2C_Readkey(unsigned char I2C_Addr);
unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr);
unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data);
unsigned char IICwriteCmd(unsigned char dev, unsigned char cmd);
uint8_t IICwriteBytes(uint8_t iicDeviceAddress, uint8_t reg, uint8_t length, uint8_t* data);
uint8_t IICWriteByte(uint8_t iicDeviceAddress, uint8_t byte);
uint8_t IICwriteBits(uint8_t dev,uint8_t reg,uint8_t bitStart,uint8_t length,uint8_t data);
uint8_t IICwriteBit(uint8_t dev,uint8_t reg,uint8_t bitNum,uint8_t data);
uint8_t IICreadBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data);
#endif /* INC_MYIIC_H_ */
