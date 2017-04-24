


#include "AD7606.h"
#include "main.h"

int16_t ADC_BUF[8];
int Offset = 0;
//  
int Offset_mid = 21411;
int Offset_up = -22319;
int Offset_down = -22989;
float rate;
float Offset_AD =0;
float realData=0;

float realData1=0;
float realData2=0;
float realData3=0;
float realData4=0;
float realData5=0;

void AD7606_GPIO_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB |RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
	//PA
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_9 | GPIO_Pin_8 | GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//PB14 RST
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//PD2  CSN
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//PC11 DATAIN
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void AD7606_READ(void)
{
	int i,j;
  u16 data=0;	
	
	AD7606_CONVTA_ON();
	AD7606_CONVTB_ON();
	delay_us(2);
	AD7606_CONVTA_OFF();
	AD7606_CONVTB_OFF();	
  AD7606_CLK_L();
	while(AD7606_BUSY()){};
			AD7606_CS_L();
  for(i=0;i<8;i++ ) // 8*16
	{	
		for(j=16;j>0;j--)
	  {  
	 		 data <<= 1;
	     data |= AD7606_ReadBIT();
	     AD7606_CLK_H();
	     AD7606_CLK_H();
	     AD7606_CLK_L();
	     AD7606_CLK_L();
    }
	   ADC_BUF[i] = data;
	}
   AD7606_CS_H();	
   realData = ((double)Offset_mid - (double)ADC_BUF[0])*rate;
   realData1 = ((double)Offset_mid - (double)ADC_BUF[0])*rate;
	 realData2 = ((double)Offset_mid - (double)ADC_BUF[0])*rate;
	 realData3 = ((double)Offset_mid - (double)ADC_BUF[0])*rate;
   realData4 = ((double)Offset_mid - (double)ADC_BUF[0])*rate;
	 realData5 = ((double)Offset_mid - (double)ADC_BUF[0])*rate;


	 
}

int ADC_Calibration(int mode)  // 30ms ~ 40 ms
{
    float K1 = 0.998;		//Offset filter coefficient K1: 0.05/(T+0.05);
    float K2 = 0.001999;	//Offset filter coefficient K2: T/(T+0.05);
	  int   i = 0;
	
	
	for(i=0;i<3000;i++)
	{	 
		AD7606_READ();
    Offset_AD = K1*Offset_AD + K2*(ADC_BUF[0]) ; 			//Phase A offset  每1ms 取 256组 一共30组 30ms
		delay_us(10);
	}
	if(mode == 0)
	 Offset_mid = (int)Offset_AD;
	if(mode == 1)
	 Offset_up = (int)Offset_AD;
	if(mode == 2)
	 Offset_down = (int)Offset_AD;
    return 1;
}

void AD7606_Init(void)
{
	AD7606_GPIO_Init();
	AD7606_OS0_L();              // OS[2:0]  110
	AD7606_OS1_H();
	AD7606_OS2_H();
	
	AD7606_CLK_H();
	
	AD7606_Reset_ON();
	AD7606_Reset_ON();
	AD7606_Reset_ON();
	delay_us(1);
	AD7606_Reset_OFF();
	AD7606_Reset_OFF();
	AD7606_Reset_OFF();
	
	//ADC_Calibration();
	rate = 2000/((2*(double)Offset_mid - ((double)Offset_up+(double)Offset_down))/2);
}
