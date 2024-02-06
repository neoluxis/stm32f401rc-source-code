/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "sdio.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "BSP_GPIO.h"
#include "BSP_USART.h"
#include "BSP_spi.h"
#include "stdio.h"	
#include "tftlcd.h"
#include "w25qxx.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
	char str[50] = {0};
	u8 t;
	
HAL_SD_CardInfoTypeDef  SDCardInfo;

void printf_sdcard_info(void)
{
	uint64_t CardCap;      	//SD卡容量
	HAL_SD_CardCIDTypeDef SDCard_CID; 

	HAL_SD_GetCardCID(&hsd,&SDCard_CID);	//获取CID
	HAL_SD_GetCardInfo(&hsd,&SDCardInfo);                    //获取SD卡信息
	CardCap=(uint64_t)(SDCardInfo.LogBlockNbr)*(uint64_t)(SDCardInfo.LogBlockSize);	//计算SD卡容量
	switch(SDCardInfo.CardType)
	{
		case CARD_SDSC:
		{
			if(SDCardInfo.CardVersion == CARD_V1_X)
				printf("Card Type:SDSC V1\r\n");
			else if(SDCardInfo.CardVersion == CARD_V2_X)
				printf("Card Type:SDSC V2\r\n");
		}
		break;
		case CARD_SDHC_SDXC:printf("Card Type:SDHC\r\n");break;
		default:break;
	}	
		
  printf("Card ManufacturerID: %d \r\n",SDCard_CID.ManufacturerID);				//制造商ID	
 	printf("CardVersion:         %d \r\n",(uint32_t)(SDCardInfo.CardVersion));		//卡版本号
	printf("Class:               %d \r\n",(uint32_t)(SDCardInfo.Class));		    //
 	printf("Card RCA(RelCardAdd):%d \r\n",SDCardInfo.RelCardAdd);					//卡相对地址
	printf("Card BlockNbr:       %d \r\n",SDCardInfo.BlockNbr);						//块数量
 	printf("Card BlockSize:      %d \r\n",SDCardInfo.BlockSize);					//块大小
	printf("LogBlockNbr:         %d \r\n",(uint32_t)(SDCardInfo.LogBlockNbr));		//逻辑块数量
	printf("LogBlockSize:        %d \r\n",(uint32_t)(SDCardInfo.LogBlockSize));		//逻辑块大小
	printf("Card Capacity:       %d MB\r\n",(uint32_t)(CardCap>>20));				//卡容量
}
	
/**
  * @brief  compares two buffers.
  * @param  pbuffer1, pbuffer2: buffers to be compared.
  * @param  buffer_length: buffer's length
  * @retval 1: pbuffer1 identical to pbuffer2
  *         0: pbuffer1 differs from pbuffer2
  */
uint8_t buffer_compare(uint8_t* pbuffer1, uint8_t* pbuffer2, uint16_t buffer_length)
{
  while(buffer_length--)
  {
    if(*pbuffer1 != *pbuffer2)
    {
      return 0;
    }
    pbuffer1++;
    pbuffer2++;
  }
  return 1;
}

  const char wbuf[] = "BORING-TECH STM32 TEST";
  char rbuf[50];

  const char wbuf1[] = "BORING-TECH STM32 TEST1";
  char rbuf1[20];
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	u8 color = 0;
	u32 flash_size;
	u8 sd_datus;
	u8 sd_buf;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_SPI1_Init();
  MX_SDIO_SD_Init();
  MX_SPI2_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
	LCD_Init();
	LCD_Clear(BLACK); 		//清屏	
	BACK_COLOR=BLACK;
	POINT_COLOR=CYAN;	
	LCD_ShowString(0, 0, 160, 12, 12, "Boring_TECH");
	LCD_ShowString(0, 15, 160, 12, 12, "TFTLCD TEST 240*135");

	LCD_ShowString(0, 30, 160, 16, 12, "STM32F401/402");
	LCD_ShowString(0, 45, 160, 12, 12, "2019/6/24");
//	
	W25QXX_Init();
	while(W25QXX_ReadID()!=W25Q64)								//检测不到W25Q
	{
			HAL_Delay(100);
			HAL_GPIO_TogglePin(RED_GPIO_Port,RED_Pin);
	}		
	printf("SPI Ready \r\n");

	flash_size=8*1024*1024;	//FLASH 大小为4M字节		
	
	W25QXX_Write((u8*)wbuf,flash_size-100,sizeof(wbuf));	
	W25QXX_Read((u8*)rbuf,flash_size-100,sizeof(wbuf));
  while(1 != buffer_compare((uint8_t*)rbuf, (uint8_t*)wbuf, sizeof(wbuf))){
			HAL_Delay(500);
			HAL_GPIO_TogglePin(RED_GPIO_Port,RED_Pin);
  }	
	printf("W25Qxx data test ok.\r\n");
//	
	SD_Driver.disk_initialize(0);
	printf_sdcard_info();	
////	
	sd_datus = SD_Driver.disk_write(0,(uint8_t*)wbuf1,20,2);
	printf("sd write content:\r\n%s\r\n",(uint8_t*)wbuf1); 
	sd_datus = SD_Driver.disk_read(0,&sd_buf,20,2);
	printf("sd reak result:%d\r\n",sd_datus);
	printf("sd read content:\r\n%s\r\n",&sd_buf);;	
  while(1 != buffer_compare(&sd_buf, (uint8_t*)wbuf1, sizeof(wbuf1))){
		HAL_Delay(500);
		LED_R_TogglePin;
		LED_G_TogglePin;
		LED_B_TogglePin;
  }		
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		switch(color%7)
		{
			case 0:
				LED_R(0);
				LED_G(1);
				LED_B(1);
				break;
			case 1:
				LED_R(1);
				LED_G(0);
				LED_B(1);
				break;
			case 2:
				LED_R(1);
				LED_G(1);
				LED_B(0);
				break;
			case 3:
				LED_R(0);
				LED_G(0);
				LED_B(1);
				break;
			case 4:
				LED_R(0);
				LED_G(1);
				LED_B(0);
				break;
			case 5:
				LED_R(1);
				LED_G(0);
				LED_B(0);
				break;
			case 6:
				LED_R(0);
				LED_G(0);
				LED_B(0);
				break;
			default:
				break;
		}
		color++;		

		sprintf(str,"%4dABCD",t);
		LCD_ShowString(0, 60, 240, 12, 12, str);
		t++;
		
		HAL_Delay(1000);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
