#ifndef __UARTxx_H__
#define __UARTxx_H__

#include "sys.h"


  extern     xdata u8        CrcH;
  extern     xdata u8        CrcL;

 



  extern u8          xdata         R_u2[255];	  //����2��������
//u8          xdata         W_u2[135];	  //����2��������
 extern u8          xdata         R_OD2;	      //����2�յ�����
  extern u8          xdata         R_CN2;		  //��2���ȼ�����
  extern u8          xdata         T_O2;		  //��2��ʱ������
  extern bit			                 Busy2;	      //����2���ͱ�־

////**********************************************************/
//  extern u8          xdata         R_u4[255];	  //����4��������
////u8          xdata         W_u4[135];	  //����4��������
//  extern u8          xdata         R_OD4;	      //����4�յ�����
// extern  u8          xdata         R_CN4;		  //��4���ȼ�����
//  extern u8          xdata         T_O4;		  //��4��ʱ������
//  extern bit			              Busy4;	      //����4���ͱ�־
/**********************************************************/
//  extern u8          xdata         R_u5[255];	  //����5��������
////u8          xdata         W_u5[135];	  //����5��������
//  extern u8          xdata         R_OD5;	      //����5�յ�����
//  extern u8          xdata         R_CN5;		  //��5���ȼ�����
//  extern u8          xdata         T_O5;		  //��5��ʱ������
//  extern bit			                   Busy5;	      //����4���ͱ�־

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