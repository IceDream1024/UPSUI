//========================================================================
// �ļ���:  LCD_Dis.c
// ��  ��: xinqiang Zhang(С��)  (email: xinqiang@Mzdesign.com.cn)
// ��  ��: 2009/02/10
// ��  ��: MzLH04-12864 LCD��ʾģ������V1.0��
//			�������ܽӿں�����
//
// ��  ��: ��
// ��  ��:
//      2009/02/10      First version    Mz Design
//========================================================================

#include "LCD_Config.h"

#ifndef __LCD_DISCFILE__
#define __LCD_DISCFILE__

xdata unsigned char X_Witch=6;
xdata unsigned char Y_Witch=10;
xdata unsigned char X_Witch_cn=16;
xdata unsigned char Y_Witch_cn=16;
xdata unsigned char Dis_Zero=0;
xdata unsigned char* dot = "��";


char i2s_buf[15]={0};
// n ��ʾλ��
// n>0x80 ����Ϊ�޷�����
char *i2s(int d,unsigned char n)
{
	bit sign = 0;
	char i = 7,j;
	long dx = d;
	if (n & 0x80)
	{
		n -= 0x80;
		dx = (n==3)?(unsigned char)d:(unsigned int)d;
	}

	if (dx<0)
	{
		dx = -dx;
		sign = 1;
	}
	while(dx)
	{
		i2s_buf[i--] = dx % 10 + '0';
		dx = dx / 10;
	}
	if (i==7) i2s_buf[i--] = '0';
	if (sign) i2s_buf[i--] = '-';
	j = n - (7 - i);
	if (j>=0)
	{
		i2s_buf[8+j] = 0;
		while(j--)
		{
			i2s_buf[8+j] = 0x20;
		}
	}
	return &i2s_buf[i+1];				
}


