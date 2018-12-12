/*****************************************************************************
​ ​* ​ ​ @file​ ​  main.c
​ * ​ ​ @brief​ ​ This is the main function which calls the function to initialize
 *			 the UART and the circular buffers. It also blinks an LED and
 *			 computes the fibonacci series.
 *   @Tools_Used GCC
​ * ​ ​ @author​ ​Devansh Mittal, Souvik De
​ * ​ ​ @date​ ​ November 27th, 2018
​ * ​ ​ @version​ ​ 1.0
*****************************************************************************/

#include "fsl_device_registers.h"
#include "uart.h"
#include "buffer.h"
#include "adc.h"
#include "DMA.h"
#include <math.h>
#include <stdint.h>

int get2decimalplaces(float value)
{
    int integervalue = (int)value;
    return ((value -(float)integervalue)*100);
}

int SearchDBFSLookup(uint16_t raw)
{
	int i,value;

	for(i = 1 ; i <= dbfs_count ;i++)
	{
		if(i == dbfs_count)
		{
			value = dbfs_count;
			break;
		}

		if((raw - raw_lookup[i]) < 0)
		{
			value = i - 1;
			break;
		}
	}
	return dbfs_lookup[value];
}

int main(void)
{
	int i = 0;
	float j = 0;

	/* Initialize UART */
	UART_init(UART0, 115200);

	count = 0;
	DMA_FLAG = 0;

	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;	// PTB0 clock
	PORTB_BASE_PTR->PCR[18] = PORT_PCR_MUX(1);
	PTB_BASE_PTR->PDDR = (1 << 18);

	/* Initialize circular buffer for transmitting characters to Terminal Emulator */
	inita();
	init_DMABuffer();
	adc_init();
	DMA_init();


	while(1)
	{
		if(DMA_FLAG == 1)
		{
			uart_string("\r\n");
			uart_num(peak);

			for(i = 0 ; i < 64 ; i++)
			{
				uart_string(" \t");
				j = SearchDBFSLookup(DMA_data_temp[i]);
				if(j <= 0)
				{
					uart_data('-');
					j = j * (-1);
					if(j != 0)
						uart_num(j);
					else
						uart_data('0');
					uart_string(" dBFS");
				}
				else
				{
					uart_string("Error");
				}

				uart_string(" \t");
				if((DMA_data_temp[i] & 0x8000) == 0)
				{
					uart_num(DMA_data_temp[i] & 0x7FFF);
				}
				else
				{
					uart_data('-');
					uart_num(((DMA_data_temp[i] & 0x7FFF) ^ 0x7FFF) + 1);
				}

				uart_string("\r\n");
			}

			DMA_FLAG = 0;
		}
	}

    return 0;
}
