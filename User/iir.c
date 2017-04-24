#include "iir.h"
//直接二形
//6阶 最小P范数IIR滤波器

float den0[3][3]={// den
               { 1,  -0.4803320178219,   0.8663410158227 },
							 { 1,  -0.5759049572574,   0.5504626188741 },
							 { 1,  -0.7339395183422,   0.2087136458117 }
               };
float num0[3][3]={
	             {1,  1.650323417802,   0.9900884909025},
							 {1,  0.5115551631838,  0.9995921105269},
	             {1,  0.04767870489813, 0.9999986662437}
               };
float gain0 = 0.03414078953606;
							 
float den1[3][3]={// den
               { 1,  -0.4803320178219,   0.8663410158227 },
							 { 1,  -0.5759049572574,   0.5504626188741 },
							 { 1,  -0.7339395183422,   0.2087136458117 }
               };
float num1[3][3]={
	             {1,  1.650323417802,   0.9900884909025},
							 {1,  0.5115551631838,  0.9995921105269},
	             {1,  0.04767870489813, 0.9999986662437}
               };
float gain1 = 0.03414078953606;
							 
float den2[3][3]={// den
               { 1,  -0.4803320178219,   0.8663410158227 },
							 { 1,  -0.5759049572574,   0.5504626188741 },
							 { 1,  -0.7339395183422,   0.2087136458117 }
               };
float num2[3][3]={
	             {1,  1.650323417802,   0.9900884909025},
							 {1,  0.5115551631838,  0.9995921105269},
	             {1,  0.04767870489813, 0.9999986662437}
               };
float gain2 = 0.03414078953606;
							 
float den3[3][3]={// den
               { 1,  -0.4803320178219,   0.8663410158227 },
							 { 1,  -0.5759049572574,   0.5504626188741 },
							 { 1,  -0.7339395183422,   0.2087136458117 }
               };
float num3[3][3]={
	             {1,  1.650323417802,   0.9900884909025},
							 {1,  0.5115551631838,  0.9995921105269},
	             {1,  0.04767870489813, 0.9999986662437}
               };
float gain3 = 0.03414078953606;

float den4[3][3]={// den
               { 1,  -0.4803320178219,   0.8663410158227 },
							 { 1,  -0.5759049572574,   0.5504626188741 },
							 { 1,  -0.7339395183422,   0.2087136458117 }
               };
float num4[3][3]={
	             {1,  1.650323417802,   0.9900884909025},
							 {1,  0.5115551631838,  0.9995921105269},
	             {1,  0.04767870489813, 0.9999986662437}
               };
float gain4 = 0.03414078953606;




short cy_signal_filter_by_6th_iir(short pcmIn, IIR_State_6order* filter_state, char wc)
{
	float (*num)[3], (*den)[3], *gain;//
  double x1, x2, x3, Tmp_f00, Tmp_f10, Tmp_f20;
  short i; 
  double Tmp_pcm;
	
	switch(wc)  // 选择正确的参数~
	{
		case 0x00: num = num0; den = den0; gain = &gain0; break;
		case 0x01: num = num1; den = den1; gain = &gain1; break;
		case 0x02: num = num2; den = den2; gain = &gain2; break;
		case 0x03: num = num3; den = den3; gain = &gain3; break;
		default: break;
	
	}

	

         Tmp_pcm = pcmIn;
         Tmp_f00 = Tmp_pcm - den[0][1] * filter_state->w01 - den[0][2] * filter_state->w02;
         x1 = Tmp_f00 + num[0][1] * filter_state->w01 + num[0][2] * filter_state->w02;
         filter_state->w02 = filter_state->w01;
         filter_state->w01 = Tmp_f00;
         x1 = (*gain) * x1;
			 
         Tmp_f10 = x1 - den[1][1] * filter_state->w11 - den[1][2] * filter_state->w12;
         x2 = Tmp_f10 + num[1][1] * filter_state->w11 + num[1][2] * filter_state->w12;
         filter_state->w12 = filter_state->w11;
         filter_state->w11 = Tmp_f10;
			 
			 
         Tmp_f20 = x2 - den[2][1] * filter_state->w21 - den[2][2] * filter_state->w22;
         x3 = Tmp_f20 * num[2][0] + num[2][1] * filter_state->w21 + num[2][2] * filter_state->w22;
         filter_state->w22 = filter_state->w21;
         filter_state->w21 = Tmp_f20;
         if (x3 >= 32767)
         {
              x3 = 32767;
         }
         if (x3 <= -32768)
         {
              x3 = -32768;
         }
         return (short)x3;

}
