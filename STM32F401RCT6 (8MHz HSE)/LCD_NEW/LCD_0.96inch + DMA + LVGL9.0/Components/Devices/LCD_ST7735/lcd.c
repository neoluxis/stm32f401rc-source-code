#include "lcd.h"
#include "font.h"
#include "spi.h"

//SPI��ʾ���ӿ�
//LCD_RST
#define LCD_RST_SET     HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_SET) 
#define LCD_RST_RESET   HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_RESET)
//LCD_DC 
#define LCD_DC_SET      HAL_GPIO_WritePin(LCD_DC_GPIO_Port,LCD_DC_Pin,GPIO_PIN_SET) 
#define LCD_DC_RESET    HAL_GPIO_WritePin(LCD_DC_GPIO_Port,LCD_DC_Pin,GPIO_PIN_RESET)

#define SPI_Drv (&hspi1)


static int32_t lcd_init(void);
static int32_t lcd_gettick(void);
static int32_t lcd_writereg(uint8_t reg,uint8_t* pdata,uint32_t length);
static int32_t lcd_readreg(uint8_t reg,uint8_t* pdata);
static int32_t lcd_senddata(uint8_t* pdata,uint32_t length);
static int32_t lcd_recvdata(uint8_t* pdata,uint32_t length);

ST7735_IO_t st7735_pIO = {
	lcd_init,
	NULL,
	NULL,
	lcd_writereg,
	lcd_readreg,
	lcd_senddata,
	lcd_recvdata,
	lcd_gettick
};
ST7735_Object_t st7735_pObj;
uint32_t st7735_id;

void LCD_Test(void)
{
	uint8_t text[20];
	ST7735_RegisterBusIO(&st7735_pObj,&st7735_pIO);
	ST7735_LCD_Driver.Init(&st7735_pObj,ST7735_FORMAT_RBG565,ST7735_ORIENTATION_LANDSCAPE_ROT180);
	ST7735_LCD_Driver.ReadID(&st7735_pObj,&st7735_id);
	
	ST7735_LCD_Driver.FillRect(&st7735_pObj,0,0,160,80,BLACK);
	
	sprintf((char *)&text,"LCD TEST");
	LCD_ShowString(4,4,160,16,16,text);
	sprintf((char *)&text,"STM32H7xx 0x%X",HAL_GetDEVID());
	LCD_ShowString(4,22,160,16,16,text);
	sprintf((char *)&text,"LCD ID: 0x%X",st7735_id);
	LCD_ShowString(4,40,160,16,16,text);

}

uint16_t POINT_COLOR=0xFFFF;	//������ɫ
uint16_t BACK_COLOR=BLACK;  //����ɫ 
/**
 * @brief	��ʾһ��ASCII���ַ�
 *
 * @param   x,y		��ʾ��ʼ����
 * @param   chr		��Ҫ��ʾ���ַ�
 * @param   size	�����С(֧��16/24/32������)
 * @param   mode	���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
 *
 * @return  void
 */
