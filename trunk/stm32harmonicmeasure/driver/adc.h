/******************** (C) COPYRIGHT 2008 ***************************************
* File Name          : adc.h
* Date First Issued  : 06/21/10
* Description        : This file contains all the functions prototypes for the
*                      adc library.
********************************************************************************
* History:
* 
* 
* 
* 06/21/10: V0.01    Created by heroxx
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __adc_H
#define __adc_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_map.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define ADC_CHANNEL_NUM     256

#define ADC_PT100_A         0     // X8
#define ADC_PT100_B         1     // X9
#define ADC_IAP             2     // X5 Ia+
#define ADC_IAN             3     // X5 Ia-
#define ADC_IBP             4     // X6 Ib+
#define ADC_IBN             5     // X6 Ib-
#define ADC_ICP             6     // X7 Ic+
#define ADC_ICN             7     // X7 Ic-
#define ADC_AVG_IL          8     // Average IL
#define ADC_TEMP            9     // Temperature sensor
#define ADC_REF             10    // Internal Ref

/* Exported functions ------------------------------------------------------- */
void ADCInit( void );

#endif /*__adc_H */

/**************END OF FILE****/
