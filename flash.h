#ifndef __FLASHHFILE__
#define __FLASHHFILE__

extern unsigned char Read_Byte(unsigned int addr);//addr��0��ʼ
extern void Erase_Section(unsigned int addr);
extern void Write_Byte(unsigned int addr,unsigned char ch);//addr��0��ʼ
extern EEROM_BASE_ADDR;

#endif //__FLASHHFILE__
