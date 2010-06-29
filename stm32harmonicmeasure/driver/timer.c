/******************** (C) COPYRIGHT 2010 ***************************************
* File Name          : timer.c
* Date First Issued  : 06/29/10
* Description        : 
********************************************************************************
* History:
* 
* 
* 
* 06/29/10: V0.01     Created by heroxx
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_lib.h"
#include "timer.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : ADC_Sample_Frequency_Set
* Description    : 设定TIM2_CC2频率
* Input          : frequency :采样波形频率,默认为50Hz
*                  dotspercycle :每周期采样点数,默认为256,其他可选为512,1024
* Output         : None
* Return         : None
*******************************************************************************/
void  TIM2Init(u32 frequency,u32 dotspercycle)
{
  TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
  //TIM_OCInitTypeDef         TIM_OCInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel  = TIM2_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
//  TIM_OCInitTypeDef     TIM_OCInitStructure;
  TIM_Cmd(TIM2, DISABLE);//先停止TIM2时钟，以准备下面的设置

  if(frequency == 0 ) frequency = 5000;//50Hz
  if( (dotspercycle != 256 ) && (dotspercycle != 512 ) && (dotspercycle != 1024 )) dotspercycle =256;
  /* -----------------------------------------------------------------------
  TIM2 配置: 产生TIM2_CC2时钟控制信号用于控制ADC采样
  1.设定TIM预分频为72,则定时器每个滴答间隔时间为72/72M=1us
  2.
  ----------------------------------------------------------------------- */
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 1000; //计数值
  TIM_TimeBaseStructure.TIM_Prescaler = 35999; //预分频,此值+1,时钟滴答为72/72M=1us
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;//时钟分割
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  /* TIM_OCMode_Toggle Mode configuration: Channel2 */
//  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  TIM_OCInitStructure.TIM_Pulse = 10;
//  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
//  TIM_OC2Init(TIM2, &TIM_OCInitStructure);
//
//  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);

    //预先清除所有中断位防止一启用就有中断/
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

  //4个通道和溢出都配置中断/
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  /*---------------------------------------*/
  //TIM_ARRPreloadConfig(TIM2, ENABLE);
  /* TIM2 enable counter */
  TIM_Cmd(TIM2, ENABLE);
}





/* End of file ---------------------------------------------------------------*/