void LCD_ShowChar(uint16_t x, uint16_t y ,uint8_t chr ,uint8_t size ,uint8_t mode)
{  							  
  uint8_t temp,t1,t;
	uint16_t y0=y;
	uint16_t x0=x;
	uint16_t colortemp=POINT_COLOR; 
  uint32_t h,w;
	
	uint16_t write[size][size==12?6:8];
	uint16_t count;
	
  ST7735_GetXSize(&st7735_pObj,&w);
	ST7735_GetYSize(&st7735_pObj,&h);
	
	//���ô���		   
	chr=chr-' ';//�õ�ƫ�ƺ��ֵ
	count = 0;
	
	if(!mode) //�ǵ��ӷ�ʽ
	{
		for(t=0;t<size;t++)
		{   
			if(size==12)temp=asc2_1206[chr][t];  //����1206����
			else temp=asc2_1608[chr][t];		 //����1608����
			
			for(t1=0;t1<8;t1++)
			{			    
				if(temp&0x80)
					POINT_COLOR=(colortemp&0xFF)<<8|colortemp>>8;
				else 
					POINT_COLOR=(BACK_COLOR&0xFF)<<8|BACK_COLOR>>8;
				
				write[count][t/2]=POINT_COLOR;
				count ++;
				if(count >= size) count =0;
				
				temp<<=1;
				y++;
				if(y>=h){POINT_COLOR=colortemp;return;}//��������
				if((y-y0)==size)
				{
					y=y0;
					x++;
					if(x>=w){POINT_COLOR=colortemp;return;}//��������
					break;
				}
			}
		}
	}
	else//���ӷ�ʽ
	{
		for(t=0;t<size;t++)
		{   
			if(size==12)temp=asc2_1206[chr][t];  //����1206����
			else temp=asc2_1608[chr][t];		 //����1608���� 	                          
			for(t1=0;t1<8;t1++)
			{			    
				if(temp&0x80)
					write[count][t/2]=(POINT_COLOR&0xFF)<<8|POINT_COLOR>>8;
				count ++;
				if(count >= size) count =0;
				
				temp<<=1;
				y++;
				if(y>=h){POINT_COLOR=colortemp;return;}//��������
				if((y-y0)==size)
				{
					y=y0;
					x++;
					if(x>=w){POINT_COLOR=colortemp;return;}//��������
					break;
				}
			}  	 
		}     
	}
	ST7735_FillRGBRect(&st7735_pObj,x0,y0,(uint8_t *)&write,size==12?6:8,size); 
	POINT_COLOR=colortemp;	     
}   
/**
 * @brief	��ʾ�ַ���
 *
 * @param   x,y		�������
 * @param   width	�ַ���ʾ������
 * @param   height	�ַ���ʾ����߶�
 * @param   size	�����С
 * @param   p		�ַ�����ʼ��ַ
 *
 * @return  void
 */
void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p)
{         
	uint8_t x0 = x;
	width += x;
	height += y;
    while((*p <= '~') && (*p >= ' ')) //�ж��ǲ��ǷǷ��ַ�!
    {
        if(x >= width)
        {
            x = x0;
            y += size;
        }

        if(y >= height)break; //�˳�

        LCD_ShowChar(x, y, *p, size, 0);
        x += size / 2;
        p++;
    }  
}

static int32_t lcd_init(void)
{
	int32_t result = ST7735_OK;
	return result;
}

static int32_t lcd_gettick(void)
{
	return HAL_GetTick();
}

static int32_t lcd_writereg(uint8_t reg,uint8_t* pdata,uint32_t length)
{
	int32_t result;
	LCD_DC_RESET;
	result = HAL_SPI_Transmit_DMA(SPI_Drv,&reg,1);
  while (HAL_SPI_GetState(SPI_Drv) != HAL_SPI_STATE_READY)
  {
  } 	
	LCD_DC_SET;
	if(length > 0)
		result += HAL_SPI_Transmit_DMA(SPI_Drv,pdata,length);
  while (HAL_SPI_GetState(SPI_Drv) != HAL_SPI_STATE_READY)
  {
  } 	
	result /= -result;
	return result;
}

static int32_t lcd_readreg(uint8_t reg,uint8_t* pdata)
{
	int32_t result;
	LCD_DC_RESET;
	result = HAL_SPI_Transmit_DMA(SPI_Drv,&reg,1);
  while (HAL_SPI_GetState(SPI_Drv) != HAL_SPI_STATE_READY)
  {
  } 	
	LCD_DC_SET;
	result += HAL_SPI_Receive_DMA(SPI_Drv,pdata,1);
  while (HAL_SPI_GetState(SPI_Drv) != HAL_SPI_STATE_READY)
  {
  } 	
	result /= -result;
	return result;
}

static int32_t lcd_senddata(uint8_t* pdata,uint32_t length)
{
	int32_t result;
	result =HAL_SPI_Transmit_DMA(SPI_Drv,pdata,length);
  while (HAL_SPI_GetState(SPI_Drv) != HAL_SPI_STATE_READY)
  {
  } 	
	result /= -result;
	return result;
}

static int32_t lcd_recvdata(uint8_t* pdata,uint32_t length)
{
	int32_t result;
	result = HAL_SPI_Receive_DMA(SPI_Drv,pdata,length);
  while (HAL_SPI_GetState(SPI_Drv) != HAL_SPI_STATE_READY)
  {
  } 	
	result /= -result;
	return result;
}

