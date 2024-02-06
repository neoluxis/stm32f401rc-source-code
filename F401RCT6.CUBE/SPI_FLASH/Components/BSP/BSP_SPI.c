/**
  ****************************(C) COPYRIGHT 2021 Boring_TECH*********************
  * @file       Drv_spi.c/h
  * @brief      ��HAL��SPI�������ж��η�װ������Ӧ���������е���
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
#include "BSP_spi.h"
#include "spi.h"

/**
  * @brief          ��װSPI2��д������Ƭ��SPI Flashʹ�ã�
  * @param[in]     	TxData ����������
  * @retval         RxData �յ�������
  */
u8 SPI2_ReadWriteByte(u8 TxData)
{
    u8 Rxdata;
    HAL_SPI_TransmitReceive(&hspi2,&TxData,&Rxdata,1, 1000);       
 	return Rxdata;          		    //�����յ�������		
}






