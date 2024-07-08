#ifndef __UARTxx_H__
#define __UARTxx_H__

#include "sys.h"


  extern     xdata u8        CrcH;
  extern     xdata u8        CrcL;

 



  extern u8          xdata         R_u2[255];	  //串口2接受数组
//u8          xdata         W_u2[135];	  //串口2发送数组
 extern u8          xdata         R_OD2;	      //串口2收到数据
  extern u8          xdata         R_CN2;		  //口2长度计数器
  extern u8          xdata         T_O2;		  //口2超时计数器
  extern bit			                 Busy2;	      //串口2发送标志

////**********************************************************/
//  extern u8          xdata         R_u4[255];	  //串口4接受数组
////u8          xdata         W_u4[135];	  //串口4发送数组
//  extern u8          xdata         R_OD4;	      //串口4收到数据
// extern  u8          xdata         R_CN4;		  //口4长度计数器
//  extern u8          xdata         T_O4;		  //口4超时计数器
//  extern bit			              Busy4;	      //串口4发送标志
/**********************************************************/
//  extern u8          xdata         R_u5[255];	  //串口5接受数组
////u8          xdata         W_u5[135];	  //串口5发送数组
//  extern u8          xdata         R_OD5;	      //串口5收到数据
//  extern u8          xdata         R_CN5;		  //口5长度计数器
//  extern u8          xdata         T_O5;		  //口5超时计数器
//  extern bit			                   Busy5;	      //串口4发送标志

 extern u16    Sec;
 extern bit   fang,fang1,fang2;
 extern u8    DATA_SEC;
 extern u8    SecIin;







void  OneSendData5(u8 Dat);
void  OneSendData4(u8 Dat);
void  OneSendData2(u8 Dat);
void  ReceiveDate2();
void  ReceiveDate4();
void  ReceiveDate5();
void sys_delay_ms(u16 ms);
void  CRC_16(u8 Data);

 
void  Sw_Data_Send ();

void UART5_SendStr(u8 *pstr,u8 strlen);
void UART4_SendStr(u8 *pstr,u8 strlen);
#endif