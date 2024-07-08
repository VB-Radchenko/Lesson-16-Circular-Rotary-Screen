#ifndef __SYS_H__
#define __SYS_H__

#include    "Dwin_T5L1H.h"
#include    "UART_xx.h"
#include    "RTC_51.h"
#include     <stdio.h>
#include     "string.h"
#include     "modbus.h"
#include     "Crc16.h"
#include                            "encoder.h"





//º¯ÊýÉêÃ÷
void Page_Change_Handler(u16 n);
void CPU_Inite();
void sys_delay_about_ms(u16 ms);
void sys_delay_about_us(u8 us);
void sys_delay_ms(u16 ms);
void read_dgus_vp(u32 addr,u8* buf,u16 len);
void write_dgus_vp(u32 addr,u8* buf,u16 len);
u16 Read_Dgus(u16 Dgus_Addr);
void Write_Dgus(u16 Dgus_Addr,u16 Val);
void  Multi_Data_Send ();  
void  ReceiveDate2();
void Write_flash(u8 place ,u16 falsh_adder,u8 len,u16 vla_adder);
void Read_flash(u8 place ,u16 falsh_adder,u8 len,u16 vla_adder);
void T0_Init(void);
void T1_Init(void);
#endif


