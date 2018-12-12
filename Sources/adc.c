/*
 * adc.c
 *
 *  Created on: 08-Dec-2018
 *      Author: Devansh
 */

#include<uart.h>

void adc_init(void)
{
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;	// ADC 0 clock

	ADC0_CFG1 = 0;
	ADC0->CFG1 |= ADC_CFG1_ADICLK(1) | ADC_CFG1_MODE(3) | ADC_CFG1_ADIV(3);
	ADC0_SC2 |= ADC_SC2_DMAEN_MASK;
	ADC0->SC3 = 0;
	ADC0->SC3 |= ADC_SC3_ADCO(1);
	ADC0->SC1[0] = 0;
	ADC0->SC1[0] |= ADC_SC1_DIFF(1);
}

int adc_data(void)
{
	if((ADC0->SC1[0] & 0x80) != 0)
	{
		return ADC0->R[0];
	}
	return -1;
}

unsigned char IsADCDataPositive(int data)
{
	if((data & 0x8000) == 0)
		return 1;
	return 0;
}
