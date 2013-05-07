#include "define.h"

#ifndef __TIMERCFILE__
#define __TIMERCFILE__

#ifdef _MODBUSMASTER
#include "modmst.h"
#endif

#ifdef _MODBUSSLAVE
#include "modslv.h"
#endif

#ifdef _COM0
#include "CommPort0.h"
#endif

#ifdef _COM1
#include "CommPort1.h"
#endif

#ifdef _HSC
#include "HSC.h"
#endif

#ifdef _ADC
#include "adc.h"
#endif

#ifdef _KEY
#include "key.h"
#endif

#ifdef _BEEP
#include "beep.h"
#endif

#ifdef _MENU
#include "menu3.h"
#endif


#ifndef PCA_TIMER
void Init_Timer(){
	TMOD&=0xF0;
	TMOD|=0x01;
	TH0=0xDC;
	TL0=0x00;
	ET0=1;
	TR0=1;
}

void Timer() interrupt 1{	

	//WDT_CONTR = 0x3F;//���Ź���λ

 	TH0=0xDC;//0xFC; 
	TL0=0x00;//0x6C; 

#ifdef _COM0
	COM0_Rcv(); //COM0���ս���
#endif

#ifdef _COM1
	COM1_Rcv(); //COM1���ս���
#endif

#ifdef _ADC
	adc();
#endif

#ifdef _KEY
	keydrv();
#endif

#ifdef _BEEP
	BeepDrv();
#endif

#ifdef _MENU
	MenuDrv();
#endif

}
#endif



//ʹ��PCAΪ��ʱ��
#ifdef PCA_TIMER
/*����10mS PCA��ʱ������*/
#define MS10L=0x00;
#define MS10H=0x24;

void Init_Timer(){//����PCAΪ��ʱ����PWMʹ�ã�����PCA Module 0Ϊ��ʱ��,PCAΪPWM.

	CMOD=0x80;//0x82; //��Fso/2��Ϊʱ��Դ
	CCON=0x00;
	CL=0x00;
	CH=0x00;
	CCAP0L=0x00;            //��ʱ����ʽ����
	CCAP0H=0x6C;
	CCAPM0=0x49;

	/***********************�����NE555�ڰ�����************************
	CCAP3L=0x70;            //PWM��ʽ����0x80=50% FREQ=FSO/2/256=21.6KHz
	CCAP3H=0x70;
	CCAPM3=0x42;			//PCA3������PWM��ʽ���趨
	P2M0=0;					//P2.4ǿ�������
	P2M1=0x10;
	B20=0;					//��ʼ��Ϊ�͵�ƽ
	*******************************************************************/

	//ELVD=1;  	           //����PCA��LVD�ж�
	CR=1;	                //PCA��ʼ����
}


void PCA_Timer(void) interrupt 7{
	//static int HT=0;
	//int tmpi;

	if (CCF0){								//�����ģ��0�������ж�
		CCF0=0;								//�������жϱ�־

		//WDT_CONTR = 0x3F;					//���Ź���λ

		CCAP0L=CCAP0L+MS10L;
		CCAP0H=CCAP0H+MS10H;
		
#ifdef COM0
		COM0_Rcv(); //COM0���ս���
#endif

#ifdef COM1
		COM1_Rcv(); //COM1���ս���
#endif

#ifdef HSC
		Hsc();		//ģ�����ɼ�
#endif

#ifdef KEY
		keydrv();
#endif
		BeepDrv();

	}
}
#endif

#endif //__TIMERCFILE__
