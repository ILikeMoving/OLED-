/************************************************************
* ���IIC��������
* GPIO:    ��soft_iic.h�궨�� SOFT_IIC_PORT
* SCL����: ��soft_iic.h�궨�� SOFT_IIC_SCL_PIN
* SDA����: ��soft_iic.h�궨�� SOFT_IIC_SDA_PIN
*************************************************************/
#ifndef __SOFT_IIC_H__
#define __SOFT_IIC_H__

#include "stm32f1xx_hal.h"	//����HAL��

#define SOFT_IIC_PORT                   GPIOA
#define SOFT_IIC_SCL_PIN                GPIO_PIN_6
#define SOFT_IIC_SDA_PIN                GPIO_PIN_5


void Soft_IIC_W_SCL(GPIO_PinState BitValue);//����SCL���ŵ�ƽ

void Soft_IIC_W_SDA(GPIO_PinState BitValue);//����SDA���ŵ�ƽ
	
GPIO_PinState Soft_IIC_R_SDA(void);//��ȡSDA���ŵ�ƽ

void Soft_IIC_Start(void);//���IIC��ʼ����

void Soft_IIC_Stop(void);//���IIC��ֹ����

void Soft_IIC_SendByte(uint8_t Byte);//���IIC����һ���ֽ�����

uint8_t Soft_IIC_ReceiveByte(void);//���IIC����һ���ֽ�����

void Soft_IIC_SendACK(uint8_t AckBit);//���IIC����Ӧ��

uint8_t Soft_IIC_ReceiveACK(void);//���IIC����Ӧ��

void Soft_IIC_Init(void);   //���IIC��ʼ��


#endif /*__SOFT_IIC_H__*/