//========================================================================
// ����: void LCD_Init(void)
// ����: LCD��ʼ��������Ҫ��������ɶ˿ڳ�ʼ���Լ�LCDģ��ĸ�λ
// ����: �� 
// ����: ��
// ��ע:
// �汾:
//      2009/02/10      First version    Mz Design
//========================================================================
void TimeDelay(unsigned int Timers)
{
	xdata unsigned int i;
	while(Timers)
	{
		Timers--;
		for(i=0;i<100;i++) ;
	}
}
void LCD_Init(void)
{
	//SS��SCKԤ������Ϊ�ߵ�ƽ
	SPI_SCK = 1;
	SPI_CS = 1;
	
	//��λLCDģ��
	SPI_RES = 0;
	TimeDelay(50);			
	//���ֵ͵�ƽ���2ms����
	SPI_RES = 1;
	TimeDelay(80);				    	//��ʱ���10ms����
}
//========================================================================
// ����: void SPI_SSSet(unsigned char Status)
// ����: ��SS��״̬
// ����: Status   =1���øߵ�ƽ��=0���õ͵�ƽ  
// ����: ��
// �汾:
//      2009/02/10      First version    Mz Design
//========================================================================
void SPI_SSSet(unsigned char Status)
{
	if(Status)				//�ж���Ҫ��SSΪ�ͻ��Ǹߵ�ƽ��//SS�øߵ�ƽ
		SPI_CS = 1;
	else   //SS�õ͵�ƽ
		SPI_CS = 0;
}
//========================================================================
// ����: void SPI_Send(unsigned char Data)
// ����: ͨ������SPI������һ��byte��������ģ��
// ����: Data Ҫ���͵����� 
// ����: ��
// �汾:
//      2007/07/17      First version
//		2007/07/24		V1.2 for MCS51 Keil C
//========================================================================
void SPI_Send(unsigned char Data)
{
	xdata unsigned char i=0;
	for(i=0;i<8;i++)
	{
		//SCK�õ�
		SPI_SCK = 0;
		if(Data&0x0080)
			SPI_SDA = 1;			
		else SPI_SDA = 0;//
		//SCK�����ش����������ݲ���
		SPI_SCK = 1;
		Data = Data<<1;							//��������һλ
		//nop();nop();nop();
	}
}
//========================================================================
// ����: void FontSet(unsigned char Font_NUM,unsigned char Color)
// ����: ASCII�ַ���������
// ����: Font_NUM ����ѡ��,�������������ֿ�Ϊ׼	0 6x10, 1 8x16
//		 Color  �ı���ɫ,��������ASCII�ֿ� 0 Ϊ��ɫ,1Ϊ��ɫ 
// ����: ��
// ��ע: 
// �汾:
//      2007/07/19      First version
//========================================================================
void FontSet(unsigned char Font_NUM,unsigned char Color)
{
	xdata unsigned char ucTemp=0;
	if(Font_NUM==0)
	{
		X_Witch = 6;//7;
		Y_Witch = 10;
	}
	else
	{
		X_Witch = 8;
		Y_Witch = 16;
	}
	ucTemp = (Font_NUM<<4)|Color;
	//����ASCII�ַ�������
	SPI_SSSet(0);					//SS�õ͵�ƽ			
	SPI_Send(0x81);					//����ָ��0x81
	SPI_Send(ucTemp);				//ѡ��8X16��ASCII����,�ַ�ɫΪ��ɫ
	SPI_SSSet(1);					//��ɲ�����SS�ߵ�ƽ	
}
//========================================================================
// ����: void FontMode(unsigned char Cover,unsigned char Color)
// ����: �����ַ���ʾ����ģʽ
// ����: Cover  �ַ�����ģʽ���ã�0��1
//		 Color ����ģʽΪ1ʱ�ַ���ʾʱ�ı�������ɫ 
// ����: ��
// ��ע: 
// �汾:
//      2008/11/27      First version
//========================================================================
void FontMode(unsigned char Cover,unsigned char Color)
{
	xdata unsigned char ucTemp=0;
	ucTemp = (Cover<<4)|Color;
	//����ASCII�ַ�������
	SPI_SSSet(0);					//SS�õ͵�ƽ			
	SPI_Send(0x89);					//����ָ��0x81
	SPI_Send(ucTemp);				//ѡ��8X16��ASCII����,�ַ�ɫΪ��ɫ
	SPI_SSSet(1);					//��ɲ�����SS�ߵ�ƽ	
}
//========================================================================
// ����: void FontSet_cn(unsigned char Font_NUM,unsigned char Color)
// ����: ���ֿ��ַ���������
// ����: Font_NUM ����ѡ��,�������������ֿ�Ϊ׼	0 12x12, 1 16x16
//		 Color  �ı���ɫ,�������ں��ֿ�  0 Ϊ��ɫ,1Ϊ��ɫ 
// ����: ��
// ��ע: 
// �汾:
//      2007/07/19      First version
//========================================================================
void FontSet_cn(unsigned char Font_NUM,unsigned char Color)
{
	unsigned char ucTemp=0;
	if(Font_NUM==0)
	{
		X_Witch_cn = 12;
		Y_Witch_cn = 12;
	}
	else
	{
		X_Witch_cn = 16;
		Y_Witch_cn = 16;
	}
	ucTemp = (Font_NUM<<4)|Color;
	//����ASCII�ַ�������
	SPI_SSSet(0);					//SS�õ͵�ƽ			
	SPI_Send(0x82);					//����ָ��0x81
	SPI_Send(ucTemp);				//ѡ��8X16��ASCII����,�ַ�ɫΪ��ɫ
	SPI_SSSet(1);					//��ɲ�����SS�ߵ�ƽ	
}
//========================================================================
// ����: void PutChar(unsigned char x,unsigned char y,unsigned char a) 
// ����: д��һ����׼ASCII�ַ�
// ����: x  X������     y  Y������
//		 a  Ҫ��ʾ���ַ����ֿ��е�ƫ����  
// ����: ��
// ��ע: ASCII�ַ���ֱ������ASCII�뼴��
// �汾:
//      2007/07/19      First version
//========================================================================
void PutChar(unsigned char x,unsigned char y,unsigned char a) 
{
	//��ʾASCII�ַ�
	SPI_SSSet(0);					//SS�õ͵�ƽ	
	SPI_Send(7);					//����ָ��0x07
	SPI_Send(x);					//Ҫ��ʾ�ַ������Ͻǵ�X��λ��
	SPI_Send(y);					//Ҫ��ʾ�ַ������Ͻǵ�Y��λ��
	SPI_Send(a);					//Ҫ��ʾ�ַ�ASCII�ַ���ASCII��ֵ
	SPI_SSSet(1);					//��ɲ�����SS�ߵ�ƽ
}
//========================================================================
// ����: void PutString(int x,int y,char *p)
// ����: ��x��yΪ��ʼ���괦д��һ����׼ASCII�ַ�
// ����: x  X������     y  Y������
//		 p  Ҫ��ʾ���ַ���  
// ����: ��
// ��ע: ���������Դ���ASCII�ַ�����ʾ
// �汾:
//      2007/07/19      First version
//========================================================================
void PutString(unsigned char x,unsigned char y,unsigned char *p)
{
	while(*p!=0)
	{
		PutChar(x,y,*p);
		x += X_Witch;
		if((x + X_Witch) > Dis_X_MAX)
		{
			x = Dis_Zero;
			if((Dis_Y_MAX - y) < Y_Witch) break;
			else y += Y_Witch;
		}
		p++;
	}
}
//========================================================================
// ����: void PutChar_cn(unsigned char x,unsigned char y,unsigned short * GB) 
// ����: д��һ���������ֿ⺺��
// ����: x  X������     y  Y������
//		 a  GB��  
// ����: ��
// ��ע: 
// �汾:
//      2007/07/19      First version
//		2007/07/24		V1.2 for MCS51 Keil C
//========================================================================
void PutChar_cn(unsigned char x,unsigned char y,unsigned char * GB) 
{
	//��ʾASCII�ַ�
	SPI_SSSet(0);					//SS�õ͵�ƽ	
	SPI_Send(8);					//����ָ��0x08
	SPI_Send(x);					//Ҫ��ʾ�ַ������Ͻǵ�X��λ��
	SPI_Send(y);					//Ҫ��ʾ�ַ������Ͻǵ�Y��λ��
	
	SPI_Send(*(GB++));	//���Ͷ����ֿ��к���GB��ĸ߰�λֵ
	SPI_Send(*GB);		//���Ͷ����ֿ��к���GB��ĵͰ�λֵ
	SPI_SSSet(1);					//��ɲ�����SS�ߵ�ƽ
}
//========================================================================
// ����: void PutString_cn(unsigned char x,unsigned char y,unsigned short *p)
// ����: ��x��yΪ��ʼ���괦д��һ�������ַ�
// ����: x  X������     y  Y������
//		 p  Ҫ��ʾ���ַ���  
// ����: ��
// ��ע: ͬPutChar_cn�еĽ���
// �汾:
//      2007/07/19      First version
//		2007/07/24		V1.2 for MCS51 Keil C
//========================================================================
void PutString_cn(unsigned char x,unsigned char y,unsigned char *p)
{

	while(*p!=0)
	{
		if(*p<128)
		{
			PutChar(x,y,*p);
			x += X_Witch+1;
			if((x + X_Witch) > Dis_X_MAX + 2)
			{
				x = Dis_Zero;
				if((Dis_Y_MAX - y) < Y_Witch) break;
				else y += Y_Witch_cn;
			}
			p+=1;
		}
		else
		{
			PutChar_cn(x,y,p);
			x += X_Witch_cn+1;
			if((x + X_Witch_cn) > Dis_X_MAX + 2)
			{
				x = Dis_Zero;
				if((Dis_Y_MAX - y) < Y_Witch_cn) break;
				else y += Y_Witch_cn;
			}
			p+=2;
		}
	}
}
   
