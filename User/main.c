
#include "main.h"
// attetion�� ÿ�ν���W5500��ʱ�򶼻�ر����ж�
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SOCK_UDPS        0
#define DATA_BUF_SIZE   2048
#define NFILTER          0x40
#define FIR              0x20
#define IIR              0x30

/* Private macro -------------------------------------------------------------*/
uint8_t Order_buff[DATA_BUF_SIZE];

//extern value
extern float h[11];
extern float h1[11];
extern float h2[11];
extern float h3[11];
extern int16_t ADC_BUF[8]; // from ad7606
extern float rate;         // from ad7606
extern float realData;     // from ad7606
extern unsigned char orderbuff[32]; // from usart2
extern char dataReady;              // from usart2
float ff = 7.99;
extern wiz_NetInfo gWIZNETINFO;
extern 	uint8_t DstIP[];
extern 	int32_t ret;
extern	uint16_t DstPort;														
// private variables
	char filterSelect = 0x40;			
  char sendFreq = 0x02;	
  char orderNum = 0x00;
  char wcType   = 0x00;														
  uint8_t work_mode = 1;
	uint8_t send_buf[16];
	int16_t filterData;
	IIR_State_6order iirState = {0,0,0,0,0,0};
	 float ui=0.7,u2=1.6,u3=0.8;
	double uu=0.7,uu2=1.6,uu3=0.8;
	int  uuu= 0, uuu1=1,uuu3=2;
int main(void)
{

	//Host dependent peripheral initialized
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);   
//	Net_platform_init();
	u3=u2/ui;
//	while(1)
//	{
//		int i =1;
//		for (i=0;i<10;i++)
//		{
//		delay_ms(100);
//		delay_ms(100);
//		delay_ms(100);
//		delay_ms(100);
//		delay_ms(100);
//		delay_ms(100);
//		delay_ms(100);
//		delay_ms(100);
//		delay_ms(100);
//		delay_ms(100);
//		}
//			delay_ms(100);
//		i=-9;
//  }
//	network_init();
//  AD7606_Init();		
	//ADC_Calibration(1);
	//ADC_Calibration(2);
	
	
	
	while(1)
	{
		AD7606_READ();
		filterData = ADC_BUF[0];
		if(dataReady != 0)
		{
			orderProcess(dataReady);
			dataReady = 0;			
		}
		if(filterSelect == 0x30)
		{	
			filterData = cy_signal_filter_by_6th_iir(ADC_BUF[0], &iirState ,wcType);
				//filterData = fir_filter(ADC_BUF[0],h);
		}
		else if(filterSelect == FIR)
		{
//			filterData = fir_filter(ADC_BUF[0],h);
//			switch(wcType)
//			{
//				case 0x00: filterData = fir_filter(ADC_BUF[0],h);break;
//				case 0x01: filterData = fir_filter(ADC_BUF[0],h1);break;
//				case 0x02: filterData = fir_filter(ADC_BUF[0],h2);break;
//				case 0x03: filterData = fir_filter(ADC_BUF[0],h3);break;
//				default: break;
//			}
		}
			
		//ret = recvfrom(SOCK_UDPS,Order_buff,DATA_BUF_SIZE,DstIP,&DstPort);	
		if(ret > 0)
		{// there is data been received ~
			
		}
			
		
		
		
//		switch(getSn_SR(SOCK_UDPS))	//��ȡsocket0��״̬
//		{
//			//socket�ȴ��ر�״̬
//			case SOCK_CLOSE_WAIT:
//         printf("%d:CloseWait\r\n",SOCK_UDPS);
//         if((ret=disconnect(SOCK_UDPS)) != SOCK_OK){
//					 break;
//				 }
//         printf("%d:Closed\r\n",SOCK_UDPS);
//         break;
//			//socket�ر�
//			case SOCK_CLOSED:
//				ret = socket(SOCK_UDPS,Sn_MR_UDP,45454,0);//��socket0��һ���˿�
//				if(ret != SOCK_UDPS){
//					printf("%d:Socket Error\r\n",SOCK_UDPS);
//					while(1);
//				}else{
//					printf("Socket OK\n\r");
//			     	}
//				break;
//     }		
//		ret = recvfrom(SOCK_UDPS,Order_buff,DATA_BUF_SIZE,DstIP,&DstPort);	//W5200��������UDP������
//		if(ret > 0){
//		sended = sendto(SOCK_UDPS,Order_buff,ret,DstIP,45454);							//W5200���ؽ��յ������� ������������Sn_IR�Ĵ���
//			work_mode = Order_buff[2];
//		}
//		switch(work_mode)
//		{
//			case 0:  memcpy(send_buf,ADC_BUF,16);
//			         sended = sendto(SOCK_UDPS,send_buf,16,DstIP,45454); 
//			         printf("%d",ADC_BUF[0]);
//			         break;
//			case 1:  memcpy(send_buf ,&realData,sizeof(realData));
//				           sendto(SOCK_UDPS,send_buf,16,DstIP,45454); 
//			         printf("%f",realData);
//			         //  printf("%f\n",ff);
//			         break;
//			
//			//
//		}
//		delay_ms(1);
	
		//	if(sended != ret){
			//	printf("%d:Send Error %d\n\r",SOCK_UDPS,sended);
	     //  }
  }//while
}//main

/**
  * @brief  Loopback Test Example Code using ioLibrary_BSD	
  * @retval None
  */
void Net_platform_init(void)
{

	SystemInit();//ϵͳʱ�ӳ�ʼ��
	//USART_Configuration();//����1��ʼ��
	USART2_INIT();
	SPI_Configuration();
	delay_init(168);
	
	//TIM6_Configuration();
	TIM4_Configuration(8000, 84-1); //��λ1us�� 1000ms
}
/*********************************END OF FILE**********************************/
void orderProcess(char order)
{
	if(order == setOrder)
	{
		switch(orderbuff[2]) // order
		{
      case 0x20: filterSelect = FIR;                     break;
			case 0x30: filterSelect = IIR;                     break;
			case 0x40: filterSelect = NFILTER;                 break;
			default:  break;			
		}				
		sendFreq = orderbuff[3];
    orderNum = orderbuff[4];	
    wcType = orderbuff[5];		
		sendData((char*)orderbuff,8);   // ������ܳɹ���������ͬ��֡
		
		if(sendFreq == 0)                 // 500hz
	  	TIM4_Configuration(2000,83);
		else if(sendFreq == 1)
			TIM4_Configuration(4000,83);  // 250hz
		else if(sendFreq == 2)
			TIM4_Configuration(8000,83);  // 125hz		
	}
	else if(order == setIP)
	{
		gWIZNETINFO.ip[0] = orderbuff[3];
		gWIZNETINFO.ip[1] = orderbuff[4];
		gWIZNETINFO.ip[2] = orderbuff[5];
		gWIZNETINFO.ip[3] = orderbuff[6];
		DstIP[0] = orderbuff[7];
		DstIP[1] = orderbuff[8];
		DstIP[2] = orderbuff[9];
		DstIP[3] = orderbuff[10];		
		disconnect(SOCK_UDPS);
		network_init();                           // ���³�ʼ����~
		ret = socket(SOCK_UDPS,Sn_MR_UDP,45454,0);//��socket0��һ���˿�
		if(ret != SOCK_UDPS)                       
		{
			printf("%d:Socket Error\r\n",SOCK_UDPS);
		}
		else
		{
			printf("Socket OK\n\r");
		}
		sendData((char*)orderbuff,13);   // if the network set is successful~ send back the same frame		
	}
}