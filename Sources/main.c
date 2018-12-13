/*****************************************************************************
​ ​* ​ ​ @file​ ​  		main.c
​ * ​ ​ @brief​ ​ 		The main function is responsible for initializing the GPIO
 *			 		which is later toggled at the DMA ISR. It also displays
 *			 		the peak value, dBFS values and raw ADC values on the
 *			 		serial port. Additionally it initializes the UART, Circular
 *			 		buffer for UART, ADC and DMA.
 *   @Tools_Used 	GCC
​ * ​ ​ @author​  		Souvik De, ​Devansh Mittal
​ * ​ ​ @date​ ​ 		December 12th, 2018
​ * ​ ​ @version​ ​ 		1.0
*****************************************************************************/

#include "fsl_device_registers.h"
#include "uart.h"
#include "buffer.h"
#include "adc.h"
#include "DMA.h"
#include <math.h>
#include <stdint.h>

/* Search Lookup table to Interpolate dBFS value for a given DMA/ADC Sample */
int SearchDBFSLookup(uint16_t raw)
{
	int i,value;

	for(i = 1 ; i <= dbfs_count ;i++)
	{
		if(i == dbfs_count)			//For Maximum dBFS when Raw Data is towards the higher end e.g. 32000
		{
			value = dbfs_count;
			break;
		}

		if((raw - raw_lookup[i]) < 0)	//When Raw value exceeds the interpolation point in lookup table
		{
			value = i - 1;				//Take the preceding value
			break;
		}
	}
	return dbfs_lookup[value];			//Return Interpolated dBFS Value
}

int main(void)
{
	int i = 0;		//Parser for DMA Data Set
	float j = 0;	//To Store Interpolated dBFS Value

	/* Initialize UART */
	UART_init(UART0, 115200);

	count = 0;		//Initialize Data Set Counter
	DMA_FLAG = 0;	//Initialize DMA ISR Flag

	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;			// PTB0 clock
	PORTB_BASE_PTR->PCR[9] = PORT_PCR_MUX(1);	//GPIO Operation
	PTB_BASE_PTR->PDDR = (1 << 9);				//Set PortB 9th Pin as Output

	/* Initialize circular buffer for transmitting characters to Terminal Emulator */
	inita();
	/* Initialize ADC */
	adc_init();
	/* Initialize DMA */
	DMA_init();


	while(1)
	{
		/* Display required data when Data Set available in DMA */
		if(DMA_FLAG == 1)
		{
			uart_string("\r\n");
			uart_num(peak);	//Display Calculated Peak Value from DMA - ISR

			for(i = 0 ; i < 64 ; i++)
			{
				uart_string(" \t");
				j = SearchDBFSLookup(DMA_data_temp[i]);	//Display Interpolated dBFS Value from Lookup Table

				/* Check if Data is Valid */
				if(j <= 0)
				{
					if(j < 0)
						uart_data('-');

					j = j * (-1);	//Get Absolute value

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

				/* Check if Differential ADC data is positive or negative and display accordingly */
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

			//Clear Flag to enable Relevant DMA-TSR Calculation
			DMA_FLAG = 0;
		}
	}

    return 0;
}
