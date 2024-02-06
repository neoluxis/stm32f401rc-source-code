#ifndef _BSP_USB_H_
#define	_BSP_USB_H_

#include "main.h"
#include "usbd_cdc_if.h"
#include "string.h"	
#include "stdarg.h"		 
#include "stdio.h"	

void usb_printf(char* fmt,...);
uint16_t VCP_DataRx (uint8_t* Buf, uint32_t Len);

#endif
