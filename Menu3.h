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
	unsigned int ModbusSA;
	unsigned int wMode;
	unsigned int sMode;
	unsigned int BKLtime;
	unsigned int Password;
	unsigned int BaudRate;
	unsigned int DataBits;
	unsigned int StopBits;
	unsigned int OddEven;
	unsigned int StartCtrl;
	unsigned int ByPassCtrl;
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
