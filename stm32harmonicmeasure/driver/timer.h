/******************** (C) COPYRIGHT 2010 ***************************************
* File Name          : Timer.h
* Date First Issued  : 06/21/10
* Description        : This file contains all the functions prototypes for the
*                      Timer library.
********************************************************************************
* History:
* 
* 
* 
* 06/29/10: V0.01    Created by heroxx
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIMER_H
#define __TIMER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_map.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void  TIM2Init(u32 frequency,u32 dotspercycle);


#endif /*__TIMER_H */

/* End of file ---------------------------------------------------------------*/

