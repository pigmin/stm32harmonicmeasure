/******************** (C) COPYRIGHT 2010 ***************************************
* File Name          : led.h
* Date First Issued  : 06/21/10
* Description        : This file contains all the functions prototypes for the
*                      Led library.
********************************************************************************
* History:
* 
* 
* 
* 06/21/10: V0.01
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LED_H
#define __LED_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_map.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Led_init(void);
void Led_On(u8 num);
void Led_Off(u8 num);
void Led_Flash(u8 num);
#endif /*__LED_H */

/* End of file ---------------------------------------------------------------*/
