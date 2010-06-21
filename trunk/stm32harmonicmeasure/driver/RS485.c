/******************** (C) COPYRIGHT 2010 ***************************************
* File Name          : RS485.c
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
#include "RS485.h"

#include "stm32f10x_lib.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
unsigned char rString = 0x00;

/* Private function prototypes -----------------------------------------------*/
void SendMode(bool bSend);

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : SendMode
* Description    : 
* Input          : bSend  true - set RS485 in send mode, else in rece mode 
*                  - 
* Output         : None
* Return         : None
*******************************************************************************/
void SendMode(bool bSend)
{
  if(bSend)
  {
    GPIO_SetBits(GPIOA, GPIO_Pin_8);
  }
  else
  {
    GPIO_ResetBits(GPIOA, GPIO_Pin_8);  
  }
}

/*******************************************************************************
* Function Name  : RS485_Configuration
* Description    : Init uart2 using PA2, PA3 pins
*                  9600, 8, 1, none flow control
*                  DE/nRE using PA0
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RS485_Configuration(u32 baudrate, u16 parity)
{ 
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  USART_ClockInitTypeDef	USART_ClockInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  USART_DeInit(USART1);
  
  #ifdef  VECT_TAB_RAM  
  /* Set the Vector Table base location at 0x20000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
  #else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
  #endif
  
  /* Enable the USART1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Config DE/nRE */
  /* Configure (PA.08) as Output push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* Configure USART1 Tx (PA.09) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* Configure USART1 Rx (PA.10) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  
  /* USARTx configuration ------------------------------------------------------*/
  /* USARTx configured as follow:
        - BaudRate = 9600 baud
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
        - USART Clock disabled
        - USART CPOL: Clock is active low
        - USART CPHA: Data is captured on the second edge 
        - USART LastBit: The clock pulse of the last data bit is not output to 
                         the SCLK pin
  */
  USART_InitStructure.USART_BaudRate = baudrate;
  USART_InitStructure.USART_WordLength = USART_WordLength_9b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  
  if( (parity != USART_Parity_Even) && (parity != USART_Parity_Odd) )
  {
    parity = USART_Parity_No;
  }
  USART_InitStructure.USART_Parity = parity ;
  
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_ClockInitStructure.USART_Clock	= USART_Clock_Disable;
  USART_ClockInitStructure.USART_CPOL		= USART_CPOL_Low;
  USART_ClockInitStructure.USART_CPHA		= USART_CPHA_2Edge;
  USART_ClockInitStructure.USART_LastBit	= USART_LastBit_Disable;

  
  /* Configure the USART1 */
  USART_Init(USART1, &USART_InitStructure);
  USART_ClockInit(USART1,&USART_ClockInitStructure);

  /* Enable the USART Transmoit interrupt: this interrupt is generated when the 
   USART1 transmit data register is empty */  
//  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);

  /* Enable the USART Receive interrupt: this interrupt is generated when the 
   USART1 receive data register is not empty */
  //USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  
  /* Enable the USART1 */
  USART_Cmd(USART1, ENABLE);

  /* Send RS485 in Rece Mode */
  //SendMode(FALSE);
  SendMode(TRUE);
}

/*******************************************************************************
* Function Name  : RS485Cmd
* Description    : Control RS485 Tx and Rx
* Input          : bRxEnable  
*                  bTxEnable 
* Output         : None
* Return         : None
*******************************************************************************/
void RS485Cmd(bool bRxEnable, bool bTxEnable)
{
  if(bRxEnable)
  {
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
    {
    }
    
    SendMode(FALSE); 
    USART_Cmd(USART1, ENABLE);
  }
  else if(bTxEnable)
  {
    SendMode(TRUE); 
    USART_Cmd(USART1, ENABLE);
    
    /* Force to Generate a TXE interrupt */
    /* USART1 transmit data register is empty */  
    USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
  }
  else
  {
    SendMode(FALSE); 
    USART_Cmd(USART1, DISABLE);
  }
}
/*******************************************************************************
* Function Name  : fputc
* Description    : Retargets the C library printf function to the USART.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int fputc(int ch, FILE *f)
{
  /* Write a character to the USART */
  USART_SendData(USART1, (u8) ch);
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
  {
  }
  return ch;
}






/* End of file ---------------------------------------------------------------*/
