#include "bsp_lcd.h"
#include "lcd.h"
void bsp_LcdInit(void)
{
	// �������Ÿ��õ�LCM
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
		
	//ʹ�� LCD clockΪ50MHz
	outpw(REG_CLK_DIVCTL1, (inpw(REG_CLK_DIVCTL1) & ~0xff1f) | 0x518);		
	//LCD��ʼ��(ѡ��1024x600�ֱ���)
	vpostLCMInit(2);
	//�������ű���1:1
	vpostVAScalingCtrl(1, 0, 1, 0, VA_SCALE_INTERPOLATION);
	//���� ���ظ�ʽRGB565
    vpostSetVASrc(VA_SRC_RGB565);
}
