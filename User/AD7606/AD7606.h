/**
  ******************************************************************************
  * @file    spi.c 
  * @author  link
  * @version 
  * @date    2016-1-18
  * @brief   AD7606底层驱动 使用spi口 配置为软件SPI，
  ******************************************************************************
  */ 



#ifndef __AD7606_H
#define __AD7606_H
#include <stm32f4xx.h>

//CS--PD2
#define AD7606_CS_H()      {GPIOD->BSRRL = GPIO_Pin_2;}   
#define AD7606_CS_L()      {GPIOD->BSRRH = GPIO_Pin_2;}    
//CLK--PC10
#define AD7606_CLK_H()     {GPIOC->BSRRL = GPIO_Pin_10;}
#define AD7606_CLK_L()     {GPIOC->BSRRH = GPIO_Pin_10;}
//OS0--PC9
#define AD7606_OS0_H()     {GPIOC->BSRRL = GPIO_Pin_9;} 
#define AD7606_OS0_L()     {GPIOC->BSRRH = GPIO_Pin_9;}
//OS1--PC8
#define AD7606_OS1_H()     {GPIOC->BSRRL = GPIO_Pin_8;} 
#define AD7606_OS1_L()     {GPIOC->BSRRH = GPIO_Pin_8;}
//0S2--PC7
#define AD7606_OS2_H()     {GPIOC->BSRRL = GPIO_Pin_7;} 
#define AD7606_OS2_L()     {GPIOC->BSRRH = GPIO_Pin_7;}
//RST PB14
#define AD7606_Reset_ON()     {GPIOB->BSRRL = GPIO_Pin_14;}   
#define AD7606_Reset_OFF()    {GPIOB->BSRRH = GPIO_Pin_14;}	   
// CONA PC6               
#define AD7606_CONVTA_ON()     {GPIOC->BSRRH = GPIO_Pin_6;}   
#define AD7606_CONVTA_OFF()    {GPIOC->BSRRL = GPIO_Pin_6;}  
#define AD7606_CONVTB_ON()     {GPIOB->BSRRH = GPIO_Pin_15;}   
#define AD7606_CONVTB_OFF()    {GPIOB->BSRRL = GPIO_Pin_15;}  
#define AD7606_BUSY()            GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13) 
#define AD7606_ReadBIT()         GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11)
/*******************************************************************************/
void AD7606_GPIO_Init(void);

/**
  * @brief  AD7606一次完整的读取函数
	* @param  
  * @return 
  */
void AD7606_READ(void);

/**
  * @brief  AD7606一次完整的读取函数
	* @param  
  * @return 
  */
void AD7606_Init(void);

int ADC_Calibration(int);  // 30ms ~ 40 ms


#endif
