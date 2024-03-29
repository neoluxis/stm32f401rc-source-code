/**
  ****************************(C) COPYRIGHT 2021 Boring_TECH*********************
  * @file       BSP_spi.c/h
  * @brief      将HAL库SPI函数进行二次封装，给相应的器件进行调用
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
  * @brief          封装SPI6写函数
  * @param[in]     	data 发出的数据
  * @retval         收到的数据
  */
u8 SPI1_WriteData(u8 *data,u16 size)
{
	return HAL_SPI_Transmit(&hspi1,data,size, 1000);
}

/**
  * @brief          封装SPI2读写函数（片外SPI Flash使用）
  * @param[in]     	TxData 发出的数据
  * @retval         RxData 收到的数据
  */
u8 SPI2_ReadWriteByte(u8 TxData)
{
    u8 Rxdata;
    HAL_SPI_TransmitReceive(&hspi2,&TxData,&Rxdata,1, 1000);       
 	return Rxdata;          		    //返回收到的数据		
}





