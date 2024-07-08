#ifndef __RTC_WIN_H__
#define __RTC_WIN_H__
#include "sys.h"


sbit	RTC_SDA	=	P3^3;
sbit	RTC_SCL	=	P3^2;


extern  u16 SysTick_RTC;
extern  bit   RTC_Flog;

void Delayus();
void SDA_IN();
void SDA_OUT();
void Rx8130_Start()	;
void Rx8130_Stop();
void Rx8130_Mack();
void Rx8130_Mnak();
void Rx8130_Cack();
void Rx8130_Writ_Byte(u8 dat);
u8 Rx8130_Read_Byte();
void Read_Time();
void Init_Rtc();
void RTC_Set_Time();
u8 RTC_Get_Week(u8 years,u8 month,u8 day);
void        Clock();












void Time_Control();
#endif
