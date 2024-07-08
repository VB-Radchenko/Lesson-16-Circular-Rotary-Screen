#include "sys.h"


//核心寄存器初始化
void sys_cpu_init()
{
			EA = 0;
			RS0 = 0;	      //Psw.3
			RS1 = 0;		  //Psw.4
			CKCON = 0x00;	  //CPU运行=0, 1T模式
			T2CON = 0x70;	  //
			DPC = 0x00;		  //
	    P3MDOUT           = 0x0C;					   //RTC时钟输出
      P3                = 0x00;  
			MUX_SEL|=0x40;    //UART2引出,UART3不引出，WDT关闭P0.4和P0.5
			P0MDOUT|=0x03;	  //p0.0 p0.1 强推 用于485方向控制
			RAMMODE =0x00;	  //
			PORTDRV =0x01;    //驱动强度+/-8mA
			IEN0 = 0x00;      //关闭所有中断
			IEN1 = 0x00;
			IEN2 = 0x00;
			IP0 = 0x00;       //中断优先级默认
			IP1 = 0x00;
			WDT_OFF();        //关闭看门狗
	
	//========================================串口2初始化*/
			P0MDOUT &= 0xCF;
			P0MDOUT |= 0x10;//设置对应的IO口输出输入
			ADCON = 0x80;//选择SREL0H:L作为波特率发生器
			SCON2 = 0x50;//接受使能和模式设置
			PCON &= 0x7F;//SMOD=0 64      SMOD=1    32
			SREL2H = (u8)(UART2_BPR>>8);  
			SREL2L = (u8)(UART2_BPR);  
			IEN0 = 0X10;//中断使能
			TR2=0;
	//========================================串口4初始化*/
//			BODE4_DIV_H =(u8)(UART4_BPR>>8); 
//			BODE4_DIV_L =(u8)(UART4_BPR); 
//		    SCON4T= 0x80;     //发送使能和模式设置
//		    SCON4R= 0x80;     //接受使能和模式设置 
//			  ES4R = 1;         //中断接受使能
//		    ES4T = 1;         //中断发送使能
//			  TR4=0;
//========================================
//========================================串口5初始化*/
//			BODE5_DIV_H =(u8)(UART4_BPR>>8);
//			BODE5_DIV_L =(u8)(UART4_BPR);
//		    SCON5T= 0x80;     //发送使能和模式设置
//		    SCON5R= 0x80;     //接受使能和模式设置 
//			  ES5R = 1;         //中断接受使能
//		    ES5T = 1;         //中断发送使能
//			  TR5=0;
}


//定时器2初始化,定时间隔为1ms
void sys_timer2_init()
{
	    TH2 = 0x00;
			TL2 = 0x00;
			T2CON = 0x70;
			TRL2H = 0xBC;	 //1ms的定时器
			TRL2L = 0xCD;
			IEN0 |= 0x20;	 //启动定时器2
			TR2 = 0x01;
      ET2 = 1;         //T2定时器中断使能控制位
			EA  = 1;
}

/*****************************************************************************
 函 数 名  : void T0_Init(void)
 功能描述  : 定时器0初始化	定时间隔1ms
 输入参数  :	 
 输出参数  : 
 修改历史  :
  1.日    期   : 
    作    者   : 
    修改内容   : 创建
*****************************************************************************/
void T0_Init(void)
{
    TMOD|=0x01;
    TH0=T1MS>>8;        //1ms定时器
    TL0=T1MS;
    ET0=1;              //开启定时器0中断
    EA=1;               //开总中断
    TR0=1;              //开启定时器
}


/*****************************************************************************
 函 数 名  : void T1_Init(void)
 功能描述  : 定时器1初始化	定时间隔1ms
 输入参数  :	 
 输出参数  : 
 修改历史  :
  1.日    期   : 
    作    者   : 
    修改内容   : 创建
*****************************************************************************/
void T1_Init(void)
{
    TMOD|=0x01;
    TH1=T1MS>>8;        //1ms定时器
    TL1=T1MS;
    ET1=1;              //开启定时器0中断
    EA=1;               //开总中断
    TR1=1;              //开启定时器
}



