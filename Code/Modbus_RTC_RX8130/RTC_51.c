#include "sys.h"

u16 SysTick_RTC   =0; 
bit   RTC_Flog    =0;

void Delayus() {u8 i;for(i=0;i<80;i++);}  //50
//===========================
void SDA_IN()  {P3MDOUT=P3MDOUT&0xF7;}
//===========================
void SDA_OUT() {P3MDOUT=P3MDOUT|0x08;}
//==========================
void Rx8130_Start()
{	
						SDA_OUT();
						RTC_SDA=1;
						RTC_SCL=1;
						Delayus();  //15
						RTC_SDA=0;
						Delayus();  //15
						RTC_SCL=0;
						Delayus();  //15
}
//================================
void Rx8130_Stop()
{	
						SDA_OUT();
						RTC_SDA=0;
						RTC_SCL=1;
						Delayus();//15
						RTC_SDA=1;
						Delayus();//15
						SDA_IN();
}
//================================
void Rx8130_Mack()
{	
						SDA_OUT();
						RTC_SDA=0;
						Delayus();
						RTC_SCL=1;
						Delayus();
						RTC_SCL=0;
						Delayus();
}
//=================================
void Rx8130_Mnak()
{	
						SDA_OUT();
						RTC_SDA=1;
						Delayus();
						RTC_SCL=1;
						Delayus();
						RTC_SCL=0;
						Delayus();
}
//=================================
void Rx8130_Cack()
{	
						u8 i;
						SDA_IN();
						RTC_SDA=1;
						Delayus();
						RTC_SCL=1;
						Delayus();
						for(i=0;i<50;i++)
						{   
										if(!RTC_SDA) break;
										Delayus();
						}
						RTC_SCL=0;
						Delayus();
						SDA_OUT();
}

//=========================================
void Rx8130_Writ_Byte(u8 dat)
{	
						u8 i;
						SDA_OUT();
						for(i=0;i<8;i++)
						{	
									    if(dat&0x80)   RTC_SDA=1;
										else           RTC_SDA=0;
										dat=(dat<<1);
										Delayus();
										RTC_SCL=1;
										Delayus();
										RTC_SCL=0;
										Delayus();
						}
						Rx8130_Cack();
}

