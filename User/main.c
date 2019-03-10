#include <stdlib.h>
#include "nuc970.h"
#include "sys.h"
#include "FreeRTOS.h"
#include "task.h"

#include "etimer.h"
#include "gpio.h"
#include "lcd.h"
#include "lvgl.h"
#include "lcd_bsp.h"

static void thread_1s(void *pvParameters);
static void MainGui( void *pvParameters );
void time_init(void);

int main(void)
{
    sysDisableCache();
    sysFlushCache(I_D_CACHE);
    sysEnableCache(CACHE_WRITE_BACK);
    sysInitializeUART();
    sysSetLocalInterrupt(ENABLE_IRQ);

    time_init();
    lcd_init();

    sysprintf("\r\nFreeRTOS start\r\n");

    xTaskCreate(thread_1s,"thread 1s",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY+1,NULL);
    xTaskCreate( MainGui, "Gui", 1024, NULL, ( tskIDLE_PRIORITY + 3 ), NULL );
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


static void tft_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p)
{
    int dy;
    for(int i=y1;i<y2+1;i++)
    {
        dy=i*800;
        for(int j=x1;j<x2+1;j++)
        {
            u16FrameBufPtr[dy+j]=color_p->full;
            color_p++;
        }
    }
    lv_flush_ready();
}

void tft_fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2, lv_color_t color)
{
    sysprintf("2 %d %d %d %d\n",x1,y1,  x2,  y2);
}

static void tft_map(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p)
{
    sysprintf("2 %d %d %d %d\n",x1,y1,  x2,  y2);
}

#include "demo.h"
#include "desktop.h"
static void MainGui( void *pvParameters )
{
    (void) pvParameters;


    lv_init();
    //tft_init();
    //touch_init();
    //lcd_init();

    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);

    disp_drv.disp_fill = tft_fill;
    disp_drv.disp_map = tft_map;
    disp_drv.disp_flush = tft_flush;

    lv_disp_drv_register(&disp_drv);

    //demo_create();

    //lv_obj_t * btn1 = lv_btn_create(lv_scr_act(), NULL);

    desktop_ui_create(lv_scr_act());

    while(1)
    {
        lv_task_handler();
        vTaskDelay(10);
    }

}
//定时器中断为GUI提供一个心跳
void ETMR0_IRQHandler(void)
{
    lv_tick_inc(1);
    ETIMER_ClearIntFlag(0);
}
//定时器初始化
void time_init(void)
{
    // 使能 ETIMER0 时钟
    outpw(REG_CLK_PCLKEN0, inpw(REG_CLK_PCLKEN0) | (1 << 4));
    // 设置中断频率为 1KHZ
    ETIMER_Open(0, ETIMER_PERIODIC_MODE, 1000);
    //使能定时器中断
    ETIMER_EnableInt(0);
    sysInstallISR(HIGH_LEVEL_SENSITIVE | IRQ_LEVEL_1, ETMR0_IRQn, (PVOID)ETMR0_IRQHandler);
    sysSetLocalInterrupt(ENABLE_IRQ);
    sysEnableInterrupt(ETMR0_IRQn);
    //定时器开始
    ETIMER_Start(0);
}
