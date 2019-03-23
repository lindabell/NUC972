#include "bsp.h"

void bsp_init(void)
{
	  *((volatile unsigned int *)REG_AIC_MDCR)=0xFFFFFFFF;  // disable all interrupt channel
    *((volatile unsigned int *)REG_AIC_MDCRH)=0xFFFFFFFF;  // disable all interrupt channel

	  outpw(REG_CLK_HCLKEN, 0x0527);
    outpw(REG_CLK_PCLKEN0, 0);
    outpw(REG_CLK_PCLKEN1, 0);
	
    sysDisableCache();
    sysFlushCache(I_D_CACHE);
    sysEnableCache(CACHE_WRITE_BACK);
	  sysInitializeUART();

		IIC_Init();
		GT911_InitHard();
}
