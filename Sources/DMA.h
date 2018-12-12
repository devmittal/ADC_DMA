/*
 * DMA.h
 *
 *  Created on: 09-Dec-2018
 *      Author: Devansh
 */

#ifndef SOURCES_DMA_H_
#define SOURCES_DMA_H_

#include <stdint.h>
#define DBFS_MAX (32767)

void DMA_init(void);
uint16_t GetLocalPeak(void);
void CreatDBFSLookup(void);

volatile uint16_t DMA_data[64];
volatile uint16_t DMA_data_temp[64];

volatile int count,parser;
volatile uint16_t peak,local_peak;
volatile unsigned char DMA_FLAG;
volatile int dbfs_lookup[80], dbfs_count;
volatile uint16_t raw_lookup[80];



#endif /* SOURCES_DMA_H_ */
