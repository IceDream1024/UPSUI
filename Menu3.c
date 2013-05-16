#include "define.h"
#ifndef __MENUCFILE__
#define __MENUCFILE__

#include "menu3.h"
#include "LCD_Dis.h"

#include "flash.h"

#ifdef _KEY
#include "key.h"
#endif

#include "modmst.h"

xdata CValue IValue = {0};
xdata CValue Vadj = {0};
xdata SValue SetValue = {0};

typedef struct _strArr
{
    int len;
    char **sa;
} strArr;

typedef struct _SettingModbus
{
	char f;
	int addr;
	int value;
} SMB;

enum ITEMTYPE
{
    CONST,      //0
    CHAR,		//1
    INTEGER,    //2
	STRING,		//3
    FUNCTION,   //4
	PASSWORD,   //5
    MENU        //6
};

enum MENUSTAT
{
	IDLE,
	BUSY,
	EDIT,
	PASS,
	COMM
};

#define TYPE(mit,misc) ((((unsigned char)(mit)<<5) & 0xE0) | ((unsigned char)(misc) & 0x1F))
#define MIT(type) (((unsigned char)(type)>>5) & 0x07)
#define MISC(misc) ((unsigned char)(misc) & 0x1F)

MENUPARAM mr = {0};
SMB smb = {0};

typedef void (*FUNC)();

#define CHAR(c)		(&c)
#define INT(i)      (&i)
#define STR(s)      (s)
#define FUNC(f)     (&f)

#define CCHAR(c)	(*(char*)(c))
#define CINT(i)     (*(int*)(i))
#define CSTR(s)     ((char*)(s))
#define CFUNC(f)    ((FUNC)(f))
#define CMENU(m)    ((MenuItem*)(m)) //(mr.ptr)+

#define cmi (mr.sm + mr.id)			//��ǰ����Ӧ�Ĳ˵���

xdata int LightCnt = 0;
bit isBackLightOn = 0;
bit SettingChanged = 1;
int Password = 0;

xdata unsigned int DspCnt = 0;
void MenuDrv(void)
{
    if (DspCnt > 0)DspCnt++;
    if (LightCnt > 0) LightCnt--;
}

void LightOn(unsigned char Deg)
{
    SetBackLight(Deg);
    isBackLightOn = 1;
	if (SetValue.BKLtime>32767) 
	{
		LightCnt = -1;
		SetValue.BKLtime = -1;
	}
	else
	if (SetValue.BKLtime>=300)
	{
		SetValue.BKLtime = 300;
	}
   	LightCnt = SetValue.BKLtime * 100;
}

void LightOff() //�رձ���
{
    SetBackLight(0);
    isBackLightOn = 0;
}


void Welcome() //Welcome
{
    ClrScreen();

    FontMode(0, 0);
    FontSet(0, 1);
    cPrint12(1, 0, 1, 24, "����ר��UPS", 0);
    cPrint12(0, 0, 24, 0, "�Ϻ�ͳ���������޹�˾", 0);
    FontMode(1, 0);
    FontSet(1, 1);
    PutString(18, 46, "TBU10KVA220V");
}

code char *WorkMode[] = {"����ʽ", "����ʽ"};
code char *StartMode[] = {"�ֶ�", "�Զ�"};

code char *BaudRate[] = {"1200", "2400", "4800", "9600"};
code char *DataBits[] = {"8", "7"};
code char *StopBits[] = {"1", "2"};
code char *OddEven[] = {"��", "��", "ż"};

code char *StartCtrl[] = {"ֹͣ","����"};
code char *ByPassCtrl[] = {"��·","���"};

code strArr SettingArray[] =
{
    {2, WorkMode},      //0
    {2, StartMode},     //1
    {4, BaudRate},      //2
    {2, DataBits},      //3
    {2, StopBits},      //4
    {3, OddEven},       //5
	{2, StartCtrl},		//6
	{2, ByPassCtrl}		//7
};

code char *FaultInfo[] =
{
    {"����ʧ��"},
    {"��·ʧ��"},
    {"��ع�ѹ"},
    {"���Ƿѹ"},
    {"��·ʧ��"},
    {"��·ʧ��"}
};

extern code MenuItem MainMenu[];

#define SSCNT 4
code MenuItem SysSetting[] =
{
    { -1, "ϵͳ����", TYPE(MENU,SSCNT), MainMenu},
    {0, " ������ʽ:", TYPE(STRING,INTEGER), INT(SetValue.wMode)},
    {1, " ����ģʽ:", TYPE(STRING,INTEGER), INT(SetValue.sMode)},
    { -1, " ����ʱ��:", TYPE(INTEGER,INTEGER), INT(SetValue.BKLtime)},
    { -1, " ��������:", TYPE(INTEGER,INTEGER), INT(SetValue.Password)}
};

