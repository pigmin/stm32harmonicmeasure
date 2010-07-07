/******************** (C) COPYRIGHT 2010 Heroxx ***************************************
* File Name          : rcc_config.c
* Date First Issued  : 06/21/10
* Description        : 
********************************************************************************
* History:
* 
* 
* 
* 06/21/10: V0.01     Created by heroxx
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "rcc_config.h"

#include "stm32f10x_lib.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration(void)
{   
  ErrorStatus HSEStartUpStatus = ERROR;
  
  while(HSEStartUpStatus == ERROR)
  {
    /* RCC system reset(for debug purpose) */
    RCC_DeInit();
  
    /* Enable HSE */
    RCC_HSEConfig(RCC_HSE_ON);
  
    /* Wait till HSE is ready */
    HSEStartUpStatus = RCC_WaitForHSEStartUp();
  
    if(HSEStartUpStatus == SUCCESS)
    {
      /* Enable Prefetch Buffer (‘§»°÷∏ª∫¥Ê)*/
      FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
  
      /* Flash 2 wait state (FLASH¥Ê¥¢∆˜—” ± º÷’)*/
      FLASH_SetLatency(FLASH_Latency_2);
   	
      /* HCLK = SYSCLK (AHB ±÷”)*/
      RCC_HCLKConfig(RCC_SYSCLK_Div1); 
    
      /* PCLK2 = HCLK (APB2 ±÷”)*/
      RCC_PCLK2Config(RCC_HCLK_Div1); 
  
      /* PCLK1 = HCLK/2 (APB ±÷”)*/
      RCC_PCLK1Config(RCC_HCLK_Div2);

      /* ADCCLK = PCLK2/8 ()*/
      RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
      
      /* PLLCLK = 8MHz * 7 = 56 MHz */
      RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_7);
  
      /* Enable PLL */ 
      RCC_PLLCmd(ENABLE);
  
      /* Wait till PLL is ready */
      while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
      {
      }
  
      /* Select PLL as system clock source */
      RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
  
      /* Wait till PLL is used as system clock source */
      while(RCC_GetSYSCLKSource() != 0x08)
      {
      }
    }
  }

  /* Enable GPIOA, GPIOC and AFIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, ENABLE);
  //RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN, ENABLE);  
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_ALL,ENABLE);
  //RCC_APB1PeriphClockCmd(RCC_APB1Periph_ALL, ENABLE);
}

/* End of file ---------------------------------------------------------------*/
