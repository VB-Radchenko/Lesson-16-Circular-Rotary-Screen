
#include "sys.h"

        
#define        MODBUS_SLAVE_ADDRESS				     0x01    //设备端地址
#define        MODBUS_READ_COMMAND_03H         0X03    //command modbus
#define        MODBUS_WRITE_COMMAND_06H        0x06     //command

u8  xdata MODBUS_READ_REG_LEN=0;
u8  xdata Modbus_Buffer[255];
u8 xdata  Modbus_Buffer_03H[255];    //03H读回来的数据保存

bit Flog=0;    
	   
/*****************************************************************************
 函 数 名  : Modbus_Read_Register(u16 addr,u16 num)
 功能描述  : 
			0x03:	读取vp变量空间
			
 输入参数  :	
 输出参数  : 
 修改历史  :
  1.日    期   : 
    作    者   : 
    修改内容   : 创建
*****************************************************************************/

void Modbus_Read_Register_03H(u16 addr,u16 num)
{
	u16 crc_data;
	u8 len,i;
	u8 modbus_tx_buf[20];
	
	len = 0;
	modbus_tx_buf[len++] = MODBUS_SLAVE_ADDRESS;
	modbus_tx_buf[len++] = MODBUS_READ_COMMAND_03H; //command
	modbus_tx_buf[len++] = (addr >> 8) & 0xFF;; //register
	modbus_tx_buf[len++] = addr & 0xFF; 
	modbus_tx_buf[len++] = (num >> 8) & 0xFF; //register number
	modbus_tx_buf[len++] = num & 0xFF; 

	crc_data = CRC16_CACU(modbus_tx_buf,len);
	modbus_tx_buf[len++] = (crc_data >> 8) & 0xFF; 
	modbus_tx_buf[len++] = crc_data & 0xFF; 
	TR5=1;
	for(i=0;i<len;i++)
	OneSendData5(	modbus_tx_buf[i]);
	while(Busy5);TR5=0;
	//UART5_SendStr(modbus_tx_buf,len);
} 

/*****************************************************************************
 函 数 名  : Modbus_Write_Register(u16 reg,u16 value)
 功能描述  : 
			0x06:	写寄存器变量空间
			
 输入参数  :	
 输出参数  : 
 修改历史  :
  1.日    期   : 
    作    者   : 
    修改内容   : 创建
*****************************************************************************/
void Modbus_Write_Register_06H(u16 reg,u16 value)
{
	u16 crc_data;
	u8 len,i;
	u8 modbus_tx_buf[20];

	len = 0;
	modbus_tx_buf[len++] = MODBUS_SLAVE_ADDRESS;
	modbus_tx_buf[len++] = MODBUS_WRITE_COMMAND_06H; //command
	modbus_tx_buf[len++] = (reg >> 8) & 0xFF;; //register
	modbus_tx_buf[len++] = reg & 0xFF; 
	modbus_tx_buf[len++] = (value >> 8) & 0xFF; //register value
	modbus_tx_buf[len++] = value & 0xFF; 
	crc_data = CRC16_CACU(modbus_tx_buf,len);
	modbus_tx_buf[len++] = (crc_data >> 8) & 0xFF; 
	modbus_tx_buf[len++] = crc_data & 0xFF; 
	TR5=1;
	for(i=0;i<len;i++)
	OneSendData5(	modbus_tx_buf[i]);
	while(Busy5);TR5=0;
	//UART5_SendStr(modbus_tx_buf,len);
} 