//系统初始化
void CPU_Inite()
{
	sys_cpu_init();//核心寄存器初始化
	sys_timer2_init();//定时器2初始化
   T0_Init();
	 T1_Init();
	
	
	
	
	Write_Dgus(0x2003,50);Write_Dgus(0x2004,50);Write_Dgus(0x1006,150);
}
//==============================================================================
void Page_Change_Handler(u16 n)
{
	Write_Dgus(0x0085,n);
	Write_Dgus(0x0084,0X5A01);
	  
}
///*****************************************************************************
// 函 数 名  : void read_dgus_vp(u32 addr,u8* buf,u16 len)
// 功能描述  : 读dgus地址的值
// 输入参数  :	 addr：dgus地址值  len：读数据长度
// 输出参数  : buf：数据保存缓存区
// 修改历史  :
//  1.日    期   : 2019年4月2日
//    作    者   : chengjing
//    修改内容   : 创建
//*****************************************************************************/
//void read_dgus_vp(u32 addr,u8* buf,u16 len)
//{
//    u32 OS_addr=0,OS_addr_offset=0;
//    u16 OS_len=0,OS_len_offset=0;
//    EA=0;
//    OS_addr=addr/2;
//    OS_addr_offset=addr%2;
//    ADR_H=(u8)(OS_addr>>16)&0xFF;
//    ADR_M=(u8)(OS_addr>>8)&0xFF;
//    ADR_L=(u8)OS_addr&0xFF;
//    ADR_INC=1;                 //DGUS  OS存储器地址在读写后自动加1
//    RAMMODE=0xAF;               //启动读模式
//    if(OS_addr_offset==1)       //首地址有偏移，修正
//    {
//        while(APP_ACK==0);      //等待
//        APP_EN=1;
//        while(APP_EN==1); 
//        *buf++=DATA1;
//        *buf++=DATA0;              
//        len--;
//        OS_addr_offset=0;
//    }
//    OS_len=len/2;
//    OS_len_offset=len%2;
//    if(OS_len_offset==1)
//    {
//         OS_len++;
//    }
//    while(OS_len--)
//    {
//        if((OS_len_offset==1)&&(OS_len==0))
//        {          
//            while(APP_ACK==0);
//            APP_EN=1;
//            while(APP_EN==1);       //读写执行结束
//            *buf++=DATA3;
//            *buf++=DATA2;           
//            break;    
//        } 
//        while(APP_ACK==0);
//        APP_EN=1;
//        while(APP_EN==1);       //读写执行结束 
//        *buf++=DATA3;
//        *buf++=DATA2;
//        *buf++=DATA1;
//        *buf++=DATA0;                          
//    }   
//    RAMMODE=0x00;           //读写完成后RAMMODE必须置零
//    EA=1;  
//	return;	
//}
/////********************************************************/


//void write_dgus_vp(u32 addr,u8* buf,u16 len)
//{
//    u32 OS_addr=0,OS_addr_offset=0;
//    u16 OS_len=0,OS_len_offset=0;
//    EA=0;
//    OS_addr=addr/2;
//    OS_addr_offset=addr%2; 
//    ADR_H=(u8)(OS_addr>>16)&0xFF;
//    ADR_M=(u8)(OS_addr>>8)&0xFF;
//    ADR_L=(u8)OS_addr&0xFF;
//    ADR_INC=0x01;                 //DGUS  OS存储器地址在读写后自动加1
//    RAMMODE=0x8F;               //启动写模式 
//    if(OS_addr_offset==1)
//    {
//        ADR_INC=0;
//        RAMMODE=0xAF;
//        while(APP_ACK==0);
//        APP_EN=1;
//        while(APP_EN==1);       //读写执行结束
//        ADR_INC=0x01; 
//        RAMMODE=0x8F;
//        while(APP_ACK==0);      
//        DATA1=*buf++;
//        DATA0=*buf++;
//        APP_EN=1;
//        while(APP_EN==1);       //读写执行结束
//        len--;
//        OS_addr_offset=0;
//    }
//    OS_len=len/2;
//    OS_len_offset=len%2; 
//    if(OS_len_offset==1)
//    {
//         OS_len++;
//    } 
//    while(OS_len--)
//    {
//        if((OS_len_offset==1)&&(OS_len==0))
//        {
//            ADR_INC=0;
//            RAMMODE=0xAF;
//            while(APP_ACK==0);
//            APP_EN=1;                //增加一个读过程，以免写单字时会将另一个单字写0
//            while(APP_EN==1);       //读写执行结束
//            ADR_INC=0x01;
//            RAMMODE=0x8F;
//            while(APP_ACK==0);           
//            DATA3=*buf++;
//            DATA2=*buf++;
//            APP_EN=1;
//            while(APP_EN==1);       //读写执行结束
//            break;
//        }
//        while(APP_ACK==0);        
//        DATA3=*buf++;
//        DATA2=*buf++;
//        DATA1=*buf++;
//        DATA0=*buf++;
//        APP_EN=1;
//        while(APP_EN==1);       //读写执行结束
//    } 
//    RAMMODE=0x00;       //读写完成后RAMMODE必须置零
//    EA=1;  
//	return;
//}

