/*
 * adc.h
 *
 *  Created on: 08-Dec-2018
 *      Author: Devansh
 */

#ifndef SOURCES_ADC_H_
#define SOURCES_ADC_H_

void adc_init(void);
int adc_data(void);
unsigned char IsADCDataPositive(int data);

#endif /* SOURCES_ADC_H_ */