//========================================================================
// ����: void SetPaintMode(unsigned char Mode,unsigned char Color)
// ����: ��ͼģʽ����
// ����: Mode ��ͼģʽ    Color  ���ص����ɫ,�൱��ǰ��ɫ  
// ����: ��
// ��ע: Mode��Ч
// �汾:
//      2007/07/19      First version
//========================================================================

void SetPaintMode(unsigned char Mode,unsigned char Color)
{
	xdata unsigned char ucTemp=0;
	ucTemp = (Mode<<4)|Color;
	//���û�ͼģʽ
	SPI_SSSet(0);					//SS�õ͵�ƽ			
	SPI_Send(0x83);					//����ָ��0x83
	SPI_Send(ucTemp);				//ѡ��8X16��ASCII����,�ַ�ɫΪ��ɫ
	SPI_SSSet(1);					//��ɲ�����SS�ߵ�ƽ
}

//========================================================================
// ����: void PutPixel(unsigned char x,unsigned char y)
// ����: ��x��y���ϻ���һ��ǰ��ɫ�ĵ�
// ����: x  X������     y  Y������
// ����: ��
// ��ע: ʹ��ǰ��ɫ
// �汾:
//      2007/07/19      First version
//========================================================================
/*
void PutPixel(unsigned char x,unsigned char y)
{
	//������	
	SPI_SSSet(0);					//SS�õ͵�ƽ
	SPI_Send(1);					//��ָ��0x01
	SPI_Send(x);					//�͵�һ������,�����õ��X��λ��
	SPI_Send(y);					//���Y��λ��
	SPI_SSSet(1);					//��ɲ�����SS�ߵ�ƽ
}
*/
//========================================================================
// ����: void Line(unsigned char s_x,unsigned char  s_y,
//					unsigned char  e_x,unsigned char  e_y)
// ����: ��s_x��s_yΪ��ʼ���꣬e_x��e_yΪ�����������һ��ֱ��
// ����: x  X������     y  Y������
// ����: ��
// ��ע: ʹ��ǰ��ɫ
// �汾:
//      2007/07/19      First version
//========================================================================

