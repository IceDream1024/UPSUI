#ifndef __MENU__
#define __MENU__

typedef union _INT
{
	unsigned short x;
	struct {
		char h;
		char l;
	}BX;	
}INTX;

typedef struct
{
	int ACInV;
	int ACInI;
	int InvV;
	int LoadI;
	int BatV;
	int BatI;
	int Factor;
	int Power; //����
	int LoadRate; //������
	int ACInF;
	int InvF;
	int ChargeStatus;
}CValue;

typedef struct
{
	unsigned int ModbusSA;	//0
	unsigned int wMode;		//1
	unsigned int sMode;		//2	
	unsigned int BKLtime;	//3		
	unsigned int Password;	//4		
	unsigned int BaudRate;	//5		
	unsigned int DataBits;	//6		
	unsigned int StopBits;	//7		
	unsigned int OddEven;	//8		
	unsigned int StartCtrl;	//9		
	unsigned int ByPassCtrl;//10			
}SValue;

typedef struct _MenuItem
{
    char id;
    char *ItemStr;
    char ItemType;  
	//��3λ������ʾʱptr��Ӧ������
    //��CHAR,INTEGERΪ����,STRINGΪԤ������ַ�����
    //��5λ����3λΪMENUʱ,��5λΪ�˵�����(���31��)
    //		 ��3λ����MENUʱ,��3λΪ��������,��5λΪ������ʾ����
    void *ptr;
    //char mcnt;
} MenuItem;

typedef struct
{
    char stat;	//״̬
    char id;
    char bid; //backup id

    MenuItem *mm;
    MenuItem *sm;
} MENUPARAM;

extern MENUPARAM mr;

extern xdata SValue SetValue;
extern xdata CValue IValue;
extern xdata CValue Vadj;
extern bit SettingChanged;

extern void MenuDrv(void);
extern void LightOn(unsigned char Deg);
extern void LightOff();
extern void InitMenu();
extern bit ReadSetting();
extern void Menu(unsigned char key);

#endif
