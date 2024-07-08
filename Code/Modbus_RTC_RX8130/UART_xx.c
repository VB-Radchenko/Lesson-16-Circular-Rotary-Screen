#include "UART_xx.h"

         xdata   u8       R_u2[255];	  //串口2接受数组

          xdata   u8       R_OD2=0;	      //串口2收到数据
          xdata  u8        R_CN2=0;		  //口2长度计数器
         xdata  u8         T_O2=0;		  //口2超时计数器
 bit			                 Busy2=0;	      //串口2发送标志

//**********************************************************/
//         xdata  u8         R_u4[255];	  //串口4接受数组

//         xdata u8          R_OD4=0;	      //串口4收到数据
//         xdata  u8         R_CN4=0;		  //口4长度计数器
//        xdata u8           T_O4=0;		  //口4超时计数器
// bit			              Busy4=0;	      //串口4发送标志
/**********************************************************/
//       xdata  u8           R_u5[255];	  //串口5接受数组

//         xdata  u8         R_OD5=0;	      //串口5收到数据
//          xdata u8         R_CN5=0;		  //口5长度计数器
//        xdata u8           T_O5=0;		  //口5超时计数器
// bit			                   Busy5=0;	      //串口4发送标志

     xdata  u8      CrcH;
     xdata  u8      CrcL;
/*******************///秒标志
u16     Sec=1000;
bit     fang=0,fang1=0,fang2=0;

/*******************/



static idata u16 delay_tick = 0; //用来实现精确延时的
//利用定时器2进行精确延时,单位ms
//void sys_delay_ms(u16 ms)
//{
//	delay_tick = ms;
//	while(delay_tick);
//}
//==============================
//void  CRC_16(u8 Data)
//{		    u8 index=0;
//			index=CrcH^Data;CrcH=CrcL^WCRCh[index];CrcL=WCRCl[index]; 
//}

//****************************************************************************/
void  OneSendData2(u8 Dat)
{           while (Busy2);               
			Busy2 = 1;
			SBUF2 = Dat; 
}
///****************************************************************************/
//void  OneSendData4(u8 Dat)
//{           while (Busy4);               
//			Busy4 = 1;
//			SBUF4_TX = Dat; 
//}
/****************************************************************************/
//void  OneSendData5(u8 Dat)
//{           while (Busy5);               
//			Busy5 = 1;
//			SBUF5_TX = Dat; 
//}

//////===============================
//char putchar (char c)   
//{ 

//	OneSendData5(c);
//	return 0;
//}
void ReceiveDate2()
{
	u8 cmd;u8 j=0,i=0;
	u8 num=0,len=0;
	u16 Temp=0;
	u8 Rx_uart2[100];
	//printf("Modbus string:");
	do
	{
		if((R_u2[num]==0x5A)&&(R_u2[num+1]==0xA5)) 
		{
			//printf("Modbus string:111");
			cmd=R_u2[num+3];
			switch(cmd)
			{
				case	0x82:		 
					len=(R_u2[num+2]-3)/2;
				  for(j=0;j<len;j++)
				  Write_Dgus((((R_u2[num+4]<<8)|R_u2[num+5])+j),((R_u2[num+6+j*2]<<8)|R_u2[num+7+j*2]));
				   Rx_uart2[0]=0x5A;
					 Rx_uart2[1]=0xA5;
					 Rx_uart2[2]=0x03;
					 Rx_uart2[3]=0x82;
					 Rx_uart2[4]=0x4F;
					 Rx_uart2[5]=0x4B;
				   for(i=0;i<6;i++)
				   OneSendData2(Rx_uart2[i]);
					break;
				case	0x83:	
				   Rx_uart2[0]=R_u2[num];
					 Rx_uart2[1]=R_u2[num+1];
					 Rx_uart2[3]=R_u2[num+3];
					 Rx_uart2[4]=R_u2[num+4];
					 Rx_uart2[5]=R_u2[num+5];
					 Rx_uart2[6]=R_u2[num+6];	
					for(i=0;i<R_u2[num+6];i++)
					{                                    
						 Temp=Read_Dgus(((R_u2[num+4]<<8)+R_u2[num+5]+i));
						 Rx_uart2[7+2*i]=(u8)(Temp>>8);
						 Rx_uart2[8+2*i]=(u8)(Temp);
					}
				  Rx_uart2[2]=(2*R_u2[num+6])+4;
					R_u2[num+2]=(2*R_u2[num+6])+4;
					for(i=0;i<(R_u2[num+2]+3);i++)
					OneSendData2( Rx_uart2[i]);
					break;
				default:
					break;
			 }
			
		}
	
	  num++;
	}while(num < (R_CN2 - 4));
	
}
/********************************************/
void T0_ISR_PC(void)    interrupt 1
{
    
    TH0=T1MS>>8;
    TL0=T1MS;
	

}
/*******************************************/
void T1_ISR_PC(void)    interrupt 3
{
    
    TH1=T1MS>>8;
    TL1=T1MS;
	
}
//定时器2中断服务程序
void sys_timer2_isr()	interrupt 5
{
	TF2=0;//清除定时器2的中断标志位//精准延时处理
	SysTick_RTC++;//RTC时钟控制
	if(delay_tick) delay_tick--;
	//if(T_O4>0)T_O4--;
	//if(T_O5>0)T_O5--;
  if(T_O2>0)T_O2--;
	if(Sec>0) Sec--;if(Sec==0){Sec=1000;fang=1;fang1=1;fang2=1;}
		
}

//void uart5_Tisr()	    interrupt 12  //串口5发送中断
//{           SCON5T&=0xFE ; 
//			      Busy5=0;
//}
////*********************************************************
//void uart5_Risr()	    interrupt 13   //串口5中断接收
//{           R_u5[R_CN5]=SBUF5_RX;
//            SCON5R&=0xFE;
//            R_OD5=1;
//            R_CN5++;
//            T_O5=5;   
//}
//*********************************************************

//void uart4_Tisr()	    interrupt 10  //串口4发送中断
//{           SCON4T&=0xFE ; 
//			Busy4=0;
//}
////*********************************************************
//void uart4_Risr()	    interrupt 11   //串口4中断接收
//{           R_u4[R_CN4]=SBUF4_RX;
//            SCON4R&=0xFE;
//            R_OD4=1;
//            R_CN4++;
//            T_O4=5;   
//}
////*********************************************************
void uart2_Risr()	    interrupt 4   
{           
           if(RI2==1)
					 {R_u2[R_CN2]=SBUF2;
            SCON2&=0xFE;
            R_OD2=1;
            R_CN2++;
            T_O2=10;    
					 }
					 if(TI2==1)
					 { SCON2&=0xFD ; 
			       Busy2=0;}
}
