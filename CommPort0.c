#include "define.h"

#ifndef __COM0CFILE__
#define __COM0CFILE__

xdata char COM0_rsBuf[COM0_RSBUF_LEN];	//����COM0���ջ�����
xdata int COM0_RcvCount = 0;				//����COM0���յ����ַ���
xdata int COM0_SndCount = 0;				//����COM0Ҫ���͵��ַ���
xdata int COM0_SndPosi = 0;				//����COM0Ҫ���͵ĵ�ǰ�ַ�����
bit COM0_RcvCompleted = 0;			//����COM0��������ɱ�־
unsigned char COM0_CET=0;			//���������

//data char COM0_PARITY=0;
#ifdef _MENU
#define COM0_PARITY SetValue.OddEven
#define ModSlv_SA SetValue.ModbusSA
#else
#define COM0_PARITY 1
#define ModSlv_SA 02
#endif

#ifdef _MODBUSSLAVE_COM0
#include "modslv.h"
#endif

#ifdef _BEEP
#include "beep.h"
#endif

#ifdef _MENU
#include "menu3.h"
#endif

#define SYSCLK 11059200
#define BAUDBASE SYSCLK / 384

code unsigned char BAUD[]= {-(BAUDBASE/1200),-(BAUDBASE/2400),-(BAUDBASE/4800),-(BAUDBASE/9600)};

void Init_COM0()
{
	COM0_RcvCount = 0;

	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD&=0x0F;
	TMOD|=0x20;
	
	//PCON |= 0x80;
	TH1=BAUD[SetValue.BaudRate]; //������9600 11.0592M 0xFD
	TL1=BAUD[SetValue.BaudRate]; //18.432M 0xFB

	SM0 = COM0_PARITY;
	SM1=1;

	SM2=0;
	REN=1;
	ES=1;
	TR1=1;
}

void Reinit_COM0()
{
	//EA = 0;

	TH1=BAUD[SetValue.BaudRate % 4]; //������9600 11.0592M 0xFD
	TL1=BAUD[SetValue.BaudRate % 4]; //18.432M 0xFB
	SM0 = COM0_PARITY;

	//EA = 1;
}

void COM0_Start_Sent()
{
	COM0_RcvCount = 0;
	ES=1;
	if (COM0_SndCount>0)
	{
		if (COM0_PARITY==1)
		{		//��
			ACC = COM0_rsBuf[0];
			TB8=~P;
			SBUF=ACC;
		}
		else if (COM0_PARITY==2)
		{	//ż
			ACC = COM0_rsBuf[0];
			TB8=P;
			SBUF=ACC;
		}
		else
		{
			SBUF=COM0_rsBuf[0];
		}
		COM0_SndPosi=1;
	}
}


void COM0_Rcv()
{
	if (
#ifdef _MODBUSMASTER_COM0	
		(COM0_RcvCount==0 && COM0_CET>=COM0_TIMEOUT) || 
#endif		
		(COM0_RcvCount>0 && COM0_RcvCount<COM0_RSBUF_LEN && COM0_CET>=COM0_RCVTIMEOUT))
	{
		COM0_RcvCompleted = 1;
		//P2=COM0_RcvCount;
		COM0_CET = 0;
		//P0=COM0_CET;
		//REN = 0;	//������ɣ����ֹ�����ж�
	}
	else if (COM0_RcvCount>=COM0_RSBUF_LEN)
	{
		COM0_RcvCount=0;
		COM0_CET=0;
	}
	else
	{
		COM0_CET++;
	}
}

void Serial0() interrupt 4
{	//����ͨѶ�ж� 
	xdata unsigned char ch=0;

	COM0_CET=0;

	if (TI)
	{		//�����ж� 
		TI=0;

		if(COM0_SndPosi<COM0_SndCount)
		{	//�����ַ� 
			ch = COM0_rsBuf[COM0_SndPosi++];
			
			if (COM0_PARITY==1)
			{		//��
				ACC = ch;
				TB8=~P;
				SBUF=ch;
			}
			else if (COM0_PARITY==2)
			{	//ż
				ACC = ch;
				TB8=P;
				SBUF=ch;
			}
			else 
			{
				SBUF=ch;
			}
		}
		else
		{		//������� 
			COM0_SndCount=0;
			REN=1;	//��������ж�
		}
	}

	if (RI)
	{		//�����ж� 
		RI=0;
		ch=SBUF;
		//Beep(1);
		if (COM0_RcvCount>0 && COM0_RcvCount<COM0_RSBUF_LEN)
		{	//����ʣ���ֽ� 
			COM0_rsBuf[COM0_RcvCount++]=ch;
		}
		else
		{
			if ((ch==
#ifdef _MODBUSMASTER_COM0
				COM0_rsBuf[0]
#else				
				ModSlv_SA
#endif				
				)||(ch==90))
			{ 	//�жϽ��յ���ַ!
				COM0_rsBuf[0]=ch;
				COM0_RcvCount=1;
			}
			else
			{
				COM0_RcvCount=0;
			}
		}
	}
}

#endif	//__COM0CFILE__
