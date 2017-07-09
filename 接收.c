
#include "nRF24L01.h" 
 	
uchar TX_ADDRESS[TX_ADR_WIDTH]={0xB1,0x51,0xF3,0x08,0x70}; //���͵�ַ		//�����շ���ַ
uchar RX_ADDRESS[RX_ADR_WIDTH]={0xB2,0x52,0xF4,0x09,0x71}; //���յ�ַ

uchar   rate = 0x0F;    //2Mbs, 0dB  ���������濪��


sbit    LED1      =P3^2;				//�źŵƽӿ�  �����Լ����Ӿ���
sbit    LED2      =P3^3;				
sbit	LED3	  =P3^4;
sbit	L1		  =P2^0;			    //������ƽӿ�
sbit	L2		  =P2^1;
sbit	R1		  =P2^2;
sbit	R2		  =P2^3;
sbit    action2   =P2^6;
sbit    action    =P2^7;				//���ȿ��ƽӿ�
uchar rece_buf[32];							//���ռĴ���
							//a0=37:���Ƕȣ�����1.899ms

uchar temp_time = 0;
sbit pwm_out = P0^0;
sbit pwm_out_part1 = P0^1;
sbit pwm_out_part2 = P0^2;
uchar set_speed = 0;

void delay(uint t)
{
	uint a,b;
	for(a=0;a<t;a++)
		for (b=0;b<255;b++);
}

void system_Init()
{
	TMOD=0x01; 
	TH0=(65535 - 10) / 256; 
	TL0=(65535 - 10) % 256;
	TR0=1; 
	ET0=1; 
	EA=1; 
}

void main()
{	int i;
	uchar n=0,m=0;
	LED1=1;
	LED2=1;
	LED3=1;
	set_speed = 0;
	system_Init();
	while(NRF24L01_Check());                    //�ȴ���⵽NRF24L01������Ż�����ִ��
	NRF24L01_RT_Init(TX_PLOAD_WIDTH,TX_ADDRESS,TX_ADR_WIDTH,RX_ADDRESS,RX_ADR_WIDTH,rate);			

	for(i=0;i<12;i++)
		{
			delay(100);		//�Լ�ɹ�
			LED1=~LED1;
		}
	LED1=1;
	pwm_out_part1 = 1;
	pwm_out_part2 = 0;

	while(1)
	{
		if(temp_time > 100)
		{
			temp_time = 0;		
		}
		if(NRF_IRQ==0)	 	// �������ģ����յ�����
		{		
			
			if(NRF24L01_RxPacket(rece_buf)==0)
			{	
				if(rece_buf[0]==1)
				{
					LED1=1;  
					LED2=0;
					LED3=1;
					set_speed = rece_buf[1];
				}
			} 
		}
		else 	
		{										   
					LED1=0;
					LED2=0;
					LED3=1;
		}
		if(temp_time <= set_speed)
		pwm_out = 1;
		else pwm_out = 0;

//**************************************		
	}
}

void timer0() interrupt 1 
{
	TH0=(65535 - 10) / 256; 
	TL0=(65535 - 10) % 256;
	temp_time++;
	
}



