/*****************************************************************************
​ ​* ​ ​ @file​ ​  		buffer.c
​ * ​ ​ @brief​ ​ 		This function carries out different operations on the circular
 *		     		buffer like insert, remove and check if it is empty or full.
 *   @Tools_Used 	GCC
​ * ​ ​ @author​  		Souvik De, ​Devansh Mittal
​ * ​ ​ @date​ ​ 		December 12th, 2018
​ * ​ ​ @version​ ​ 		1.0
*****************************************************************************/

#include "fsl_device_registers.h"
#include "buffer.h"

/* Initializes ring buffer */
void inita()
{
	/* Allocate memory for the structure and character array (buffer) dynamically and process if successful */
	if((ringtx = malloc(1000 + sizeof(*ringtx))) != NULL)
	{
		/* Initialize all structure parameters */
		ringtx->Length = 1000;
		/* Set Rear (insertion - Ini) and Front (Deletion - Outi) Markers  to default */
		ringtx->Ini = 0;
		ringtx->Outi = 0;
		/* Point to space reserved for buffer */
		ringtx->Buffer = (char *)(ringtx + 1);
	}
}

/* To insert data in the circular buffer */
void insert_data(ring_t *ring, char data)
{
	/* Insert data in buffer and conform index within buffer length */
	ring->Buffer[ring->Ini++ % ring->Length] = data;
}

/* To remove data from the FIFO Circular buffer */
int remove_data(ring_t *ring)
{
	/* Remove single data and conform the out marker within buffer length */
	return ring->Buffer[ring->Outi++ % ring->Length];
}

/* Check if buffer is empty */
int isempty(ring_t *ring)
{
	return ring->Outi == ring->Ini;
}

/* Check if buffer is full */
int isfull(ring_t *ring)
{
	return ring->Ini-ring->Outi == ring->Length;
}
