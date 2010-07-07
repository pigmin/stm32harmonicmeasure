/**
  ******************************************************************************
  * @file    main.c 
  * @author  heroxx
  * @version V1.0.0
  * @date    06/20/2010
  * @brief   Main program body
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 HEROXX</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_lib.h"

#include "rcc_config.h"
#include "RS485.h"
#include "led.h"
#include "adc.h"
#include "timer.h"

#define ADC1_DR_Address    ((u32)0x4001244C)
ADC_InitTypeDef ADC_InitStructure;
DMA_InitTypeDef DMA_InitStructure;

extern vu32  ADC_Value[ADC_CHANNEL_NUM];
extern vu8 flag_adcover;
u32 counter=0;

u32 ltemp=0;
s16 Temp=0;
s16 GetTemp(u16 advalue);
void NVIC_Configuration(void);

int main()
{
  RCC_Configuration();
  NVIC_Configuration();
  Led_init();
  //RS485_Configuration(115200,USART_Parity_Even);
  
  ADCInit();
  
  TIM2Init(5000,1024);

  while(1)
  {
    if(flag_adcover==1)
    {
      flag_adcover  = 0;
//      printf("!\r\n");
      for(counter=0;counter<ADC_CHANNEL_NUM;counter++)
      {
        printf("%d\t%d\n",ADC_Value[counter]>>16,ADC_Value[counter]&0xFFFF);
      }
      //GPIOD->ODR ^= ((u32)1<<6);
    }
  }


}

s16 GetTemp(u16 advalue)
{
	u32 Vtemp_sensor;
	s32 Current_Temp;
	
	Vtemp_sensor	= advalue*330/4096;
	Current_Temp	= (s32)(143-Vtemp_sensor)*10000/43 +2500;
	return (s16)Current_Temp;	
}

void NVIC_Configuration(void)
{
#ifdef  VECT_TAB_RAM  
  /* Set the Vector Table base location at 0x20000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif
}

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
