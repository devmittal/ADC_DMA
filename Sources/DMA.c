/*
 * DMA.c
 *
 *  Created on: 09-Dec-2018
 *      Author: Devansh
 */
#include "DMA.h"
#include "fsl_device_registers.h"
#include "uart.h"
#include "buffer.h"
#include <math.h>
#define DECAY	(0.85)

void CreatDBFSLookup(void)
{
	int i = 0, j = 0, k=0;

	dbfs_count = 1;
	dbfs_lookup[0] = -90;
	raw_lookup[0] = 1;

	for(i = 1 ; i < DBFS_MAX-1 ; i++)
	{
		j = (int)((20 * log((float)(i)/(DBFS_MAX))/log(10)));
		k = (int)((20 * log((float)(i+1)/(DBFS_MAX))/log(10)));

		if(abs(j ^ k) != 0)
		{
			dbfs_lookup[dbfs_count] = k;
			raw_lookup[dbfs_count] = i+1;
			dbfs_count++;
		}

		if(dbfs_count == 79)
			break;
	}
}

uint16_t GetLocalPeak()
{
	int i=0;
	uint16_t temp = DMA_data[0];
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
	SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
	SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;

	DMAMUX0_CHCFG0 = 0x00;

	DMA_SAR0 = (uint32_t)&ADC0->R[0];
	DMA_DAR0 = (uint32_t)DMA_data;
	DMA_DSR_BCR0 = DMA_DSR_BCR_BCR(128);
	DMA_DCR0 |= (DMA_DCR_SSIZE(2)) |
				(DMA_DCR_D_REQ_MASK) |
				(DMA_DCR_DINC_MASK) |
				(DMA_DCR_EINT_MASK) |
				(DMA_DCR_DSIZE(2)) |
				(DMA_DCR_DMOD(0)) |
				(DMA_DCR_ERQ_MASK);
	DMAMUX0_CHCFG0 |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(40);

	NVIC_EnableIRQ(DMA0_IRQn);
}

void DMA0_IRQHandler(void)
{
	PTB_BASE_PTR->PCOR = 1 << 9;

	if(DMA_FLAG == 0)
	{
		count++;

		if(count == 1)
			CreatDBFSLookup();

		for(parser = 0; parser < 64 ; parser++)
			DMA_data_temp[parser] = DMA_data[parser];

		local_peak = GetLocalPeak();

		if(count == 1)
			peak = local_peak;
		else
		{
			if(local_peak > peak)
			{
				peak = local_peak;
			}
			else
			{
				peak = (uint16_t)(DECAY * peak);
			}
		}
	}

	DMA_FLAG = 1;

	DMA_DSR_BCR0 |= DMA_DSR_BCR_DONE_MASK;
	DMA_init();
	PTB_BASE_PTR->PSOR = 1 << 9;
}
