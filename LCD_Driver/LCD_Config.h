
#include "REG52.h"

#define Dis_X_MAX		128-1
#define Dis_Y_MAX		64-1

//Define the MCU Register

sbit SPI_CS = P0^4;				//����CS����P1.0�˿�
sbit SPI_SDA = P0^5;			//����SDA����P1.1�˿�
sbit SPI_SCK = P0^6;			//����SCK����P1.2�˿�
sbit SPI_RES = P0^7;			//����RESET��P1.3�˿�


