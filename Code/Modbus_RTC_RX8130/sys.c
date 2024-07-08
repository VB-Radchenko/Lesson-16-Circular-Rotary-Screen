#include "sys.h"


//���ļĴ�����ʼ��
void sys_cpu_init()
{
			EA = 0;
			RS0 = 0;	      //Psw.3
			RS1 = 0;		  //Psw.4
			CKCON = 0x00;	  //CPU����=0, 1Tģʽ
			T2CON = 0x70;	  //
			DPC = 0x00;		  //
	    P3MDOUT           = 0x0C;					   //RTCʱ�����
      P3                = 0x00;  
			MUX_SEL|=0x40;    //UART2����,UART3��������WDT�ر�P0.4��P0.5
			P0MDOUT|=0x03;	  //p0.0 p0.1 ǿ�� ����485�������
			RAMMODE =0x00;	  //
			PORTDRV =0x01;    //����ǿ��+/-8mA
			IEN0 = 0x00;      //�ر������ж�
			IEN1 = 0x00;
			IEN2 = 0x00;
			IP0 = 0x00;       //�ж����ȼ�Ĭ��
			IP1 = 0x00;
			WDT_OFF();        //�رտ��Ź�
	
	//========================================����2��ʼ��*/
			P0MDOUT &= 0xCF;
			P0MDOUT |= 0x10;//���ö�Ӧ��IO���������
			ADCON = 0x80;//ѡ��SREL0H:L��Ϊ�����ʷ�����
			SCON2 = 0x50;//����ʹ�ܺ�ģʽ����
			PCON &= 0x7F;//SMOD=0 64      SMOD=1    32
			SREL2H = (u8)(UART2_BPR>>8);  
			SREL2L = (u8)(UART2_BPR);  
			IEN0 = 0X10;//�ж�ʹ��
			TR2=0;
	//========================================����4��ʼ��*/
//			BODE4_DIV_H =(u8)(UART4_BPR>>8); 
//			BODE4_DIV_L =(u8)(UART4_BPR); 
//		    SCON4T= 0x80;     //����ʹ�ܺ�ģʽ����
//		    SCON4R= 0x80;     //����ʹ�ܺ�ģʽ���� 
//			  ES4R = 1;         //�жϽ���ʹ��
//		    ES4T = 1;         //�жϷ���ʹ��
//			  TR4=0;
//========================================
//========================================����5��ʼ��*/
//			BODE5_DIV_H =(u8)(UART4_BPR>>8);
//			BODE5_DIV_L =(u8)(UART4_BPR);
//		    SCON5T= 0x80;     //����ʹ�ܺ�ģʽ����
//		    SCON5R= 0x80;     //����ʹ�ܺ�ģʽ���� 
//			  ES5R = 1;         //�жϽ���ʹ��
//		    ES5T = 1;         //�жϷ���ʹ��
//			  TR5=0;
}


//��ʱ��2��ʼ��,��ʱ���Ϊ1ms
void sys_timer2_init()
{
	    TH2 = 0x00;
			TL2 = 0x00;
			T2CON = 0x70;
			TRL2H = 0xBC;	 //1ms�Ķ�ʱ��
			TRL2L = 0xCD;
			IEN0 |= 0x20;	 //������ʱ��2
			TR2 = 0x01;
      ET2 = 1;         //T2��ʱ���ж�ʹ�ܿ���λ
			EA  = 1;
}

/*****************************************************************************
 �� �� ��  : void T0_Init(void)
 ��������  : ��ʱ��0��ʼ��	��ʱ���1ms
 �������  :	 
 �������  : 
 �޸���ʷ  :
  1.��    ��   : 
    ��    ��   : 
    �޸�����   : ����
*****************************************************************************/
void T0_Init(void)
{
    TMOD|=0x01;
    TH0=T1MS>>8;        //1ms��ʱ��
    TL0=T1MS;
    ET0=1;              //������ʱ��0�ж�
    EA=1;               //�����ж�
    TR0=1;              //������ʱ��
}


/*****************************************************************************
 �� �� ��  : void T1_Init(void)
 ��������  : ��ʱ��1��ʼ��	��ʱ���1ms
 �������  :	 
 �������  : 
 �޸���ʷ  :
  1.��    ��   : 
    ��    ��   : 
    �޸�����   : ����
*****************************************************************************/
void T1_Init(void)
{
    TMOD|=0x01;
    TH1=T1MS>>8;        //1ms��ʱ��
    TL1=T1MS;
    ET1=1;              //������ʱ��0�ж�
    EA=1;               //�����ж�
    TR1=1;              //������ʱ��
}



