#ifndef __BSP_GT911_H
#define __BSP_GT911_H
#include "nuc970.h"


void GT911_InitHard(void);
void GT911_Reset_Sequence(uint8_t ucAddr);
uint16_t GT911_ReadFirmwareVersion(void);
void GT911_Soft_Reset(void);
uint8_t GT911_ReadStatue(void);
void GT911_Scan(void);
#endif
