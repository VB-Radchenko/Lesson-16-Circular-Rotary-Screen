#ifndef __MODBUS_RTU1_H__
#define __MODBUS_RTU1_H__



#define UART_TX_BUF_MAX_LEN 				64
#define UART_RX_BUF_MAX_LEN 				64
//#define MODBUS_READ_REG_LEN 				14      //¼Ä´æÆ÷¸öÊý

 extern bit Flog;



 
 

extern u8  xdata MODBUS_READ_REG_LEN;

void Modbus_Read_Register_03H(u16 addr,u16 num);
void Modbus_Write_Register_06H(u16 reg,u16 value);
void Modbus_Write_Register_10H(u16 reg,u16 Dgus_adder,u8 ModbusLen);
void Modbus_Salve_Handler(void);
void Modbus_RX_Reset(void);
void Modbus_master_Salve_Set();
void Modbus_Salve_Command();
void modbus_process_command();
void Modbus_Process_Task();



void  Decimal_conversion();
void  OneSendData5_16(u16 D_data);
#endif