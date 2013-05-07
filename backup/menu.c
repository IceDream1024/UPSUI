#include "reg52.h"
#include "define.h"
#include "menu.h"
#include "LCD_Dis.h"
#include "LCD_Dis.c"

#ifdef KEY
#include "key.h"
//unsigned char kState = 0;
#endif

CValue IValue={0};

INT MenuItemID;
unsigned int RefreshMenuCnt=0;

#define iItemID MenuItemID.BX.l
#define iSetID MenuItemID.BX.h

#define MSTR_TITLE(mid) (MS[MS[mid].bid].ItemStr)
//#define MSTR_FID(cid) (((cid - MS[cid].bid - 1) / 3) * 3 + MS[cid].bid)
//#define MSTR_LID(cid) (((cid - MS[cid].bid - 1) / 3) * 3 + MS[cid].bid + 2)

#define MSTR_FID(cid) (((cid - MS[cid].bid - 1) / 3) * 3 + MS[cid].bid + 1)
#define MSTR_LID(cid) (((MSTR_FID(cid)+3)<=MS[cid].eid)?(MSTR_FID(cid)+2):(MS[cid].eid))

#define MSTR(cid) (MS[cid].ItemStr)

#define MenuItemCount() (sizeof(MS)/sizeof(MenuItem) - 1)
#define GetTitleID(mID) (MS[(mID)].bid)
#define isSet(mID)(MS[(mID)].isSet)

code MenuItem MS[]=
{
//	 b,e,mstr   ,isset,proc()
	{0,3,"ϵͳ����"   ,1,0},	//0
	{0,3,"����ģʽ��" ,1,0},	//1
	{0,3,"������ʽ��" ,1,0},	//2
	{0,3,"����״̬��" ,1,0},	//3

	{4,8,"ͨѶ����"   ,1,0},	//4
	{4,8,"BaudRate��" ,1,0},	//5
	{4,8,"DataBits��" ,1,0},	//6
	{4,8,"StopBits��" ,1,0},	//7
	{4,8,"��żУ�飺" ,1,0},	//8
		
	{9,20,"ϵͳ��Ϣ"  ,0,0},	//9

	{9,20,"�����ѹ��",0,0},	//10
	{9,20,"���������",0,0},	//11
	{9,20,"����Ƶ�ʣ�",0,0},	//12

	{9,20,"����ѹ��",0,0},	//13
	{9,20,"���ص�����",0,0},	//14
	{9,20,"���Ƶ�ʣ�",0,0},	//15

	{9,20,"������ʣ�",0,0},	//16
	{9,20,"�������أ�",0,0},	//17

	{9,20,"��ص�ѹ��",0,0},	//18
	{9,20,"��������",0,0},	//19
	{9,20,"���״̬��",0,0}		//20
};

code char* FaultInfo[]=
{
	{"����ʧ��"},
	{"��·ʧ��"},
	{"��ع�ѹ"},
	{"���Ƿѹ"},
	{"��·ʧ��"},
	{"��·ʧ��"}
};

code int BdRate[]={9600,4800,2400,1200};
code char DataBits[]={8,7};
code char *OddEvn[]={"��","��","ż"};
code char *WorkMode[]={"����ʽ","����ʽ"};
code char *StartMode[]={"�Զ�","�ֶ�"};

void LightOn(unsigned char Deg)
{
	SetBackLight(Deg);
}

void LightOff()
{
	SetBackLight(0);
}

void Welcome() //Welcome
{
	MenuItemID.x = 0;
	ClrScreen();

	FontMode(0,0);
	FontSet(0,1);
	cPrint12(1,0,1,24,"����ר��UPS",0);
	cPrint12(0,0,24,0,"�Ϻ�ͳ���������޹�˾",0);
	FontMode(1,0);
	FontSet(1,1);
	PutString(18,46,"TBU10KVA220V");
}

char *i2s(long v)
{
	#define LEN 12
	static char vs[LEN+1];
	char i = LEN,s; v=(s=(v<0))?-v:v;
	vs[LEN] = 0;

	do{ vs[--i]=(v%10)+'0'; v/=10; }while(v && i);
	if(s)vs[--i]='-';

	return vs+i;
}

void InitMenu(void)
{
	iItemID = 0;
	iSetID = 0;
	ShowMenu(iItemID);
}


void Dec(char *mID)
{
	if (!MS[*mID].isSet)
	{
		if (*mID-3<MS[*mID].bid)
		{
			*mID = MS[*mID].bid - 1;
		}
		else
		{
			*mID = ((*mID - MS[*mID].bid)/3 - 1)*3+MS[*mID].bid+1;
		}
	}
	else
	{
		(*mID) --;
		if (MS[*mID].bid == *mID)
		{
			(*mID)--;
		}
	}
	if (*mID<1) *mID = MenuItemCount();
}