/************************************************************************/
/*****************************************************************************
 函 数 名  : Modbus_Write_Register_10H(u16 reg,u16 Dgus_adder,u8 ModbusLen)
 功能描述  : 
			0x10:	多个寄存器变量空间
			
 输入参数  :	
 输出参数  : 
 修改历史  :
  1.日    期   : 
    作    者   : 
    修改内容   : 创建
*****************************************************************************/
//modbus 10H 发送  01 10 1000 0005 0A 0001 0002 0003 0004 0005 CRCH CRCL
void Modbus_Write_Register_10H(u16 reg,u16 Dgus_adder,u8 ModbusLen)
{
	u16 crc_data=0,n=0;
	u8 len,i;
	u8 modbus_tx_buf[100];
	len = 0;
	crc_data=0;
	n=0;
	modbus_tx_buf[len++] = MODBUS_SLAVE_ADDRESS;
	modbus_tx_buf[len++] = 0x10; //command
	modbus_tx_buf[len++] = (reg >> 8) & 0xFF;; //register 
	modbus_tx_buf[len++] = reg & 0xFF; 
	modbus_tx_buf[len++] = 0; //
	modbus_tx_buf[len++] = ModbusLen; 
	modbus_tx_buf[len++] = ModbusLen*2;
	for(i=0;i<ModbusLen;i++)
	{ 
	 n=Read_Dgus(Dgus_adder+i);
	 modbus_tx_buf[len++]=(u8)(n>>8);
	 modbus_tx_buf[len++]=(u8)(n);
	}
	crc_data = CRC16_CACU(modbus_tx_buf,len);
	modbus_tx_buf[len++] = (crc_data >> 8) & 0xFF; 
	modbus_tx_buf[len++] = crc_data & 0xFF; 
	TR5=1;
	for(i=0;i<len;i++)
					 {
					 
					 OneSendData5(modbus_tx_buf[i]);
					 while(Busy5);
					 TR5=0;
					 }
} 



///**************************************************************************/

/******************************************************************************
 /*****************************************************************************
 函 数 名  :modbus_process_command(u8 *pstr,u16 strlen)
 功能描述  : 主机
			
 输入参数  :	
 输出参数  : 
 修改历史  :
  1.日    期   : 
    作    者   : 
    修改内容   : 创建
*****************************************************************************/   
void modbus_process_command()
{
	
	
	u16  modbus_crc;
	u8  MODBUS_LEN;
	//u8  CH=0,CL=0;
  u8  N=0,i=0;
	MODBUS_LEN = 0;
  Flog=1;
			
			while(Flog)
			{		if((R_u5[N]==0X01))   
					{	     if(R_u5[N+1]==0x03)     
						     {	   
												MODBUS_LEN=R_u5[N+2]+3+2;
											 for(i=0;i<MODBUS_LEN;i++)
												{
												Modbus_Buffer_03H[i]=	R_u5[N+i];
												}
												MODBUS_LEN-=2;
												modbus_crc=CRC16_CACU(Modbus_Buffer_03H,MODBUS_LEN);
												MODBUS_LEN+=2;
												if(modbus_crc==((Modbus_Buffer_03H[MODBUS_LEN-2]<<8)+Modbus_Buffer_03H[MODBUS_LEN-1]))
												{
															MODBUS_LEN=R_u5[N+2]/2;
															for(i=0;i<MODBUS_LEN;i++)
															{
																 Write_Dgus(0x6000+i,((Modbus_Buffer_03H[3+2*i]<<8)+Modbus_Buffer_03H[4+2*i]));
															
															}
												}
						      }		
						     Flog=0;
					}
					else
					{	     if(N<(R_CN5-4)) N++;
						     else            Flog=0; 
					}
			}

}
/******************************************************************************
modbus 发送和接收任务处理程序，实现：
1. 监控串口接收，当判断接收结束后，调用处理函数，
2. 监控UI的触发命令，当有检测到发送命令时，发送modbus写命令
3. 每隔1秒钟触发一次查询modbus寄存器状态的命令
******************************************************************************/
void Modbus_Process_Task()
{
	u16 xdata TP_keycode=Read_Dgus(0x0F00);

	if(((u8)(TP_keycode>>8))==0x5A)
	 {
		 
		  Write_Dgus(0X0F00,0);
			Modbus_Write_Register_10H(0X0000,0X1000,10);
	 }
	 else
	 {
	  if(fang1)  //一秒钟执行一次，
			{
					fang1=0;
					Modbus_Read_Register_03H(0x0D42,7); //从4101H地址开始读取30个寄存器的内容，更新状态信息

			
				  //sys_delay_ms(100);
				  //Modbus_Write_Register_10H(0X0001,0X1000,5);
				  //sys_delay_ms(100);
					//Modbus_Write_Register_06H(0x1000,50);
				
				
			}
	 
	 }
}
/*****************************************************************************
 函 数 名  : void Modbus_Salve_Handler(void)
 功能描述  : modbus salve处理函数
			0x03:	读取vp变量空间
			0x06 	写单个vp变量
			0x10	写vp变量空间
 输入参数  :	
 输出参数  : 
 修改历史  :
  1.日    期   : 
    作    者   : 
    修改内容   : 创建
*****************************************************************************/