////软件大致延时,单位ms
////如果修改了优化等级,那么此函数内部的参数需要重新调试
//void sys_delay_about_ms(u16 ms)
//{
//	u16 i,j;
//	for(i=0;i<ms;i++)
//			for(j=0;j<3000;j++);    
//}


////软件大致延时,单位us
////如果修改了优化等级,那么此函数内部的参数需要重新调试
//void sys_delay_about_us(u8 us)
//{
//	u8 i,j;
//	for(i=0;i<us;i++)
//			for(j=0;j<5;j++);    
//}





//===============================================
u16 Read_Dgus(u16 Dgus_Addr)
{	   	   	u16 R_Dgus=0;
			EA = 0;
			ADR_H = 0x00;
			ADR_M = (u8)((Dgus_Addr/2)>>8);
			ADR_L = (u8)(Dgus_Addr/2);
			RAMMODE = 0xAF;		//读操作
			while(!APP_ACK);
			APP_EN = 1;
			while(APP_EN);
			if(Dgus_Addr%2)	R_Dgus = (DATA1<<8)+DATA0; 
			else   R_Dgus=(DATA3<<8)+DATA2;
			RAMMODE = 0;
			EA = 1;
			return   R_Dgus;		
}
/****************************************************************************/
void Write_Dgus(u16 Dgus_Addr,u16 Val)
{           EA = 0;
			ADR_H = 0x00;
			ADR_M = (u8)((Dgus_Addr/2)>>8);
			ADR_L = (u8)(Dgus_Addr/2);
			RAMMODE = 0xAF;		//读操作
			while(!APP_ACK);
			APP_EN = 1;
			while(APP_EN);
			RAMMODE = 0;
		   	ADR_M = (u8)((Dgus_Addr/2)>>8);
			ADR_L = (u8)(Dgus_Addr/2);
			RAMMODE = 0x8F;		//写操作
		    while(!APP_ACK);
			if(Dgus_Addr%2){DATA1=(u8)(Val>>8); DATA0=(u8)(Val); }
			else { DATA3 =(u8)(Val>>8);	DATA2 =(u8)(Val);}
		    APP_EN = 1;
			while(APP_EN);
			RAMMODE = 0;
			EA = 1;
}
/*********************************/
/************************************************************//*
D7：操作模式0x5A=读0xA5=写，CPU操作完清零。
D6:4：片内NorFlash数据库首地址，必须是偶数，0x000000-0x02:7FFE，160KWords。
D3:2：数据变量空间首地址，必须是偶数。
D1:0：读写字长度，必须是偶数。
参数1  选择NorFlash 位置 00-02
参数2  NorFlash 地址   0x000000-0x02:7FFE，160KWords。
参数3  DGUS地址        0x10000-xffff
参数4  长度             必须是偶数。
**************************************************************/
// void Write_flash(u8 place ,u16 falsh_adder,u8 len,u16 vla_adder)
// {
//   Write_Dgus(0x000B,len);
//	 Write_Dgus(0x000A,vla_adder);
//   Write_Dgus(0x0009,falsh_adder);
//	 Write_Dgus(0x0008,((0XA5<<8)+place));
//   while(Read_Dgus(0X0008)&0xFF00);
// }
// 
// //=============================================
// void Read_flash(u8 place ,u16 falsh_adder,u8 len,u16 vla_adder)
// {
//   Write_Dgus(0x000B,len);
//	 Write_Dgus(0x000A,vla_adder);
//   Write_Dgus(0x0009,falsh_adder);
//	 Write_Dgus(0x0008,((0X5A<<8)+place));
//	 while(Read_Dgus(0X0008)&0xFF00);
// }