#define CSCNT 5
code MenuItem ComSetting[] =
{
    { -1, "ͨѸ����", TYPE(MENU,CSCNT), MainMenu},
    { -1, " վ  ַ:", TYPE(CHAR,INTEGER), INT(SetValue.ModbusSA)},
    {2, " ������:", TYPE(STRING,INTEGER), INT(SetValue.BaudRate)},
    {3, " ����λ:", TYPE(STRING,INTEGER), INT(SetValue.DataBits)},
    {4, " ֹͣλ:", TYPE(STRING,INTEGER), INT(SetValue.StopBits)},
    {5, " У��λ:", TYPE(STRING,INTEGER), INT(SetValue.OddEven)}
};

#define IMCNT 11
code MenuItem Info[] =
{
    { -1, "ϵͳ��Ϣ", TYPE(MENU,IMCNT), MainMenu},
    { -1, " �����ѹ:", TYPE(CONST,INTEGER), INT(IValue.ACInV)},
    { -1, " �������:", TYPE(CONST,INTEGER), INT(IValue.ACInI)},
    { -1, " ����Ƶ��:", TYPE(CONST,INTEGER), INT(IValue.ACInF)},
    { -1, " �����ѹ:", TYPE(CONST,INTEGER), INT(IValue.InvV)},
    { -1, " ���ص���:", TYPE(CONST,INTEGER), INT(IValue.LoadI)},
    { -1, " ���Ƶ��:", TYPE(CONST,INTEGER), INT(IValue.InvF)},
    { -1, " �������:", TYPE(CONST,INTEGER), INT(IValue.Power)},
    { -1, " ��������:", TYPE(CONST,INTEGER), INT(IValue.Factor)},
    { -1, " ��ص�ѹ:", TYPE(CONST,INTEGER), INT(IValue.BatV)},
    { -1, " ������:", TYPE(CONST,INTEGER), INT(IValue.BatI)},
    { -1, " ���״̬:", TYPE(CONST,INTEGER), INT(IValue.ChargeStatus)}
};

#define IACNT 11
code MenuItem InfoAdj[] =
{
    { -1, "����У��", TYPE(MENU,IACNT), MainMenu},
    { -1, " �����ѹ:", TYPE(INTEGER,INTEGER), INT(Vadj.ACInV)},
    { -1, " �������:", TYPE(INTEGER,INTEGER), INT(Vadj.ACInI)},
    { -1, " ����Ƶ��:", TYPE(INTEGER,INTEGER), INT(Vadj.ACInF)},
    { -1, " �����ѹ:", TYPE(INTEGER,INTEGER), INT(Vadj.InvV)},
    { -1, " ���ص���:", TYPE(INTEGER,INTEGER), INT(Vadj.LoadI)},
    { -1, " ���Ƶ��:", TYPE(INTEGER,INTEGER), INT(Vadj.InvF)},
    { -1, " �������:", TYPE(INTEGER,INTEGER), INT(Vadj.Power)},
    { -1, " ��������:", TYPE(INTEGER,INTEGER), INT(Vadj.Factor)},
    { -1, " ��ص�ѹ:", TYPE(INTEGER,INTEGER), INT(Vadj.BatV)},
    { -1, " ������:", TYPE(INTEGER,INTEGER), INT(Vadj.BatI)},
    { -1, " ���״̬:", TYPE(INTEGER,INTEGER), INT(Vadj.ChargeStatus)}
};

#define PMCNT 1
code MenuItem PasswordMenu[] =
{
    { -1, "��������������", TYPE(MENU,PMCNT), MainMenu},
    { -1, " ����:", TYPE(INTEGER,INTEGER), INT(Password)},
    //{2,"��    ��",FUNCTION,Exit,MMCNT},
};

#define CMCNT 2
code MenuItem ControlMenu[] =
{
    { -1, "ϵͳ����", TYPE(MENU,CMCNT), MainMenu},
    { 6, " ��ͣ����:", TYPE(STRING,INTEGER), INT(SetValue.StartCtrl)},
    { 7, " �ֹ���·:", TYPE(STRING,INTEGER), INT(SetValue.ByPassCtrl)}
};

