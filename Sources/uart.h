/*****************************************************************************
​ ​* ​ ​ @file​ ​  		uart.h
​ * ​ ​ @brief​ ​ 		Contains the function prototypes essential for uart.c
 *   @Tools_Used 	GCC
​ * ​ ​ @author​  		Souvik De, ​Devansh Mittal
​ * ​ ​ @date​ ​ 		December 12th, 2018
​ * ​ ​ @version​ ​ 		1.0
*****************************************************************************/

#include "fsl_device_registers.h"


/**
* @Brief This function initializes the UART.
*
* @Param  UART number to be initialized, and baud rate to be used.
*
* @Return void
**/
extern void UART_init(void *UARTx, uint32_t Baud);

#ifdef BLOCKING
/**
* @Brief This function transmits character through UART using blocking mechanism.
*
* @Param  Character to be transmitted.
*
* @Return void
**/
void UART_PutChar(char c);



/**
* @Brief This function receives character through UART using blocking mechanism.
*
* @Param  void
*
* @Return Character received
**/
char UART_GetChar(void);

#else
/**
* @Brief This function handles the interrupt operations for UART0.
*
* @Param  void
*
* @Return void
**/
void UART0_IRQHandler (void);

/**
* @Brief This function enables UART0.
*
* @Param interrupt number.
*
* @Return void
**/
void enable_irq (int irq);
#endif

/**
* @Brief This function transmits string out of UART using the circular buffer.
*
* @Param Pointer to string to be transmitted out of UART.
*
* @Return void
**/
void uart_string(char *p);

/**
* @Brief This function transmits numbers out of UART using the circular buffer.
*
* @Param Number to be transmitted out of UART.
*
* @Return void
**/
void uart_num(unsigned long int number);

/**
* @Brief This function transmits data out of UART using the circular buffer.
*
* @Param Data to be transmitted out of UART.
*
* @Return void
**/
void uart_data(unsigned char c);
