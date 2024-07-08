#include "sys.h"



void main(void) 
{
	CPU_Inite();
	while(1)
	{
		Clock();
		if((1==R_OD2)&&(T_O2==0)) 
			{
				ReceiveDate2();
				R_OD2=0;
				R_CN2=0;
			}//8283
		Encoder_DGUS();
	}
}