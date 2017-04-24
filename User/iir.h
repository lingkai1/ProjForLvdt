#ifndef IIR_H
#define IIR_H




typedef struct tag_IIR_State_3Order

{
     double w01;
     double w02;
     double w11;
     double w12;
     double w21;
     double w22;
}IIR_State_6order;

short cy_signal_filter_by_6th_iir(short pcmIn, IIR_State_6order* filter_state,char wcType);
#endif