/*****************************************************************************
​ ​* ​ ​ @file​ ​  		DMA.h
​ * ​ ​ @brief​ ​ 		Contains the function prototypes, macros and global
 *					variables essential for DMA.c
 *   @Tools_Used 	GCC
​ * ​ ​ @author​  		Souvik De, ​Devansh Mittal
​ * ​ ​ @date​ ​ 		December 12th, 2018
​ * ​ ​ @version​ ​ 		1.0
*****************************************************************************/

#ifndef SOURCES_DMA_H_
#define SOURCES_DMA_H_

#include <stdint.h>
#define DBFS_MAX (32767)	//Maximum Possible ADC Value in DMA Destination Buffer

/**
* @Brief  Initializes DMA Controller.
*
* @Param  void
*
* @Return void
**/
void DMA_init(void);

/**
* @Brief  Returns Peak Value of DMA Buffer (ADC Sample Set) Data.
*
* @Param  void
*
* @Return Peak Value
**/
uint16_t GetLocalPeak(void);

/**
* @Brief  Creates dBFS Lookup Table.
*
* @Param  void
*
* @Return void
**/
void CreatDBFSLookup(void);

/* 128 Byte ADC Data in DMA Buffer */
volatile uint16_t DMA_data[64];
/* Temporary Storage variable for DMA Data (128 byte ADC Sample Set) */
volatile uint16_t DMA_data_temp[32];
/* Sample set Tracker and For loop parser respectively */
volatile int count,parser;
/* Local Peak of a data set and peak after comparing two peaks of subsequent data sets repectively */
volatile uint16_t peak,local_peak;
/* Flag to synchronize calculations in ISR and display in Main */
volatile unsigned char DMA_FLAG;
/* Lookup Table for dBFS and table length */
volatile int dbfs_lookup[80], dbfs_count;
/* Lookup table for Raw DMA Data */
volatile uint16_t raw_lookup[80];
/* To toggle destination address of DMA - DMA Buffer Upper and Lower Half Alternately*/
volatile unsigned char toggel_buffer_address;


#endif /* SOURCES_DMA_H_ */
