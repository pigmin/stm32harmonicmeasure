/******************** (C) COPYRIGHT 2010 ***************************************
* File Name          : adc.c
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
#include "adc.h"

#include "stm32f10x_lib.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define ADC1_DR_Address    ((u32)0x4001244C)
/* Private variables ---------------------------------------------------------*/
vu32 ADC_DualConvertedValueTab[ADC_CHANNEL_NUM];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void  DMA1_Init(void);
void  ADC_Sample_Frequency_Set(u32 frequency);

/*******************************************************************************
* Function Name  : DMA1_Init
* Description    : Init DMA1
* Input          : - 
*                  - 
* Output         : None
* Return         : None
*******************************************************************************/
void  DMA1_Init(void)
{
  DMA_InitTypeDef DMA_InitStructure;

  /* Enable DMA Clock           */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
  /* DMA channel1 configuration ----------------------------------------------*/
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_DualConvertedValueTab;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = ADC_CHANNEL_NUM;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);

  /* Enabel DMA TC Interrupt */
  DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);
  /* Enable DMA Channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);
}

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
  TIM_OCInitTypeDef         TIM_OCInitStructure;
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
  TIM_TimeBaseStructure.TIM_Period = 10; //计数值
  TIM_TimeBaseStructure.TIM_Prescaler = 71; //预分频,此值+1,时钟滴答为72/72M=1us
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;//时钟分割
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  /* TIM_OCMode_Toggle Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 10;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OC2Init(TIM2, &TIM_OCInitStructure);

  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);

    //预先清除所有中断位防止一启用就有中断/
  TIM_ClearITPendingBit(TIM2, TIM_IT_CC2 |TIM_IT_Update);

  //4个通道和溢出都配置中断/
  TIM_ITConfig(TIM2, TIM_IT_CC2 |TIM_IT_Update, ENABLE);
  /*---------------------------------------*/
  //TIM_ARRPreloadConfig(TIM2, ENABLE);
  /* TIM2 enable counter */
  TIM_Cmd(TIM2, ENABLE);
}
/*******************************************************************************
* Function Name  : ADC_Init
* Description    : Init ADC Input, DMA Channel1, and ADC peripheral
* Input          : - 
*                  - 
* Output         : None
* Return         : None
*******************************************************************************/
void ADCInit( void )
{
  ADC_InitTypeDef ADC_InitStructure;

  DMA1_Init();
  TIM2Init(5000,512);

  /* ADC1 Configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;//DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//DISABLE;
  //ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  ADC_RegularChannelConfig(ADC1, ADC_Channel_16,   1, ADC_SampleTime_1Cycles5);
  
  /* ADC2配置 */
  ADC_InitStructure.ADC_Mode  = ADC_Mode_RegSimult;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  //ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC2, &ADC_InitStructure);

  ADC_RegularChannelConfig(ADC2, ADC_Channel_17, 1, ADC_SampleTime_1Cycles5);
  //ADC_ExternalTrigConvCmd(ADC2, ENABLE);  // 使能外部触发转换模式

  ADC_Cmd(ADC1, ENABLE);
  ADC_ResetCalibration(ADC1);
  while(ADC_GetResetCalibrationStatus(ADC1));
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1));  
  
  ADC_Cmd(ADC2, ENABLE);
  ADC_ResetCalibration(ADC2);
  while(ADC_GetResetCalibrationStatus(ADC2));
  ADC_StartCalibration(ADC2);
  while(ADC_GetCalibrationStatus(ADC2));

  //ADC_ExternalTrigConvCmd(ADC1, ENABLE);

  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/* End of file ---------------------------------------------------------------*/
