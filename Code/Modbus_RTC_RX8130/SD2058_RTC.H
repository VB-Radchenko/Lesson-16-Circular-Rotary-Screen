#ifndef __RTC_SD2058_H__
#define __RTC_SD2058_H__
#include "sys.h"




sbit	RTC_SDA	=	P3^3;
sbit	RTC_SCL	=	P3^2;

extern  u16 SysTick_RTC;
extern  bit   RTC_Flog;

void Delayus();
void SDA_IN();
void SDA_OUT();
void SD2058_Start()	;
void SD2058_Stop();
void SD2058_Mack();
void SD2058_Mnak();
void SD2058_Cack();
void SD2058_Writ_Byte(u8 dat);
u8 SD2058_Read_Byte();
void Read_Time();
void Init_Rtc();
void RTC_Set_Time();
u8 RTC_Get_Week(u8 years,u8 month,u8 day);
void        Clock();













#endif
