#ifndef __ENCODER_H__
#define __ENCODER_H__
#include "sys.h"



void delay_us(unsigned int t);
void  PinOutput(u8 Port,u8 Pin,u8 value);
u8 GetPinIn(u8 Port,u8 Pin);


void Send_UART2_Encoder_Data(u8 set_data);
void  OneSendData2_16(u16 D_data);
void  Encoder_DGUS();
u16 Encoder_recevie(void);
u16 Encoder_Set_Value(u16 unit_value,u16 upper_limit_value,u16 lower_limiting_value);

#endif