void Line(unsigned char s_x,unsigned char  s_y,unsigned char  e_x,unsigned char  e_y)
{  
	//����ֱ��
	SPI_SSSet(0);					//SS�õ͵�ƽ
	SPI_Send(2);					//��ָ��0x02
	SPI_Send(s_x);					//���X������
	SPI_Send(s_y);					//���Y������
	SPI_Send(e_x);					//�յ�X������
	SPI_Send(e_y);					//�յ�Y������
	SPI_SSSet(1);					//��ɲ�����SS�ߵ�ƽ
}

//========================================================================
// ����: void Circle(unsigned char x,unsigned char y,
//					unsigned char r,unsigned char mode)
// ����: ��x,yΪԲ��RΪ�뾶��һ��Բ(mode = 0) or Բ��(mode = 1)
// ����: 
// ����: ��
// ��ע: ��Բ����ִ�н��������MCU�п��Ź����������幷�Ĳ���
// �汾:
//      2007/07/19      First version
//========================================================================
/*
void Circle(unsigned char x,unsigned char y,unsigned char r,unsigned char mode)
{
	SPI_SSSet(0);
	if(mode)
		SPI_Send(6);
	else
		SPI_Send(5);
	SPI_Send(x);
	SPI_Send(y);
	SPI_Send(r);
	SPI_SSSet(1);
}
*/
//========================================================================
// ����: void Rectangle(unsigned char left, unsigned char top, unsigned char right,
//				 unsigned char bottom, unsigned char mode)
// ����: ��x,yΪԲ��RΪ�뾶��һ��Բ(mode = 0) or Բ��(mode = 1)
// ����: left - ���ε����ϽǺ����꣬��Χ0��126
//		 top - ���ε����Ͻ������꣬��Χ0��62
//		 right - ���ε����½Ǻ����꣬��Χ1��127
//		 bottom - ���ε����½������꣬��Χ1��63
//		 Mode - ����ģʽ��������������ֵ֮һ��
//				0:	���ο򣨿��ľ��Σ�
//				1:	�����棨ʵ�ľ��Σ�
// ����: ��
// ��ע: ��Բ����ִ�н��������MCU�п��Ź����������幷�Ĳ���
// �汾:
//      2007/07/19      First version
//========================================================================
void Rectangle(unsigned char left, unsigned char top, unsigned char right,
				 unsigned char bottom, unsigned char mode)
{
	SPI_SSSet(0);
	if(mode)
		SPI_Send(4);
	else
		SPI_Send(3);
	SPI_Send(left);
	SPI_Send(top);
	SPI_Send(right);
	SPI_Send(bottom);
	SPI_SSSet(1);
}
//========================================================================
// ����: void ClrScreen(void)
// ����: ����������ִ��ȫ��Ļ���
// ����: 
// ����: ��
// ��ע: 
// �汾:
//      2007/07/19      First version
//========================================================================
void ClrScreen(void)
{
	//��������
	SPI_SSSet(0);					//SS�õ͵�ƽ
	SPI_Send(0x80);					//��ָ��0x80
	SPI_SSSet(1);					//��ɲ�����SS�ߵ�ƽ
}
//========================================================================
// ����: void PutBitmap(unsigned char x,unsigned char y,unsigned char width,
//						unsigned char high,unsigned char *p)
// ����: ��x��yΪ��ʼ������ʾһ����ɫλͼ
// ����: x  X������     y  Y������
//		 width λͼ�Ŀ��  
//		 high  λͼ�ĸ߶�
//		 p  Ҫ��ʾ��λͼ����ģ�׵�ַ  
// ����: ��
// ��ע: ��
// �汾:
//      2008/11/27      First version
//========================================================================
/*
void PutBitmap(unsigned char x,unsigned char y,unsigned char width,unsigned char high,unsigned char *p)
{
	xdata unsigned short Dat_Num;
	//unsigned short i;
	xdata unsigned char ucTemp=0;
	SPI_SSSet(0);					//SS�õ͵�ƽ
	SPI_Send(0x0e);	
	SPI_Send(x);
	SPI_Send(y);
	SPI_Send(width);
	SPI_Send(high);
	
	width = width+0x07;
	Dat_Num = (width>>3)*high;
	while(Dat_Num--)
	{
	  	ucTemp++;
		SPI_Send(*p);
		if(ucTemp>250)				//����Ļ����MCUʱ,���������Ҫ�ʵ��ĵ���
		{
			TimeDelay(28);			//�Լ��˴�,�Ա�֤ǰ�洫�͵�LCDģ���е������ѱ�������
			ucTemp = 0;
		}
		p++;
	}
	SPI_SSSet(1);					//��ɲ�����SS�ߵ�ƽ
}


//========================================================================
// ����: void ShowChar(unsigned char x,unsigned char y,unsigned char Num,unsigned char type)
// ����: ��x��y��λ��Ϊ��ʾ��������Ͻǣ���ʾһ��unsigned char����
// ����: x  X������     y  Y������    Num  Ҫ��ʾ��8λ��ȵ�����  type  ��ʾ����(0,1,2)
// ����: ��
// ��ע: 
// �汾:
//      2008/11/27      First version
//========================================================================
void ShowChar(unsigned char x,unsigned char y,unsigned char a,unsigned char type) 
{
	//��ʾASCII�ַ�
	SPI_SSSet(0);					//SS�õ͵�ƽ	
	SPI_Send(11);					//����ָ��0x0B
	SPI_Send(x);					//Ҫ��ʾ�ַ������Ͻǵ�X��λ��
	SPI_Send(y);					//Ҫ��ʾ�ַ������Ͻǵ�Y��λ��
	SPI_Send(a);					//Ҫ��ʾ������
	SPI_Send(type);
	SPI_SSSet(1);					//��ɲ�����SS�ߵ�ƽ
}
//========================================================================
// ����: void ShowShort(unsigned char x,unsigned char y,unsigned short Num,unsigned char type)
// ����: ��x��y��λ��Ϊ��ʾ��������Ͻǣ���ʾһ��unsigned short������16λ��ȣ�
// ����: x  X������     y  Y������    Num  Ҫ��ʾ��16λ��ȵ�����  type  ��ʾ����(0,1,2)
// ����: ��
// ��ע: 
// �汾:
//      2008/11/27      First version
//========================================================================
void ShowShort(unsigned char x,unsigned char y,unsigned short a,unsigned char type) 
{
	//��ʾASCII�ַ�
	SPI_SSSet(0);					//SS�õ͵�ƽ	
	SPI_Send(12);					//����ָ��0x0C
	SPI_Send(x);					//Ҫ��ʾ�ַ������Ͻǵ�X��λ��
	SPI_Send(y);					//Ҫ��ʾ�ַ������Ͻǵ�Y��λ��
	SPI_Send((unsigned char)(a>>8));
	SPI_Send((unsigned char)a);
	SPI_Send(type);					//Ҫ��ʾ�ַ�ASCII�ַ���ASCII��ֵ
	SPI_SSSet(1);					//��ɲ�����SS�ߵ�ƽ
}
*/
//========================================================================
// ����: void SetBackLight(unsigned char Deg)
// ����: ���ñ������ȵȼ�
// ����: Grade  0~127�ĵȼ�
// ����: ��
// ��ע: 
// �汾:
//      2008/11/27      First version
//========================================================================
void SetBackLight(unsigned char Deg) 
{
	//��ʾASCII�ַ�
	SPI_SSSet(0);					//SS�õ͵�ƽ	
	SPI_Send(0x8a);					//����ָ��0x07
	SPI_Send(Deg);					//Ҫ��ʾ�ַ������Ͻǵ�X��λ��
	SPI_SSSet(1);					//��ɲ�����SS�ߵ�ƽ
}


