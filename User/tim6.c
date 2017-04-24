#include "main.h"
extern char usartOutTimeFlag;
extern char state;
void TIM6_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  tim;
	NVIC_InitTypeDef         nvic;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	nvic.NVIC_IRQChannel = TIM6_DAC_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 2;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	
	tim.TIM_Prescaler = 84-1;
	tim.TIM_CounterMode = TIM_CounterMode_Up;
	tim.TIM_ClockDivision = TIM_CKD_DIV1;
	tim.TIM_Period = 2000;                    //2ms
	TIM_TimeBaseInit(TIM6,&tim);    
	TIM_Cmd(TIM6, ENABLE);	 
	TIM_ITConfig(TIM6, TIM_IT_Update,ENABLE);
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);	
	
}

void TIM6_DAC_IRQHandler(void)  
{
	if (TIM_GetITStatus(TIM6,TIM_IT_Update)!= RESET) 
	{
    usartOutTimeFlag = 1;  // 每2ms 设置一次usart超时
	 //printf("%d %d",20000,30000);    
	}
		TIM_ClearFlag(TIM6, TIM_FLAG_Update);	
}
	


