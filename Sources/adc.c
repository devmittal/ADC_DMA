/*****************************************************************************
​ ​* ​ ​ @file​ ​  		adc.c
​ * ​ ​ @brief​ ​ 		This C file contains the function definitions responsible for
 *					initializing the ADC, reading ADC data and checking ADC data
 *					sign bit.
 *   @Tools_Used 	GCC
​ * ​ ​ @author​  		Souvik De, ​Devansh Mittal
​ * ​ ​ @date​ ​ 		December 12th, 2018
​ * ​ ​ @version​ ​ 		1.0
*****************************************************************************/

#include "fsl_device_registers.h"
#include "adc.h"

void adc_init(void)
{
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;	// ADC 0 clock

	ADC0_CFG1 = 0;						//Cleared
	ADC0->CFG1 |= ADC_CFG1_ADICLK(1) |	//Clock Source = BUS_CLK/2
				  ADC_CFG1_MODE(3) |	//16 bits mode with DIFF = 1
				  ADC_CFG1_ADIV(3);		//Clock Rate is (Input Clock/8)
	ADC0_SC2 |= ADC_SC2_DMAEN_MASK;		//Enabling DMA for ADC
	ADC0->SC3 = 0;						//Cleared
	ADC0->SC3 |= ADC_SC3_ADCO(1);		//Continuous Conversion
	ADC0->SC1[0] = 0;					//Cleared
	ADC0->SC1[0] |= ADC_SC1_DIFF(1);	//Differential Mode Enable
}

int adc_data(void)
{
	if((ADC0->SC1[0] & 0x80) != 0)	//Checking Conversion Completion
	{
		return ADC0->R[0];			//ADC Data
	}
	return -1;						//Error Code, Conversion unsuccessful
}

unsigned char IsADCDataPositive(int data)
{
	if((data & 0x8000) == 0)		//Checking Sign Bit
		return 1;					//Return Positive
	return 0;						//Return Negative
}