//==========jwz add===========
unsigned char cStrLen(char * str)
{
	char* p = str;
	while(*p++);
	return ((p-str))/2;
}

unsigned char StrLen(char * str)
{
	char* p = str;
	while(*p++);
	return p-str-1;
}

void jmemcpy(char* s, char * d,unsigned int ls)
{
	while(ls--) *d++ =  *s++;
}

bit isNum(char* s)
{
	while(*s)
	{
		if (*s<'0' || *s>'9') 
			return 0;
		s++;
	}
	return 1;
}

//********************************************************
//i ���Ʊ�־
// 0000 0000
// 1 		 = InEdit								0x80
//  1 		 = cstring								0x40
//   1 		 = string								0x20
//    1 	 = Integer								0x10
//													
//      1 	 = char									0x08
//       1   = Font size 1=>Large font,0 small font	0x04
//        0  = NC									0x02
//         1 = invert								0x01
//********************************************************
#define P_EDIT	0x80
#define P_CSTR	0x40
#define P_ASTR	0x20
#define P_INT	0x10
#define P_CHAR	0x08
#define P_FONT	0x04
#define P_AINV	0x02
#define P_INV	0x01
unsigned char Print(char x,char y,char* str,char i)
{
	char len = (i & (P_CSTR|P_ASTR))?StrLen(str):(i & P_INT)?6:4;
	bit inv = 1, isn = 0;
	if (i & P_EDIT)	//InEdit
	{
		SetPaintMode(0,0);
		Rectangle(x,y,127,y+12,1);
		SetPaintMode(0,1);
		Line(x,y+12,x+len*6+3,y+12);	
		FontMode(1,0);
	}
	else if (i & (P_AINV | P_INV))	//Invert
	{
		//SetPaintMode(0,1);
		if (i & P_AINV)
		{
			Rectangle(0,y,127,y+12,1);
		}
		else
		{
			Rectangle(x,y,x+len*6+3,y+12,1);
		}
		FontMode(1,1); inv = 0;
	}
	else
	{
		FontMode(1,0);
	}

	if (i & P_FONT) 	//Large font
	{
		FontSet_cn(1, inv);
		FontSet(1, inv);
	}
	else				//Small font
	{
		FontSet_cn(0, inv);
		FontSet(0, inv);
	}

	if (i & P_CSTR)
	{
		isn = isNum(str);
		PutString_cn(x+(isn?1:0),y+(isn?2:0),str);
	}
	else if (i & P_ASTR)
	{
		PutString(x+(isn?1:0),y+(isn?2:0),str);
	}
	else if (i & P_INT)
	{
		PutString(x+1,y+2,i2s(*(int*)str,6));
	}
	else if (i & P_CHAR)
	{
		//ShowChar(x,y,*str,1);
		PutString(x+1,y+2,i2s((char)*(char*)str,0x83));
	}
	return (x+7*len);
}