void Inc(char *mID)
{
	if (!MS[*mID].isSet)
	{
		if (*mID+3>MS[*mID].eid)
		{
			*mID = MS[*mID].eid + 1;
		}
		else
		{
			*mID = ((*mID - MS[*mID].bid)/3 + 1)*3+MS[*mID].bid+1;
		}
	}
	else
	{
		(*mID)++;
		if (MS[*mID].bid == *mID)
		{
			(*mID)++;
		}
	}
	if (*mID>MenuItemCount()) *mID = 1;
}

void MenuProc(unsigned char kState)
{
#ifdef KEY
	//kState = GetKey();	
#endif
	
	if (kState)
	{
		switch(kState)
		{
			case K_ESC: 
				iItemID = 0;
				ShowMenu(iItemID);
			break;
	
			case K_UP:
				if (iItemID)
				{
					Dec(&iItemID);
					ShowMenu(iItemID);
				}
			break;
	
			case K_DN: 
				if (iItemID)
				{
					Inc(&iItemID);
					ShowMenu(iItemID);
				}
			break;
	
			case K_OK:
				if (iItemID==0)
				{
					iItemID = 1;
				}
				else
				if (isSet(iItemID)==4)
				{
					iSetID = !iSetID;
				}
				ShowMenu(iItemID);
			break;
	
			case K_AT:
				ShowMenu(iItemID); 
			break;
	
			case K_OFF: 
				ShowMenu(iItemID);
			break;
		}
	}
	//kState = 0;
	///return miID;
}

char* STR(const char* s)
{
	static char b[17];
	char bi = 0;
	do
	{
		b[bi]=s[bi];
		bi++;
	}while(s[bi]);
	b[bi]=0;
	return b;
}
char* MenuInfo(unsigned char mID)
{
	switch(mID)
	{
		case 1: return WorkMode[IValue.Mode.BX.h]; break;
		case 2: return StartMode[IValue.Mode.BX.l]; break;
		case 3: return i2s(IValue.Status.BX.h); break;
		case 5: return i2s(BdRate[IValue.ComSet[0].BX.h]); break;
		case 6: return i2s(DataBits[IValue.ComSet[0].BX.l]); break;
		case 7: return i2s(IValue.ComSet[1].BX.h); break;
		case 8: return OddEvn[IValue.ComSet[1].BX.l]; break;
		case 10: return i2s(IValue.ACInV); break;
		case 11: return i2s(IValue.InvV); break;
		case 12: return i2s(IValue.LoadI); break;
		case 13: return i2s(IValue.Freq.BX.h); break;	//��������Ƶ��
		case 14: return i2s(IValue.Freq.BX.l); break;	//������Ƶ��
		case 15: return i2s(IValue.LoadRate); break;
	}
	return "0";
}

void ShowMenu(char mID) //����1
{
	if (mID<0 || mID>MenuItemCount()) mID = 0;
	if (mID==0)
	{
	    Welcome();
	    return ;
    }
	ClrScreen();
	cPrint12(1,1,0,30,MSTR_TITLE(mID),0);
	if (MSTR_FID(mID) <= MSTR_LID(mID)) 
	{
		cPrint12(0,0,18,0,MSTR(MSTR_FID(mID)),0);
		cPrint12(0,0,18,64,MenuInfo(MSTR_FID(mID)),(MSTR_FID(mID)==mID && isSet(mID))?1:0);
	}	
	if (MSTR_FID(mID)+1 <= MSTR_LID(mID)) 
	{
		cPrint12(0,0,34,0,MSTR(MSTR_FID(mID)+1),0);
		cPrint12(0,0,34,64,MenuInfo(MSTR_FID(mID)+1),(MSTR_FID(mID)+1==mID && isSet(mID))?1:0);
	}
	if (MSTR_FID(mID)+2 <= MSTR_LID(mID)) 
	{
		cPrint12(0,0,50,0,MSTR(MSTR_FID(mID)+2),0);
		cPrint12(0,0,50,64,MenuInfo(MSTR_FID(mID)+2),(MSTR_FID(mID)+2==mID && isSet(mID))?1:0);
	}
}

void Menu(unsigned char kState)
{
	if (RefreshMenuCnt>10000)
	{
		ShowMenu(iItemID);
		RefreshMenuCnt=0;
	}
	MenuProc(kState);
	RefreshMenuCnt++;
}
	

