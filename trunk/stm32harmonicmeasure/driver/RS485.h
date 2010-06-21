/******************** (C) COPYRIGHT 2008 ***************************************
* File Name          : RS485.h
* Date First Issued  : 06/21/10
* Description        : This file contains all the functions prototypes for the
*                      RS485 library.
********************************************************************************
* History:
* 
* 
* 
* 06/21/10: V0.01    Created by heroxx
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RS485_H
#define __RS485_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_map.h"

#include "stdio.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void RS485_Configuration(u32 baudrate, u16 parity);
void RS485Cmd(bool bRxEnable, bool bTxEnable);

int fputc(int ch, FILE *f);

#endif /*__RS485_H */

/* End of file ---------------------------------------------------------------*/
