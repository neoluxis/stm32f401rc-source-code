#ifndef _BSP_SPI_H_
#define _BSP_SPI_H_
#include "main.h"


u8 SPI1_WriteData(u8 *data,u16 size);
u8 SPI2_ReadWriteByte(u8 TxData);

#endif