//ϵͳ��ʼ��
void CPU_Inite()
{
	sys_cpu_init();//���ļĴ�����ʼ��
	sys_timer2_init();//��ʱ��2��ʼ��
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
// �� �� ��  : void read_dgus_vp(u32 addr,u8* buf,u16 len)
// ��������  : ��dgus��ַ��ֵ
// �������  :	 addr��dgus��ֵַ  len�������ݳ���
// �������  : buf�����ݱ��滺����
// �޸���ʷ  :
//  1.��    ��   : 2019��4��2��
//    ��    ��   : chengjing
//    �޸�����   : ����
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
//    ADR_INC=1;                 //DGUS  OS�洢����ַ�ڶ�д���Զ���1
//    RAMMODE=0xAF;               //������ģʽ
//    if(OS_addr_offset==1)       //�׵�ַ��ƫ�ƣ�����
//    {
//        while(APP_ACK==0);      //�ȴ�
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
//            while(APP_EN==1);       //��дִ�н���
//            *buf++=DATA3;
//            *buf++=DATA2;           
//            break;    
//        } 
//        while(APP_ACK==0);
//        APP_EN=1;
//        while(APP_EN==1);       //��дִ�н��� 
//        *buf++=DATA3;
//        *buf++=DATA2;
//        *buf++=DATA1;
//        *buf++=DATA0;                          
//    }   
//    RAMMODE=0x00;           //��д��ɺ�RAMMODE��������
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
//    ADR_INC=0x01;                 //DGUS  OS�洢����ַ�ڶ�д���Զ���1
//    RAMMODE=0x8F;               //����дģʽ 
//    if(OS_addr_offset==1)
//    {
//        ADR_INC=0;
//        RAMMODE=0xAF;
//        while(APP_ACK==0);
//        APP_EN=1;
//        while(APP_EN==1);       //��дִ�н���
//        ADR_INC=0x01; 
//        RAMMODE=0x8F;
//        while(APP_ACK==0);      
//        DATA1=*buf++;
//        DATA0=*buf++;
//        APP_EN=1;
//        while(APP_EN==1);       //��дִ�н���
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
//            APP_EN=1;                //����һ�������̣�����д����ʱ�Ὣ��һ������д0
//            while(APP_EN==1);       //��дִ�н���
//            ADR_INC=0x01;
//            RAMMODE=0x8F;
//            while(APP_ACK==0);           
//            DATA3=*buf++;
//            DATA2=*buf++;
//            APP_EN=1;
//            while(APP_EN==1);       //��дִ�н���
//            break;
//        }
//        while(APP_ACK==0);        
//        DATA3=*buf++;
//        DATA2=*buf++;
//        DATA1=*buf++;
//        DATA0=*buf++;
//        APP_EN=1;
//        while(APP_EN==1);       //��дִ�н���
//    } 
//    RAMMODE=0x00;       //��д��ɺ�RAMMODE��������
//    EA=1;  
//	return;
//}

////���������ʱ,��λms
////����޸����Ż��ȼ�,��ô�˺����ڲ��Ĳ�����Ҫ���µ���
//void sys_delay_about_ms(u16 ms)
//{
//	u16 i,j;
//	for(i=0;i<ms;i++)
//			for(j=0;j<3000;j++);    
//}


////���������ʱ,��λus
////����޸����Ż��ȼ�,��ô�˺����ڲ��Ĳ�����Ҫ���µ���
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
			RAMMODE = 0xAF;		//������
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
			RAMMODE = 0xAF;		//������
			while(!APP_ACK);
			APP_EN = 1;
			while(APP_EN);
			RAMMODE = 0;
		   	ADR_M = (u8)((Dgus_Addr/2)>>8);
			ADR_L = (u8)(Dgus_Addr/2);
			RAMMODE = 0x8F;		//д����
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
D7������ģʽ0x5A=��0xA5=д��CPU���������㡣
D6:4��Ƭ��NorFlash���ݿ��׵�ַ��������ż����0x000000-0x02:7FFE��160KWords��
D3:2�����ݱ����ռ��׵�ַ��������ż����
D1:0����д�ֳ��ȣ�������ż����
����1  ѡ��NorFlash λ�� 00-02
����2  NorFlash ��ַ   0x000000-0x02:7FFE��160KWords��
����3  DGUS��ַ        0x10000-xffff
����4  ����             ������ż����
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


