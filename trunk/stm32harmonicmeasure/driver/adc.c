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
  DMA_InitTypeDef DMA_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;
  
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
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  /* Enable DMA Channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);
   
  
  /* ADC1 Configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//ADC_Mode_RegSimult;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  ADC_RegularChannelConfig(ADC1, ADC_Channel_16,   1, ADC_SampleTime_239Cycles5);
     
  
  /* ADC2配置 */
  ADC_InitStructure.ADC_Mode  = ADC_Mode_Independent;//ADC_Mode_RegSimult;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC2, &ADC_InitStructure);
  ADC_RegularChannelConfig(ADC2, ADC_Channel_17, 1, ADC_SampleTime_239Cycles5);
   
  ADC_Cmd(ADC1, ENABLE); 
  ADC_TempSensorVrefintCmd(ENABLE);
    
  ADC_ResetCalibration(ADC1);
  while(ADC_GetResetCalibrationStatus(ADC1));
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1));  
  
  ADC_Cmd(ADC2, ENABLE);
  ADC_ResetCalibration(ADC2);
  while(ADC_GetResetCalibrationStatus(ADC2));
  ADC_StartCalibration(ADC2);
  while(ADC_GetCalibrationStatus(ADC2));
}

/*******************************************************************************
* Function Name  : ADCStart
* Description    : Start ADC to continous convert
* Input          : None 
*                  - 
* Output         : None
* Return         : None
*******************************************************************************/
void ADCStart( void )
{
  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  ADC_SoftwareStartConvCmd(ADC2, ENABLE);
}

/*******************************************************************************
* Function Name  : ADCStop
* Description    : Stop ADC
* Input          : None 
*                  - 
* Output         : None
* Return         : None
*******************************************************************************/
void ADCStop( void )
{
  /* Stop ADC1 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC1, DISABLE);
}

/*******************************************************************************
* Function Name  : ADC_GetValue
* Description    : Return specified ADC channel value
* Input          : index - channel index 
*                  - 
* Output         : None
* Return         : adc value
*******************************************************************************/
u32 ADC_GetValue(int index)
{
  if( (index >= 0) && (index < ADC_CHANNEL_NUM) )
  {
    return (u16)ADC_DualConvertedValueTab[index];
  }
  else
  {
    return 0;
  }
}

/*******************************************************************************
* Function Name  : ADC_GetVoltage
* Description    : Get ADC result in mV 
* Input          : index - channel index 
*                  - 
* Output         : None
* Return         : voltage in mV
*******************************************************************************/
u16 ADC_GetVoltage(int index)
{
  u32 ltemp;
  
  if( (index >= 0) && (index < (ADC_CHANNEL_NUM - 2) ) )
  {
    ltemp = (u32)ADC_GetValue(index) * 1200;          //内部REF典型电压值为1.2V
    ltemp = ltemp / ADC_GetValue(ADC_REF);
    
    return (u16)ltemp;
  }
  
  return (u16)0;
}

/*******************************************************************************
* Function Name  : ADC_GetTemperature
* Description    : Get temperature sensor value in .C
* Input          : - 
*                  - 
* Output         : None
* Return         : None
*******************************************************************************/
u16 ADC_GetTemperature( void )
{
  u32 ltemp;
  
  ltemp = (u32)ADC_GetValue(ADC_TEMP) * 1200;
  ltemp = ltemp / ADC_GetValue(ADC_REF);
  
  ltemp = (1420 - ltemp) / 4.35 + 25;
//  ltemp = (1400 - ltemp) / 4.478 + 25;  //数据手册值
  
  return (u16)ltemp;
}


/* End of file ---------------------------------------------------------------*/