unsigned char cPrint12(unsigned char cfs,unsigned char efs,char line,char Column,char * str,unsigned char i)
{
	xdata unsigned char len = cStrLen(str);
	FontMode(1,0);
	FontSet(efs,1);
	FontSet_cn(cfs,1);
	if (line<1) line = 1; if (Column<1) Column=1;
	if (i)
	{
		if (i&0x80)
		{
			Line(Column-1,line+13,Column+13*len,line+13);
		}
		else
		{
			FontSet(efs,0);
			FontSet_cn(cfs,0);
			FontMode(1,1);
			if (i==2)
			{
				Rectangle(0,line-1,127,line+12,1);
			}
			else
			{
				Rectangle(Column-1,line-1,Column+13*len,line+12,1);
			}
		}
	}
	PutString_cn(Column-((len<10)?0:1),line,str);
	return (Column+13*len);
}

/*
unsigned char PrintNum(unsigned char cfs,unsigned char efs,char line,char Column,unsigned int num,unsigned char i)
{
	xdata unsigned char len = ((i&0x10)?5:3);
	FontMode(1,0);
	FontSet(efs,1);
	FontSet_cn(cfs,1);
	if (line<1) line = 1; if (Column<1) Column=1;
	if (i&0x0F)
	{
		if ((i & 0x80))
		{
			//Rectangle(Column-1,Line-1,Column+6*len-1,Line-1,0);
			Line(Column,line,Column + 6*len,line);
		}
		else
		{
			FontSet(efs,0);
			FontSet_cn(cfs,0);
			FontMode(1,1);
			Rectangle(Column-1,line-1,Column+6*len-1,line+12,1);
		}
	}

	if (i & 0x10)
	{
		PutString(Column+2,line,i2s(num,));
		//ShowShort(Column,Line,num,1);
	}
	else
	{
		//PutString(Column+2,Line,i2s((char)num));
		ShowChar(Column+2,line,num,1);
	}
	return (Column+13*len);
}
*/
/*
void Print6(unsigned char fs,char Line,char Column, char * str,unsigned char i)
{
	FontMode(1,0);
	FontSet(fs,1);
	if (i)
	{
		FontSet(fs,0);
		FontMode(0,1);
		Rectangle(Column,Line,Column+6*StrLen(str)-1,Line+8,1);
	}
	PutString(Column,Line,str);
	FontMode(1,0);
}
*/
#endif //__LCD_DISCFILE__
