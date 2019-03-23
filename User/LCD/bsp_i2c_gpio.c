/*-----------------------NUC972����������----------------------------

����˵����

��IO��ģ��IIC����

���뻷����Keil 5.24a
��ַ��qlqc.taobao.com

----------------���ƿƼ����ൺǧ�ڵ��ӿƼ����޹�˾��Ȩ����-----------*/

#include "nuc970.h"
#include "sys.h"
#include "gpio.h"
#include "bsp_i2c_gpio.h"
#include "delay.h"

//IO��������	 
#define SET_IIC_SCL    GPIO_Set(GPIOE, BIT9) //SCL
#define SET_IIC_SDA    GPIO_Set(GPIOE, BIT8) //SDA	

#define CLE_IIC_SCL    GPIO_Clr(GPIOE, BIT9) //SCL
#define CLE_IIC_SDA    GPIO_Clr(GPIOE, BIT8) //SDA	


#define READ_SDA  		 GPIO_ReadBit(GPIOE, BIT8)  //����SDA 


//sda --pe8
//scl --pe9

//��ʼ��IIC
void IIC_Init(void)
{					     
    GPIO_OpenBit(GPIOE,BIT9, DIR_OUTPUT, PULL_UP);
    GPIO_OpenBit(GPIOE,BIT8, DIR_OUTPUT, PULL_UP);
		SET_IIC_SCL; 
	  SET_IIC_SDA;
	  delay_ms(500);
}

//IO��������
void SDA_IN()
{
	 GPIO_SetBitDir(GPIOE, BIT8, DIR_INPUT);
}
void SDA_OUT()
{
    GPIO_SetBitDir(GPIOE, BIT8, DIR_OUTPUT);
}



//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	SET_IIC_SDA;	  	  
	SET_IIC_SCL;
	delay_us(4);
 	CLE_IIC_SDA;
	delay_us(4);
	CLE_IIC_SCL;
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();
	CLE_IIC_SCL;
	CLE_IIC_SDA;
 	delay_us(4);
	SET_IIC_SCL; 
	SET_IIC_SDA;
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();     
	SET_IIC_SDA;delay_us(1);	   
	SET_IIC_SCL;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	CLE_IIC_SCL;
	return 0;  
} 
//����ACKӦ��
void IIC_Ack(void)
{
	CLE_IIC_SCL;
	SDA_OUT();
	CLE_IIC_SDA;
	delay_us(2);
	SET_IIC_SCL;
	delay_us(2);
	CLE_IIC_SCL;
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	CLE_IIC_SCL;
	SDA_OUT();
	SET_IIC_SDA;
	delay_us(2);
	SET_IIC_SCL;
	delay_us(2);
	CLE_IIC_SCL;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    CLE_IIC_SCL;
    for(t=0;t<8;t++)
    {              
		if((txd&0x80)>>7)
			SET_IIC_SDA;
		else
			CLE_IIC_SDA;
		txd<<=1; 	  
		delay_us(2);
		SET_IIC_SCL;
		delay_us(2); 
		CLE_IIC_SCL;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        CLE_IIC_SCL; 
        delay_us(2);
		SET_IIC_SCL;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}
