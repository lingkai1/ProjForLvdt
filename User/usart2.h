
#ifndef __SBUS_H__
#define __SBUS_H__

#include <stm32f4xx.h>
#define setIP  2
#define setOrder 1


void USART2_INIT(void);

void Tim4_Configuration(void);

void Data_Trans(void);	

int sumCheck(unsigned char*,char);
int sendData(uint8_t* ch, int num);
#endif 
