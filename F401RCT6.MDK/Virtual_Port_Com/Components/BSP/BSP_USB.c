/**
  ****************************(C) COPYRIGHT 2021 Boring_TECH*********************
  * @file       BSP_USB.c/h
  * @brief      
  * @note      	
  * @history
  *  Version    Date            Author          Modification
  *  V3.0.0     2020.7.14     	              	1. done
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2021 Boring_TECH*********************
  */
#include "BSP_USB.h"

extern uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];


/**
  * @brief          usb���⴮��,printf ����
  * @param[in]     	char* fmt,...
  * @retval         none
  */
void usb_printf(char* fmt,...)  
{  
	u16 i;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)UserTxBufferFS,fmt,ap);
	va_end(ap);
	i=strlen((const char*)UserTxBufferFS);     //�˴η������ݵĳ���
  CDC_Transmit_FS(UserTxBufferFS,i);              //��������
}

/**
	* @brief          usb���⴮�ڣ�ͨ�����⴮�ڷ��ͽ��յ�������
										��Ҫ��CDC_Receive_FS�����е���
  * @param[in]     	uint8_t* Buf 
										uint32_t Len
  * @retval         USBD_OK
  */
uint16_t VCP_DataRx (uint8_t* Buf, uint32_t Len)
{
	CDC_Transmit_FS(Buf,Len); 
	return USBD_OK;
}