#define MMCNT 5
code MenuItem MainMenu[] =
{
    { -1, "���˵�", TYPE(FUNCTION,MMCNT), Welcome},
    { -1, " ϵͳ��Ϣ", TYPE(MENU,0), Info},
    { -1, " ϵͳ����", TYPE(MENU,0), ControlMenu},
    { -1, " ϵͳ����", TYPE(PASSWORD,0), SysSetting},
    { -1, " ͨѶ����", TYPE(PASSWORD,0), ComSetting},
    { -1, " ����У��", TYPE(PASSWORD,0), InfoAdj}
};

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
void ShowMenu(MENUPARAM *m)
{
    #define FID(id) (((id) - 1) / 3 * 3)
    #define dcmi (m->sm + i)

    unsigned char fid = FID(m->id), i, x;
    if (!mr.stat) ClrScreen();
    cPrint12(0, 0, 0, (128 - 12 * cStrLen((m->sm)->ItemStr)) / 2, (m->sm)->ItemStr, 2);
    for (i = fid + 1; i <= ((fid + 3 < MISC(m->sm->ItemType)) ? (fid + 3) : MISC(m->sm->ItemType)); i++)
    {
        x = cPrint12(0, 0, 18 + ((i - 1) % 3) * 16, 0, dcmi->ItemStr, 
			((MIT(dcmi->ItemType) == MENU || MIT(dcmi->ItemType) == PASSWORD) && (m->id == i)));
        if (MIT(dcmi->ItemType) == STRING)
        {
            if (CINT(dcmi->ptr) > SettingArray[dcmi->id].len - 1) CINT(dcmi->ptr) = 0;
            if (CINT(dcmi->ptr) < 0) CINT(dcmi->ptr) = SettingArray[dcmi->id].len - 1;
            Print(x, 18 + ((i - 1) % 3) * 16, *(SettingArray[dcmi->id].sa + CINT(dcmi->ptr)),
				(m->id == i) ? (mr.stat==EDIT ? 0xC0 : 0x41) : 0x40);
        }
        else if (MIT(dcmi->ItemType) == CHAR)
        {
            Print(x, 18 + ((i - 1) % 3) * 16, ((char *)(dcmi->ptr)+1), 
                (m->id == i) ? (mr.stat==EDIT ? 0x88 : 0x09) : 0x08);
		}
        else if (MIT(dcmi->ItemType) == INTEGER)
        {
            Print(x, 18 + ((i - 1) % 3) * 16, (char *)(dcmi->ptr), 
				(m->id == i) ? (mr.stat==EDIT ? 0x90 : 0x11) : 0x10);
        }
        else if (MIT(dcmi->ItemType) == CONST)
        {
            Print(x, 18 + ((i - 1) % 3) * 16, (char *)(dcmi->ptr), 0x10);
        }
    }
}


bit ReadSetting()
{
    return ModMst(2, 3, 0x4000, 4, (unsigned char *)&SetValue);
}

void InitMenu()
{
    LCD_Init();
    mr.stat = IDLE;
    mr.mm = 0;
    mr.sm = MainMenu;
    mr.id = 1;
    mr.bid = mr.id;
    ShowMenu(&mr);
}

// mr.sm + mr.id => ��ǰ��
// cmi->ptr => Ϊ��ǰ�� ��Ӧ�� �������Ӳ˵� ��ָ��

