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
#include "stm32f10x_lib.h"
#include "adc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define ADC1_DR_Address    ((u32)0x4001244C)
/* Private variables ---------------------------------------------------------*/
vu32 ADC_DualConvertedValueTab[ADC_CHANNEL_NUM];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void  DMA1_Init(void);

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
//  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable DMA Clock           */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

//  NVIC_InitStructure.NVIC_IRQChannel  = DMA1_Channel1_IRQChannel;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
    
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
  //DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);
  /* Enable DMA Channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);
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
  NVIC_InitTypeDef NVIC_InitStructure;

  DMA1_Init();
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel  = ADC1_2_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* ADC1 Configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//ADC_Mode_RegSimult;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//DISABLE;
  //ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
  ADC_TempSensorVrefintCmd(ENABLE);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_16,   1, ADC_SampleTime_239Cycles5);
  
  /* ADC2配置 */
//  ADC_InitStructure.ADC_Mode  = ADC_Mode_Independent;//ADC_Mode_RegSimult;
//  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
//  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
//  //ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2;
//  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
//  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
//  ADC_InitStructure.ADC_NbrOfChannel = 1;
//  ADC_Init(ADC2, &ADC_InitStructure);
//
//  ADC_RegularChannelConfig(ADC2, ADC_Channel_17, 1, ADC_SampleTime_1Cycles5);
  //ADC_ExternalTrigConvCmd(ADC2, ENABLE);  // 使能外部触发转换模式

  ADC_DMACmd(ADC1,ENABLE);

  ADC_Cmd(ADC1, ENABLE);
  ADC_ResetCalibration(ADC1);
  while(ADC_GetResetCalibrationStatus(ADC1));
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1));  

  ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
//  ADC_Cmd(ADC2, ENABLE);
//  ADC_ResetCalibration(ADC2);
//  while(ADC_GetResetCalibrationStatus(ADC2));
//  ADC_StartCalibration(ADC2);
//  while(ADC_GetCalibrationStatus(ADC2));

  

  //ADC_ExternalTrigConvCmd(ADC1, ENABLE);
  
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/* End of file ---------------------------------------------------------------*/
