/*****************************************************************************
​ ​* ​ ​ @file​ ​  		adc.h
​ * ​ ​ @brief​ ​ 		Contains the function prototypes essential for adc.c
 *   @Tools_Used 	GCC
​ * ​ ​ @author​  		Souvik De, ​Devansh Mittal
​ * ​ ​ @date​ ​ 		December 12th, 2018
​ * ​ ​ @version​ ​ 		1.0
*****************************************************************************/

#ifndef SOURCES_ADC_H_
#define SOURCES_ADC_H_

/**
* @Brief  Initializes ADC0.
*
* @Param  void
*
* @Return void
**/
void adc_init(void);

/**
* @Brief  Reads ADC Data.
*
* @Param  void
*
* @Return Data
**/
int adc_data(void);

/**
* @Brief  Checks if ADC Data is Positive or Negative
*
* @Param  Data
*
* @Return Error Code - Positive or Negative
**/
unsigned char IsADCDataPositive(int data);

#endif /* SOURCES_ADC_H_ */
