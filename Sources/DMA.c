/*****************************************************************************
​ ​* ​ ​ @file​ ​  		DMA.c
​ * ​ ​ @brief​ ​ 		This C file contains the function definitions responsible
 *					initializing the DMA controller, creating a lookup table to
 *					interpolate the dBFS values and calculating the peak value
 *					from a sample set. It also contains the DMA ISR.
 *   @Tools_Used 	GCC
​ * ​ ​ @author​  		Souvik De, ​Devansh Mittal
​ * ​ ​ @date​ ​ 		December 12th, 2018
​ * ​ ​ @version​ ​ 		1.0
*****************************************************************************/

#include "DMA.h"
#include "fsl_device_registers.h"
#include "uart.h"
#include "buffer.h"
#include <math.h>
#define DECAY	(0.85)		//Decay Coefficient

void CreatDBFSLookup(void)
{
	int i = 0, j = 0, k=0;

	dbfs_count = 1;
	dbfs_lookup[0] = -90; //First entry of dBFS which happens to be maximum possible value for raw value of 1
	raw_lookup[0] = 1;	  //First entry of ADC raw value which is also the smallest possible legitimate value

	/* Create Loopup table */
	for(i = 1 ; i < DBFS_MAX-1 ; i++)
	{
		j = (int)((20 * log((float)(i)/(DBFS_MAX))/log(10)));
		k = (int)((20 * log((float)(i+1)/(DBFS_MAX))/log(10)));

		if(abs(j ^ k) != 0) // When adjacent values are not equal (j != k)
		{
			dbfs_lookup[dbfs_count] = k;
			raw_lookup[dbfs_count] = i+1;
			dbfs_count++;
		}

		if(dbfs_count == 79)	//Break when maximum dBFS attained
			break;
	}
}

uint16_t GetLocalPeak()
{
	int i=0;
	uint16_t temp = DMA_data[0];

	/* Get Maximum Value (Peak) of DMA (ADC Samples) Data Set */
	for (i = 1; i < 64 ; i++)
	{
		if(temp < DMA_data[i])
		{
			temp = DMA_data[i];
		}
	}
	return temp;
}

void DMA_init(void)
{
	/* Clocks Enabled for DMA Multiplexer and Controller */
	SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
	SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;

	/* Disable DMA MUX channel to configure DMA Controller */
	DMAMUX0_CHCFG0 = 0x00;

	/* Configuring DMA */
	DMA_SAR0 = (uint32_t)&ADC0->R[0];		//Source Address (ADC)
	DMA_DAR0 = (uint32_t)DMA_data;			//Destination Address (Buffer)
	DMA_DSR_BCR0 = DMA_DSR_BCR_BCR(128);	//Bytes to be transfered (128 Bytes)

	DMA_DCR0 |= (DMA_DCR_SSIZE(2)) |		//Source Size BUS (16 bits)
				(DMA_DCR_D_REQ_MASK) |		//ERQ bit is cleared when the BCR is exhausted
				(DMA_DCR_DINC_MASK) |		//Destination increment Enable
				(DMA_DCR_EINT_MASK) |		//Enable Interrupt
				(DMA_DCR_DSIZE(2)) |		//Destination Size BUS (16 bits)
				(DMA_DCR_DMOD(0)) |			//Circular Buffer Disabled
				(DMA_DCR_ERQ_MASK);			//Enable Peripheral Request

	/* Enable DMA Channel and Source for ADC0 */
	DMAMUX0_CHCFG0 |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(40);

	/* Enable Interrupt */
	NVIC_EnableIRQ(DMA0_IRQn);
}

void DMA0_IRQHandler(void)
{
	PTB_BASE_PTR->PCOR = 1 << 9;	//Set GPIO

	if(DMA_FLAG == 0)				//Calculate only when Flag is CLeared from Main
	{
		count++;					//Track Number of Data Sets

		if(count == 1)				//Create Lookup table before processing first data set
			CreatDBFSLookup();

		for(parser = 0; parser < 64 ; parser++)
			DMA_data_temp[parser] = DMA_data[parser];	//Store in temporary variable to safeguard against overwriting, until operations are over

		local_peak = GetLocalPeak();					//Get Peak value of a data set

		if(count == 1)
			peak = local_peak;							//First Local Peak is the main Peak as there are no data sets yet to compare
		else
		{
			if(local_peak > peak)
			{
				peak = local_peak;						//Local peak becomes main peak, if succeeding data set peak is more than the previous
			}
			else
			{
				peak = (uint16_t)(DECAY * peak);		//Decay present peak if succeeding data set peak is smaller than the present peak
			}
		}
	}

	DMA_FLAG = 1;	//Set Flag for Display in Main

	/* CLear Done Flag to restart */
	DMA_DSR_BCR0 |= DMA_DSR_BCR_DONE_MASK;
	DMA_init();		//Initialize DMA Again

	PTB_BASE_PTR->PSOR = 1 << 9;	//Clear GPIO
}
