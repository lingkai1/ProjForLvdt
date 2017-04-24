/**
  ******************************************************************************
  * @file    main.h
  * $Revision: 17 $
  * $Date:: 2012-07-06 11:16:48 +0800 #$
  * @brief   主函数包含的头文件.
  ******************************************************************************
  * @attention
  *
  *<h3><center>&copy; Copyright 2009-2012, EmbedNet</center>
  *<center><a href="http:\\www.embed-net.com">http://www.embed-net.com</a></center>
  *<center>All Rights Reserved</center></h3>
  * 
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
#define SOCK_UDPS        0
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_conf.h"
//#include "usart.h"
#include "usart2.h"
#include "stdio.h"
#include "delay.h"
#include "spi.h"
#include "socket.h"	// Just include one header for WIZCHIP
#include "AD7606.h"
#include "tim6.h"
#include "tim4.h"
#include "iir.h"
#include "fir.h"
//  function declare for main.c and network.c
void  Net_platform_init(void);								// initialize the dependent host peripheral
void network_init(void);								// Initialize Network information and display it
void orderProcess(char);



#endif /* __MAIN_H */

/*********************************END OF FILE**********************************/
