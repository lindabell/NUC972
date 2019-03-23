/*-----------------------NUC972开发板例程----------------------------

例程说明：

用IO口模拟IIC总线

编译环境：Keil 5.24a
网址：qlqc.taobao.com

----------------新唐科技·青岛千亿电子科技有限公司版权所有-----------*/

#include "nuc970.h"
#include "sys.h"
#include "gpio.h"
#include "bsp_i2c_gpio.h"
#include "delay.h"

//IO操作函数	 
#define SET_IIC_SCL    GPIO_Set(GPIOE, BIT9) //SCL
#define SET_IIC_SDA    GPIO_Set(GPIOE, BIT8) //SDA	

#define CLE_IIC_SCL    GPIO_Clr(GPIOE, BIT9) //SCL
#define CLE_IIC_SDA    GPIO_Clr(GPIOE, BIT8) //SDA	


#define READ_SDA  		 GPIO_ReadBit(GPIOE, BIT8)  //输入SDA 


//sda --pe8
//scl --pe9

//初始化IIC
void IIC_Init(void)
{					     
    GPIO_OpenBit(GPIOE,BIT9, DIR_OUTPUT, PULL_UP);
    GPIO_OpenBit(GPIOE,BIT8, DIR_OUTPUT, PULL_UP);
		SET_IIC_SCL; 
	  SET_IIC_SDA;
	  delay_ms(500);
}

//IO方向设置
void SDA_IN()
{
	 GPIO_SetBitDir(GPIOE, BIT8, DIR_INPUT);
}
void SDA_OUT()
{
    GPIO_SetBitDir(GPIOE, BIT8, DIR_OUTPUT);
}



//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	SET_IIC_SDA;	  	  
	SET_IIC_SCL;
	delay_us(4);
 	CLE_IIC_SDA;
	delay_us(4);
	CLE_IIC_SCL;
}	  
//产生IIC停止信号
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
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
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
//产生ACK应答
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
//不产生ACK应答		    
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
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
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
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
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
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}
