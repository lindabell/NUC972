#include "nuc970.h"
#include "sys.h"
#include "uart.h"
#include "lcd_bsp.h"

uint8_t clor;
int main(void)
{
    sysDisableCache();
    sysFlushCache(I_D_CACHE);
    sysEnableCache(CACHE_WRITE_BACK);
    sysInitializeUART();

	sysprintf("\n\n Hello NUC970 !!!\n");

    sysprintf("APLL    clock %d MHz\n", sysGetClock(SYS_APLL));
    sysprintf("UPLL    clock %d MHz\n", sysGetClock(SYS_UPLL));
    sysprintf("CPU     clock %d MHz\n", sysGetClock(SYS_CPU));
    sysprintf("System  clock %d MHz\n", sysGetClock(SYS_SYSTEM));
    sysprintf("HCLK1   clock %d MHz\n", sysGetClock(SYS_HCLK1));
    sysprintf("HCLK234 clock %d MHz\n", sysGetClock(SYS_HCLK234));
    sysprintf("PCLK    clock %d MHz\n", sysGetClock(SYS_PCLK));

    lcd_lint();
    clor=0x11;
    while(1)
    {
        memset((void *)u16FrameBufPtr,clor, 800*480*2);
        clor+=0x11;
    }
}
