#include "main.h"

unsigned char Rx_data = 0;
char state = 0;
char usartOutTimeFlag = 1;
unsigned char orderbuff[32] = {0xAA,0xBB};
uint8_t countReceived = 0; 
char dataReady = 0;
extern	uint8_t work_mode ;

void USART2_INIT(void)   //  DMA  8BIT 偶校验 1个停止位 TX RX模式
{ 
    USART_InitTypeDef usart2;   // 只用一个io 作为接受
	  GPIO_InitTypeDef  gpio;
	  NVIC_InitTypeDef  NVIC_InitStructure;
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_DMA1,ENABLE);
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource3 ,GPIO_AF_USART2);       
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource2 ,GPIO_AF_USART2);    
	
	  gpio.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2;
	  gpio.GPIO_Mode = GPIO_Mode_AF;   
    gpio.GPIO_OType = GPIO_OType_PP; 
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    gpio.GPIO_PuPd = GPIO_PuPd_UP;
	  GPIO_Init(GPIOA,&gpio);
    
    USART_DeInit(USART2);
	  usart2.USART_BaudRate = 57600;   //SBUS 100K baudrate
	  usart2.USART_WordLength = USART_WordLength_8b;
	  usart2.USART_StopBits = USART_StopBits_1;
	  usart2.USART_Parity = USART_Parity_No;
	  usart2.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    usart2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //// none
	  USART_Init(USART2,&usart2);
   // USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);  
    USART_ClearFlag(USART2, USART_FLAG_TC);
	  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断
	  USART_Cmd(USART2,ENABLE);
		
	//Usart2 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、	 

}

void USART2_IRQHandler(void)
{
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART2,USART_IT_RXNE);
        Rx_data  = USART2->DR;
			  if(usartOutTimeFlag == 1)
				{
					state = 0;            //如果距离上次接受已经超时~ 则reset.
					countReceived = 0;    
				}
        usartOutTimeFlag = 0;   //每次进入 usart将 timeout清零
			switch(state)
			{
				case 0:if(Rx_data == 0xAA) state = 1;break;
				case 1:if(Rx_data == 0xBB) state = 2;
				        else if(Rx_data != 0xAA)state = 0; break;
				case 2: 
					   orderbuff[2] = Rx_data;
				     if(Rx_data == 0x10) state = 3;
				     else state = 4; 
				      break;
				
				case 3:  //  接受8字节 IP + 一个帧尾+一个校验
					    if(countReceived < 9) // 
							{	
								countReceived++;
								orderbuff[countReceived+2] = Rx_data; // 前三个单元放了 0xAA 0xBB order 
							}
							else                                    // 接受完成~                                     
							{
								countReceived++; 
							  orderbuff[countReceived+2] = Rx_data;
							  if(orderbuff[countReceived+2] == 0xFA) dataReady = setIP;// 检测校验位
							  else dataReady = 0;
								state = 0; countReceived = 0; 
							}
							break;
				case 4: // 接受4个字节的控制字 + 一个校验
				      if(countReceived < 4) // 
							{	
								countReceived++;
								orderbuff[countReceived+2] = Rx_data;
							}
							else 
							{
								countReceived++; 
							  orderbuff[countReceived+2] = Rx_data;
							  if(orderbuff[countReceived+2] == 0xFA) dataReady = setOrder;
							  else dataReady = 0;
								 state = 0; countReceived = 0; 
							}break;	
				
				default:break;
			}			  			
    }
		
		
}

int fputc(int ch, FILE *f)
{
    while (USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
    USART_SendData(USART2, (uint8_t)ch);
    return ch;
}

int sendData(uint8_t* ch, int num)
{
	int i ;
	for(i = 0; i<num;i++)
	{
		while (USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
    USART_SendData(USART2, (uint8_t)ch[i]);
	}
}

int sumCheck(unsigned char* in,char num)
{
	char i = 0;
	uint8_t temp=0;
	for(i=0;i<num-1;i++)
	{
		temp += in[i];
	}
	if(in[num-1]==temp)
		return 1;
	else return 0;
}
//void DMA_UARST_INIT()
//{
//	  NVIC_InitTypeDef  nvic; 
//    DMA_InitTypeDef   dma;
//	
//	  nvic.NVIC_IRQChannel = DMA1_Stream5_IRQn;     // 配置中断   拥有最高的中断优先级
//    nvic.NVIC_IRQChannelPreemptionPriority = 0;
//    nvic.NVIC_IRQChannelSubPriority = 0;
//    nvic.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&nvic);
//    
//    DMA_DeInit(DMA1_Stream5);
//    dma.DMA_Channel= DMA_Channel_4;
//    dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);    // use usart2
//    dma.DMA_Memory0BaseAddr = (uint32_t)buffer;      // memory addr
//    dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
//    dma.DMA_BufferSize = 18;
//    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;       //1 reg
//    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;                //18 byte
//    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//    dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//    dma.DMA_Mode = DMA_Mode_Circular;                        // circular transmit
//    dma.DMA_Priority = DMA_Priority_VeryHigh;                
//    dma.DMA_FIFOMode = DMA_FIFOMode_Disable;   
//    dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;     //?????  nerver mind
//    dma.DMA_MemoryBurst = DMA_Mode_Normal;
//    dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;      //?????
//    DMA_Init(DMA1_Stream5,&dma);

//    DMA_ITConfig(DMA1_Stream5,DMA_IT_TC,ENABLE);      //DMA interrupt   transform complete
//    DMA_Cmd(DMA1_Stream5,ENABLE);
//}

//void DMA1_Stream5_IRQHandler(void)        //  dma int -> usart complete update the data;   7ms一次  
//{
//    if(DMA_GetITStatus(DMA1_Stream5, DMA_IT_TCIF5))
//    {
// 		   DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF5);
//           DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF5);
//		
//	
//			
//			}//  竖直量不归一  云台控制另行	
//}
