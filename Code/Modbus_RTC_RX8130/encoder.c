#include "sys.h"
//#include "encoder.h"

//IO口得根据实际的硬件来改动
//p1.0  P1.1   P1.2
#define CWPORT	1     //P1口
#define CWPIN	  3     //IO
#define CCWPORT	1     //P1口
#define CCWPIN	5     //IO
#define DGUSADDER      0X3000

u16 xdata back_Encoder_Data = 0;
u16 xdata set_value = 0;    
u16 xdata set_value_OK = 0; 
u16 xdata Encoder_Data_cnt = 0;
////======================================================
//===============================================
u8 GetPinIn(u8 Port,u8 Pin)
{	 
  	u8 value;
	  switch(Port)
		{
			  case 0:
					value=P0&(1<<Pin);
				break;
				case 1: 
					value=P1&(1<<Pin);
				break;
				case 2: 
					value=P2&(1<<Pin);
				break;
				case 3: 
					value=P3&(1<<Pin);
				break;	
        default:
					value=0;
				break;				
		}
		return value;
}

//============================================================
void delay_us(unsigned int t)
{ 
 u8 i;

 while(t)
 {
  for(i=0;i<50;i++)
   {i=i;}
  t--;
 }
}
//=============================================
void delay_ms(u16 t)
{
	u16 i,j;
	  for(i=0;i<t;i++)
	  {
			for(j=0;j<300;j++)
	     delay_us(1);
		}
}
//============================================================
u8 EncoderflagOld;


u16 Encoder_recevie(void)	
{
	u8 Encoder_flag = 0;
	u8 KEY_CW_PORT;	
	u8 KEY_CCW_PORT;
	static u8 rEncoder_Code = 0;
	static u8 rEn_Right_Save = 1;
	static u8 rEn_Left_Save = 1;
	static u8 F_Encoder_Check = 0;
	static u8 rEn_Right_Read;
	static u8 rEn_Left_Read;
	 //static u8 x=0;
    static u16 y=0;
	delay_us(250);
		if(GetPinIn(1,0))
		KEY_CW_PORT = 1;
	else
		KEY_CW_PORT = 0;
	if(GetPinIn(1,1))
		KEY_CCW_PORT = 1;
	else
		KEY_CCW_PORT = 0;
	//======================================
	 if(GetPinIn(1,2)==0)
    {
			while(1)
				{
					if(GetPinIn(1,2)!=0) { break;}
          y++;
          delay_ms(1);
        }
        if(y>10&&y<2000) { Encoder_flag = 3; }
        else  if(y>=2000) { Encoder_flag = 4; break;}
    }
    if(GetPinIn(1,2)!=0) { y=0;}
		
		//==============================================
	if((KEY_CW_PORT == 1)&&(KEY_CCW_PORT == 1))
	{
		if(F_Encoder_Check)
		{
			F_Encoder_Check = 0 ;
			if(rEncoder_Code ==0x42)     { Encoder_flag = 1;}
			else if(rEncoder_Code ==0x24){ Encoder_flag = 2;}
		}
		rEncoder_Code  = 0;
		rEn_Right_Save = 1;
		rEn_Left_Save  = 1;
	}
	else
	{
		F_Encoder_Check = 1 ;	
		if(KEY_CW_PORT)		{ rEn_Right_Read = 1 ; }
		else              { rEn_Right_Read = 0 ; }
		
		if(KEY_CCW_PORT) { rEn_Left_Read = 1 ; }
		else             { rEn_Left_Read = 0 ; } 

		if((rEn_Right_Read !=rEn_Right_Save)||(rEn_Left_Read !=rEn_Left_Save)) 
		{
		   rEn_Right_Save = rEn_Right_Read ;
		   rEn_Left_Save = rEn_Left_Read ;
		   rEncoder_Code = rEn_Right_Read + rEncoder_Code ;
		   rEncoder_Code = rEncoder_Code<<1;
		   rEncoder_Code = rEn_Left_Read + rEncoder_Code ;
		   rEncoder_Code = rEncoder_Code<<1;   
		}     
	}
	if(Encoder_flag == 2) 			{ Send_UART2_Encoder_Data(Encoder_flag);}
	else if(Encoder_flag == 1) { Send_UART2_Encoder_Data(Encoder_flag);}
	else if(Encoder_flag == 3) { Send_UART2_Encoder_Data(Encoder_flag);}
  else if(Encoder_flag == 4) { if (Encoder_flag != encoder_tmp_old) Send_UART2_Encoder_Data(Encoder_flag);}
	EncoderflagOld = Encoder_flag;
	return 0;
}

//=============================================================


  
  u16 Stata_ok = 0;
//===============================================================
void  Encoder_DGUS()
{
	Encoder_recevie();
}
	 


void Send_UART2_Encoder_Data(u8 set_data)
{
	u8 xdata i=0;
  u8 xdata UART2_Data[10];
	  UART2_Data[0]=0x5A;
    UART2_Data[1]=0xA5;
	  UART2_Data[2]=0x06;
	  UART2_Data[3]=0x83;
	  UART2_Data[4]=0x80;
	  UART2_Data[5]=0x00;
	  UART2_Data[6]=0x01;
	  UART2_Data[7]=0x00;
	  UART2_Data[8]=set_data;
	  for(i=0;i<9;i++)
	  OneSendData2( UART2_Data[i]);
}




