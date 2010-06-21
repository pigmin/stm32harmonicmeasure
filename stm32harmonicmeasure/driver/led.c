/******************** (C) COPYRIGHT 2010 ***************************************
* File Name          : led.c
* Date First Issued  : 06/21/10
* Description        : 
********************************************************************************
* History:
* 
* 
* 
* 06/21/10: V0.01 heroxx
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "led.h"

#include "stm32f10x_lib.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Led_init
* Description    : 
* Input          : config the OverVoltage as Input,and the LED as Output 
*                  - 
* Output         : None
* Return         : None
*******************************************************************************/
void Led_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
   
  /* Configure PD2 and PD3 as Output push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  GPIO_SetBits(GPIOD, GPIO_Pin_7);
  GPIO_SetBits(GPIOD, GPIO_Pin_6);
}

/*******************************************************************************
* Function Name  : Led_On
* Description    : 
* Input          : to set the LED on 
*                  - 
* Output         : None
* Return         : None
*******************************************************************************/
void Led_On(u8 num)
{
  switch(num)
  {
    case 0:
      GPIO_ResetBits(GPIOD, GPIO_Pin_7);
      break;
      
    case 1:
      GPIO_ResetBits(GPIOD, GPIO_Pin_6);
      break;
      
    default:
      break;
  }
}

/*******************************************************************************
* Function Name  : Led_Off
* Description    : 
* Input          : to set the LED off 
*                  - 
* Output         : None
* Return         : None
*******************************************************************************/
void Led_Off(u8 num)
{
  switch(num)
  {
    case 0:
      GPIO_SetBits(GPIOD, GPIO_Pin_7);
      break;
      
    case 1:
      GPIO_SetBits(GPIOD, GPIO_Pin_6);
      break;
      
    default:
      break;
  }
}

/* End of file ---------------------------------------------------------------*/
