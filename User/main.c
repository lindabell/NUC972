#include <stdlib.h>
#include "nuc970.h"
#include "sys.h"
#include "FreeRTOS.h"
#include "task.h"

static void thread_1s(void *pvParameters);

int main( void )
{
    sysDisableCache();
    sysFlushCache(I_D_CACHE);
    sysEnableCache(CACHE_WRITE_BACK);
    sysInitializeUART();
    sysSetLocalInterrupt(ENABLE_IRQ);

    sysprintf("\r\nFreeRTOS start\r\n");

    xTaskCreate(thread_1s,"thread 1s",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY+1,NULL);

    vTaskStartScheduler();

    while(1);
}
/*-----------------------------------------------------------*/

static void thread_1s(void *pvParameters)
{
    (void) pvParameters;
    while(1)
    {
        vTaskDelay(1000/portTICK_RATE_MS);
        sysprintf("*");
    }
}
