#include "lcd_bsp.h"
#include <stdlib.h>
#include "nuc970.h"
#include "lcd.h"
#include "gpio.h"
#include <string.h>
#include "sys.h"

uint16_t *u16FrameBufPtr;

void lcd_init(void)
{
    //add by qlqc
    outpw(REG_CLK_PCLKEN0,inpw(REG_CLK_PCLKEN0) | (1<<3)); //Enable GPIO engin clock.
    //add end
    GPIO_OpenBit(GPIOG,BIT3, DIR_OUTPUT, PULL_UP);
    GPIO_Set(GPIOG,BIT3);

    // 配置引脚复用到LCM
    //GPG6 (CLK), GPG7 (HSYNC)
    outpw(REG_SYS_GPG_MFPL, (inpw(REG_SYS_GPG_MFPL)& ~0xFF000000) | 0x22000000);
    //GPG8 (VSYNC), GPG9 (DEN)
    outpw(REG_SYS_GPG_MFPH, (inpw(REG_SYS_GPG_MFPH)& ~0xFF) | 0x22);
    //GPA0 ~ GPA7 (DATA0~7)
    outpw(REG_SYS_GPA_MFPL, 0x22222222);
    //GPA8 ~ GPA15 (DATA8~15)
    outpw(REG_SYS_GPA_MFPH, 0x22222222);
    //GPD8~D15 (DATA16~23)
    outpw(REG_SYS_GPD_MFPH, 0x22222222);

    //使能 LCD clock
    outpw(REG_CLK_DIVCTL1, (inpw(REG_CLK_DIVCTL1) & ~0xff1f) | 0xe18);
    //LCD初始化
    vpostLCMInit(0);
    //设置缩放比例1:1
    vpostVAScalingCtrl(1, 0, 1, 0, VA_SCALE_INTERPOLATION);
    //设置 像素格式RGB565
    vpostSetVASrc(VA_SRC_RGB565);
    // 获取显存地址指针
    u16FrameBufPtr =(uint16_t *)vpostGetFrameBuffer();
    if(u16FrameBufPtr == NULL)
    {
        sysprintf("获取显存地址指针失败!!\n");
        while(1);
    }
    vpostVAStartTrigger();
    //刷全屏为粉色
    memset((void *)u16FrameBufPtr,0XFF, 800*480*2);

}