void Menu(unsigned char key)
{
    switch (key)
    {
    case K_UP: case K_UPOK: case K_UPESC:
    {
        if (mr.stat==BUSY)			//�˵�æ,�����ǲ��ڲ˵���,�����FUNCTION
        {
            key = 0;
        }
        else if (mr.stat==EDIT)		//�˵����ڱ༭��
        {
            if (MISC(cmi->ItemType) == CHAR)
            {
                CCHAR(cmi->ptr) += (key == K_UP) ? 1 : 10;;
            }
            else if (MISC(cmi->ItemType) == INTEGER)
            {
                CINT(cmi->ptr) += (key == K_UP) ? 1 : (key == K_UPOK) ? 10 : 100;
            }
        }
        else 						//�˵���ѡ��
		{
			if (MIT(cmi->ItemType) == CONST)	//�˵�Ϊ����ʾ,��������
	        {
	            if (mr.id > 3) mr.id = (mr.id - 3) / 3 * 3 + 1; else mr.id = MISC(mr.sm->ItemType) / 3 * 3 + 1;
	        }
	        else
	        {
	            if (mr.id > 1) mr.id --; else mr.id = MISC(mr.sm->ItemType);
	        }
		}
        break;
    }
    case K_DN: case K_DNOK: case K_DNESC:
    {
        if (mr.stat==BUSY)
        {
            key = 0;
        }
        else if (mr.stat==EDIT)
        {
            if (MISC(cmi->ItemType) == CHAR)
            {
                CCHAR(cmi->ptr) -= (key == K_DN) ? 1 : 10;
            }
            else if (MISC(cmi->ItemType) == INTEGER)
            {
                CINT(cmi->ptr) -= (key == K_DN) ? 1 : (key == K_DNOK) ? 10 : 100;
            }
        }
        else
		{
			if (MIT(cmi->ItemType) == CONST)
	        {
	            if (mr.id < (MISC(mr.sm->ItemType)) / 3 * 3 + 1) mr.id = (mr.id + 3) / 3 * 3 + 1; else mr.id = 1;
	        }
	        else
	        {
	            if (mr.id < (MISC(mr.sm->ItemType))) mr.id ++; else mr.id = 1;
	        }
		}
        break;
    }
    case K_OK:
    {
        if (mr.stat==BUSY)	//�˳�FUNCTION
        {
            mr.stat = IDLE;
        }
		else if (MIT(cmi->ItemType) == PASSWORD)	//��������ܽ���
		{
			mr.mm = CMENU(cmi->ptr);
			mr.sm = PasswordMenu;
			mr.bid = mr.id;                      //�����ϼ��˵���λ��
			mr.id = 1;                           

            if (CINT(cmi->ptr) == SetValue.Password || CINT(cmi->ptr) == 1234)   //��������Ϊ1000
            {
                mr.sm = mr.mm;                  //��������(mr.mmָ��Ҫ��������ò˵�ָ��)
	            if (mr.sm == InfoAdj)           //�ǲ��������˵�?
	            {
	                jmemcpy((char *)&IValue, (char *)&Vadj, 24);   //������ʾֵ����������
	            }
                mr.mm = 0;                      //�ѽ�������
            }
		}
        else if (MIT(cmi->ItemType) == MENU)
        {
            mr.sm = CMENU(cmi->ptr);
            mr.bid = mr.id;
            mr.id = 1;

        }
        else if (MIT(cmi->ItemType) == FUNCTION)
        {
            CFUNC(cmi->ptr)();
        }
		else if ((MIT(cmi->ItemType) == CHAR) || (MIT(cmi->ItemType) == INTEGER) || (MIT(cmi->ItemType) == STRING))
        {
            if (mr.stat==EDIT)//�ڱ༭��,�����˳��༭״̬
            {
				if (mr.mm != 0)	//������Ի���?
				{
					if (CINT(cmi->ptr) == SetValue.Password || CINT(cmi->ptr) == 1234)
					{
						mr.sm = mr.mm;
			            if (mr.mm == InfoAdj)
			            {
			                jmemcpy((char *)&IValue, (char *)&Vadj, 24);
			            }
					}
					else		//���벻��ȷ,����ԭ���Ĳ˵�
					{
						mr.sm = mr.sm->ptr;
						mr.id = mr.bid;
						mr.bid = 0;
					}
					mr.mm = 0;
				}
				else		//������Ի���,�������˵�
				{						
					//�˴���ʼ����༭��ֵ
					if (mr.sm == InfoAdj)	//��ǰʵ�ֵ���INTEGER�������ݵı���(MISC()==INTEGER)
					{
						ModMst(2,6,(cmi->ptr - &Vadj)/2,CINT(cmi->ptr),0);						
					}
					else
					{
						SettingChanged = 1;
						ModMst(2,6,0x4000+(cmi->ptr - &SetValue)/2,CINT(cmi->ptr),0);
					}
				}
				mr.stat = IDLE;
            }
            else	//���ڱ༭��,�����༭״̬
            {
                mr.stat = EDIT;
            }
        }
        break;
    }
    case K_ESC:
    {
        if (mr.stat == EDIT)
        {
            mr.stat = IDLE;
        }

		if (MIT(mr.sm->ItemType) == FUNCTION)
        {
            CFUNC(mr.sm->ptr)(); 
			mr.stat = BUSY;
            Password = 0;            
            key = 0;
        }
		else
        {
            mr.sm = CMENU(mr.sm->ptr);
            mr.id = mr.bid;
            mr.bid = 0;
        }
        break;
    }
	default:
		key = 0;
    }

	//��ʱ�ԵĴ����������Ϊ����,��ʾ����Ϊ�ַ��ı���
	if (MIT(cmi->ItemType)==CHAR && MISC(cmi->ItemType)==INTEGER)
	{
		CINT(cmi->ptr) = CINT(cmi->ptr) & 0xFF;
	}


    if (key)
    {
        ShowMenu(&mr);
        //ShowChar(108,56,SetValue.ModbusSA>>8,1);

        LightOn(127);
        if (mr.sm == Info) DspCnt = 1; else DspCnt = 0;
    }

    if (DspCnt > 100)
    {
        ShowMenu(&mr);
        DspCnt = 1;
    }

    if (LightCnt==0 && isBackLightOn)
    {
        LightOff();
    }
}

#endif //__MENUCFILE__