void Modbus_Salve_Handler(void)
{
	u8 cmd;u8 j=0,i=0;
	u16 modbus_addr,modbus_len,modbus_data;
	u8 len;
	u16	modbus_crc;
  u8  num=0;
	
	 do
	 {
		
		if(R_u5[num]==MODBUS_SLAVE_ADDRESS)
		{
		
			cmd=R_u5[num+1];
      
			
			switch(cmd)
			{
				case	0x03:		//0x03 读取vp变量空间
					for(i=0;i<8;i++)
				  {
					Modbus_Buffer[i]=	R_u5[num+i];
					}
				  modbus_crc=CRC16_CACU(Modbus_Buffer,6);
				 if(modbus_crc==((Modbus_Buffer[6]<<8)+Modbus_Buffer[7]))
				 {
					modbus_addr=(R_u5[num+2]<<8)|R_u5[num+3];
					modbus_len=(R_u5[num+4]<<8)|R_u5[num+5];
					len=modbus_len*2;
				  //num=0;
					Modbus_Buffer[0]=R_u5[num+0];
					Modbus_Buffer[1]=0x03;
					Modbus_Buffer[2]=len;
					read_dgus_vp((u32)modbus_addr,(u8*)&Modbus_Buffer[3],modbus_len);
					len+=3;
					modbus_crc=CRC16_CACU(Modbus_Buffer,len);
					Modbus_Buffer[len]=(u8)(modbus_crc>>8);
					Modbus_Buffer[len+1]=(u8)(modbus_crc);
					len+=2;
				
					 for(i=0;i<len;i++)
					 {
					  TR5=1;
					 OneSendData5(Modbus_Buffer[i]);
					 while(Busy5);
					 TR5=0;
					 }
					//UART5_SendStr(Modbus_Buffer,len);
					  
				 }
				 //Modbus_RX_Reset();
					break;
				case	0x06:		//0x06 	写单个vp变量
				for(i=0;i<8;i++)
				  {
					Modbus_Buffer[i]=	R_u5[num+i];
						
					}
				  modbus_crc=CRC16_CACU(Modbus_Buffer,6);
				 if(modbus_crc==((Modbus_Buffer[6]<<8)+Modbus_Buffer[7]))
				 {
					modbus_addr=(R_u5[num+2]<<8)|R_u5[num+3];
					modbus_data=(R_u5[num+4]<<8)|R_u5[num+5];
				  
				  // Write_Dgus(((R_u5[num+2]<<8)+R_u5[num+3]),((R_u5[num+4]<<8)+R_u5[num+5]));
					write_dgus_vp((u32)modbus_addr,(u8*)&modbus_data,1);
				  //num=0;
					Modbus_Buffer[0]=R_u5[num];
					Modbus_Buffer[1]=R_u5[num+1];
					Modbus_Buffer[2]=R_u5[num+2];
					Modbus_Buffer[3]=R_u5[num+3];
					Modbus_Buffer[4]=R_u5[num+4];
					Modbus_Buffer[5]=R_u5[num+5];
					len=6;
					modbus_crc=CRC16_CACU(Modbus_Buffer,len);
					Modbus_Buffer[len]=(u8)(modbus_crc>>8);
					Modbus_Buffer[len+1]=(u8)(modbus_crc);
					len+=2;
					 for(i=0;i<len;i++)
					 {
						 TR5=1;
						 OneSendData5(Modbus_Buffer[i]);
						 while(Busy5);
						 TR5=0;
					 }
					//UART5_SendStr(Modbus_Buffer,len);
				  //Modbus_RX_Reset();
			   }
					break;
				case	0x10:		//0x10	写vp变量空间
					  

					modbus_len=(R_u5[num+4]<<8)|R_u5[num+5];
				  len=(modbus_len*2)+7+2;//加前面len 后面CRC
					  for(i=0;i<len;i++)
				  {
					  Modbus_Buffer[i]=	R_u5[num+i];
						//OneSendData5(Modbus_Buffer[i]);
					}
					
					  len-=2;
						//OneSendData5(len);
					
					modbus_crc=CRC16_CACU(Modbus_Buffer,len);
				  //CH=(u8)(modbus_crc>>8);
					//CL=(u8)(modbus_crc);
					//OneSendData5(CH);
					//OneSendData5(CL);
					len+=2;
					//OneSendData5(Modbus_Buffer[len-2]);
					//OneSendData5(Modbus_Buffer[len-1]);
					
				 if(modbus_crc==((Modbus_Buffer[len-2]<<8)+Modbus_Buffer[len-1]))
					{
				  for(j=0;j<modbus_len;j++)
				  Write_Dgus(((R_u5[num+2]<<8)+R_u5[num+3]+j),((R_u5[num+7+j*2]<<8)+R_u5[num+8+j*2]));
			
					Modbus_Buffer[0]=R_u5[num];
					Modbus_Buffer[1]=0x10;
					Modbus_Buffer[2]=R_u5[num+2];
					Modbus_Buffer[3]=R_u5[num+3];
					Modbus_Buffer[4]=R_u5[num+4];
					Modbus_Buffer[5]=R_u5[num+5];
					len=6;
					modbus_crc=CRC16_CACU(Modbus_Buffer,len);
					Modbus_Buffer[len]=(u8)(modbus_crc>>8);
					Modbus_Buffer[len+1]=(u8)(modbus_crc);
					len+=2;
					for(i=0;i<len;i++)
					 {
					 TR5=1;
					 OneSendData5(Modbus_Buffer[i]);
					 while(Busy5);
					 TR5=0;
					 }
					//UART5_SendStr(Modbus_Buffer,len);
				  //Modbus_RX_Reset();
					 }
					break;
				   
				default:
				
					break;
			 }
			
		}
	
	  num++;
	}while (num<(R_CN5 -7));
	//Modbus_RX_Reset();
 
}
/*****************************************************************************
 函 数 名  :void Modbus_Salve_Command()
 功能描述  : modbus salve处理函数
			
 输入参数  :	
 输出参数  : 
 修改历史  :
  1.日    期   : 
    作    者   : 
    修改内容   : 创建
*****************************************************************************/


/*****************************************************************************
 函 数 名  :Modbus_master_Salve_Set()
 功能描述  : 主机和从机选择函数
			
 输入参数  :	
 输出参数  : 
 修改历史  :
  1.日    期   : 
    作    者   : 
    修改内容   : 创建
*****************************************************************************/



//==========================================
void Decimal_conversion()
{
	   float  xdata   tem[8];
	   float  xdata   sdfg1[8];
	   u8     xdata   i=0;
				 for(i=0;i<8;i++)
         sdfg1[i]=Read_Dgus(0x6000+i);
				 for(i=0;i<8;i++)
	       tem[i]=(float)(sdfg1[i]*100)/32767;
				 write_dgus_vp(0X7000,(u8*)tem,16);
		

}

