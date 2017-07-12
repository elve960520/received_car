
#include "nRF24L01.h" 
 	
uchar TX_ADDRESS[TX_ADR_WIDTH]={0xB1,0x51,0xF3,0x08,0x70}; //发送地址		//更改收发地址
uchar RX_ADDRESS[RX_ADR_WIDTH]={0xB2,0x52,0xF4,0x09,0x71}; //接收地址

uchar   rate = 0x0F;    //2Mbs, 0dB  低噪声增益开启


sbit    LED1      =P3^2;				//信号灯接口  根据自己焊接决定
sbit    LED2      =P3^3;				
sbit	LED3	  =P3^4;
sbit	L1		  =P2^0;			    //电机控制接口
sbit	L2		  =P2^1;
sbit	R1		  =P2^2;
sbit	R2		  =P2^3;
sbit    action2   =P2^6;
sbit    action    =P2^7;				//风扇控制接口
uchar rece_buf[32];							//接收寄存器
							//a0=37:最大角度，脉宽1.899ms

void delay(uint t)
{
	uint a,b;
	for(a=0;a<t;a++)
		for (b=0;b<255;b++);
}

void main()
{	int i;
	uchar n=0,m=0;
	LED1=1;
	LED2=1;
	LED3=1;
	while(NRF24L01_Check());                    //等待检测到NRF24L01，程序才会向下执行
	NRF24L01_RT_Init(TX_PLOAD_WIDTH,TX_ADDRESS,TX_ADR_WIDTH,RX_ADDRESS,RX_ADR_WIDTH,rate);			

	for(i=0;i<12;i++)
		{
			delay(100);		//自检成功
			LED1=~LED1;
		}
	LED1=1;
				

	while(1)
	{
 
		if(NRF_IRQ==0)	 	// 如果无线模块接收到数据
		{		
			
			if(NRF24L01_RxPacket(rece_buf)==0)
			{	
				if(rece_buf[1]=='1')		   	//前进--第1位以后是收到的命令数据，rece_buf[0]是数据位数长度
				{
					LED1=1;
					LED2=0;
					LED3=1;
					L1	=0;
					L2	=1;
					R1	=0;
					R2	=1;
				}
				else if(rece_buf[1]=='b')	 		//后退--第1位以后是收到的命令数据，rece_buf[0]是数据位数长度
				{
					LED1=0;
					LED2=1;
					LED3=1;
					L1	=1;
					L2	=0;
					R1	=1;
					R2	=0;
				}
				else if(rece_buf[1]=='3')	 		//左--第1位以后是收到的命令数据，rece_buf[0]是数据位数长度
				{
					LED1=1;
					LED2=1;
					LED3=0;
					L1	=1;
					L2	=0;
					R1	=0;
					R2	=1;
				}	
				else if(rece_buf[1]=='4')	 		//右--第1位以后是收到的命令数据，rece_buf[0]是数据位数长度
				{
					LED1=0;
					LED2=1;
					LED3=0;
					L1  =0;
					L2	=1;
					R1	=1;
					R2	=0;
				}

				else if(rece_buf[1]=='5')			//左前
				{
					LED1=1;
					LED2=0;
					LED3=0;
					L1	=0;
					L2	=0;
					R1	=0;
					R2	=1;
				}
				else if(rece_buf[1]=='6')
				{									//右前
					LED1=0;
					LED2=0;
					LED3=0;
					L1	=0;
					L2	=1;
					R1	=0;
					R2	=0;
				}
				else if(rece_buf[1]=='7')
				{									//左后
					LED1=1;
					LED2=0;
					LED3=0;
					L1	=0;
					L2	=0;
					R1	=1;
					R2	=0;
				}
				else if(rece_buf[1]=='8')
				{									//右后
					LED1=0;
					LED2=0;
					LED3=0;
					L1	=1;
					L2	=0;
					R1	=0;
					R2	=0;
				}
				if(rece_buf[1]=='9')
				{									//风扇
					LED1=0;
					LED2=1;
					LED3=1;
					L1	=0;
					L2	=0;
					R1	=0;
					R2	=0;
					action2=1;
					action=1;
				}
				delay(5);
			}
		}
		else 	
		{										   //停止
					LED1=0;
					LED2=0;
					LED3=1;
					L1	=0;
					L2	=0;
					R1	=0;
					R2	=0;
					action2=0;
					action=0;
		}

//**************************************		
	}
}