//====================================================
u8 Rx8130_Read_Byte()
{	
						u8 i;
						u8 Dat;
						SDA_IN();
						for(i=0;i<8;i++)
						{	
										Delayus();
										RTC_SCL=1;
										Delayus();
										Dat=(Dat<<1);
										if(RTC_SDA)  Dat=Dat|0x01;
										else         Dat=Dat&0xFE;
										RTC_SCL=0;
										Delayus();
						}
						return(Dat);
}
u8 RTC_Get_Week(u8 years,u8 month,u8 day)
{	
	u8 xdata weak;
	u16 xdata year;
	year= years+2000;
  if(month==1||month==2)
	{
		month += 12, year--;
	}
  if((year<1752)||(year==1752&&month<9)||(year==1752&&month==9&&day<3))
	{
		weak = (day + 2*month + 3*(month+1)/5 + year + year/4 + 6)%7;
	}
  else
	{
		weak = (day +1 + 2*month + 3*(month+1)/5 + year + year/4 -year/100 + year/400)%7;
	}
   return weak;
}
//把RTC读取并处理，写到DGUS对应的变量空间，主程序中每0.5秒调用一次
void Read_Time()
{	
						u8 i,N,M; 
	                    u8 Rtcdata[]={0,0,0,0,0,0,0,0};
						Rx8130_Start();
						Rx8130_Writ_Byte(0x64);
						Rx8130_Writ_Byte(0x10);
						Rx8130_Stop();
						Rx8130_Start();
						Rx8130_Writ_Byte(0x65);
						for(i=6;i>0;i--)
						{	
													Rtcdata[i]=Rx8130_Read_Byte();
													Rx8130_Mack();
						}
						Rtcdata[0]=Rx8130_Read_Byte(); Rx8130_Mnak();
						Rx8130_Stop();
						for(i=0;i<3;i++)	//年月日转换成HEX
						{	
							                        N=Rtcdata[i]/16;
													M=Rtcdata[i]%16;
													Rtcdata[i]=N*10+M;
						}
						for(i=4;i<7;i++)	//时分秒转换成HEX
						{	
													N=Rtcdata[i]/16;
													M=Rtcdata[i]%16;
													Rtcdata[i]=N*10+M;
						}
						Rtcdata[3]=RTC_Get_Week(Rtcdata[0],Rtcdata[1],Rtcdata[2]);//周
						Rtcdata[7]=0;
						Write_Dgus(0X0010,(Rtcdata[0]<<8)+Rtcdata[1]);
						Write_Dgus(0X0011,(Rtcdata[2]<<8)+Rtcdata[3]);
						Write_Dgus(0X0012,(Rtcdata[4]<<8)+Rtcdata[5]);
						Write_Dgus(0X0013,(Rtcdata[6]<<8)+Rtcdata[7]);
}
//====================================================================
void Init_Rtc()
{	
					   u8 i;
				  	   Rx8130_Start();
					   Rx8130_Writ_Byte(0x64);
					   Rx8130_Writ_Byte(0x1d);
					   Rx8130_Stop();
					   Rx8130_Start();
					   Rx8130_Writ_Byte(0x65);
					   i=Rx8130_Read_Byte();
					   Rx8130_Mack();
					   Rx8130_Read_Byte();
					   Rx8130_Mnak();
					   Rx8130_Stop();
					   if((i&0x02)==0x02)
				       {	
									//重新配置时间
									Rx8130_Start();		//30=00
									Rx8130_Writ_Byte(0x64);
									Rx8130_Writ_Byte(0x30);
									Rx8130_Writ_Byte(0x00);
									Rx8130_Stop();
									Rx8130_Start();		//1C-1F=48 00 40 10
									Rx8130_Writ_Byte(0x64);
									Rx8130_Writ_Byte(0x1C);
									Rx8130_Writ_Byte(0x48);
									Rx8130_Writ_Byte(0x00);
									Rx8130_Writ_Byte(0x40);
									Rx8130_Writ_Byte(0x10);
									Rx8130_Stop();
									Rx8130_Start();		//10-16=RTC设置值 BCD格式
									Rx8130_Writ_Byte(0x64);
									Rx8130_Writ_Byte(0x10);
									Rx8130_Writ_Byte(0x00);	//秒
									Rx8130_Writ_Byte(0x00);	//分
									Rx8130_Writ_Byte(0x00);	//时
									Rx8130_Writ_Byte(0x01);	//星期
									Rx8130_Writ_Byte(0x01);	//日
									Rx8130_Writ_Byte(0x01);	//月
									Rx8130_Writ_Byte(0x17);	//年
									Rx8130_Stop();
									Rx8130_Start();		//1E-1F 00 10
									Rx8130_Writ_Byte(0x64);
									Rx8130_Writ_Byte(0x1E);
									Rx8130_Writ_Byte(0x00);	
									Rx8130_Writ_Byte(0x10);			
									Rx8130_Stop();
				       }
}
//=========================================
void RTC_Set_Time()
{					    
						u16  Va=Read_Dgus(0x009C); //0x5AA5
						if(Va==0x5AA5)
						{			u8 N=0,S=0;bit  Flog=1;
									u8 TimeS[]={0,0,0,0,0,0,0};
									u16  V1=Read_Dgus(0x009D); //年，月
									N=(u8)(V1>>8);TimeS[0]=((N/10)*16)+(N%10);
									N=(u8)(V1);   TimeS[1]=((N/10)*16)+(N%10);
									V1=Read_Dgus(0x009E); //日，星期
									N=(u8)(V1>>8);TimeS[2]=((N/10)*16)+(N%10);
									TimeS[3]=RTC_Get_Week(TimeS[0],TimeS[1],TimeS[2]);//周
									N=(u8)(V1);   TimeS[4]=((N/10)*16)+(N%10);
									V1=Read_Dgus(0x009F); //时，分
									N=(u8)(V1>>8);TimeS[5]=((N/10)*16)+(N%10);
									N=(u8)(V1);   TimeS[6]=((N/10)*16)+(N%10);
//						            V1=Read_Dgus(0xDFFF); //秒，x  
//						            N=(u8)(V1>>8);TimeS[6]=((N/10)*16)+(N%10);
									Write_Dgus(0x009c,0);	
									while(Flog)
									{	
										        Rx8130_Start();		//30=00
												Rx8130_Writ_Byte(0x64);
												Rx8130_Writ_Byte(0x30);
												Rx8130_Writ_Byte(0x00);
												Rx8130_Stop();
												Rx8130_Start();		//1C-1F=48 00 40 10
												Rx8130_Writ_Byte(0x64);
												Rx8130_Writ_Byte(0x1C);
												Rx8130_Writ_Byte(0x48);
												Rx8130_Writ_Byte(0x00);
												Rx8130_Writ_Byte(0x40);
												Rx8130_Writ_Byte(0x10);
												Rx8130_Stop();
												Rx8130_Start();		//10-16=RTC设置值 BCD格式
												Rx8130_Writ_Byte(0x64);
												Rx8130_Writ_Byte(0x10);
												Rx8130_Writ_Byte(TimeS[6]);	//秒
												Rx8130_Writ_Byte(TimeS[5]);	//分
												Rx8130_Writ_Byte(TimeS[4]);	//时
												Rx8130_Writ_Byte(TimeS[3]);	//星期
												Rx8130_Writ_Byte(TimeS[2]);	//日
												Rx8130_Writ_Byte(TimeS[1]);	//月
												Rx8130_Writ_Byte(TimeS[0]);	//年
												Rx8130_Stop();
												Rx8130_Start();		//1E-1F 00 10
												Rx8130_Writ_Byte(0x64);
												Rx8130_Writ_Byte(0x1E);
												Rx8130_Writ_Byte(0x00);	
												Rx8130_Writ_Byte(0x10);			
												Rx8130_Stop(); 
												Flog=0;
								   }
						}
}
//===================================
void        Clock()
{
                  	   if(RTC_Flog==0){ RTC_Flog=1;  Init_Rtc(); }
				       if(SysTick_RTC >= 500) 
				       {
										Read_Time();								//更新硬件RTC时间
										SysTick_RTC = 0;
				       }
					   RTC_Set_Time();
}
