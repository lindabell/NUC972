#include "bsp_gt911.h"
#include "bsp_i2c_gpio.h"
#include "sys.h"
#include "gpio.h"
#include "delay.h"

#define GT911_READ_XY_REG 0x814E /* ����Ĵ��� */
#define GT911_CLEARBUF_REG 0x814E /* �������Ĵ��� */
#define GT911_CONFIG_REG	0x8047	/* ���ò����Ĵ��� */
#define GT911_COMMAND_REG 0x8040 /* ʵʱ���� */
#define GT911_PRODUCT_ID_REG 0x8140 /*productid*/
#define GT911_VENDOR_ID_REG 0x814A /* ��ǰģ��ѡ����Ϣ */
#define GT911_CONFIG_VERSION_REG 0x8047 /* �����ļ��汾�� */
#define GT911_CONFIG_CHECKSUM_REG 0x80FF /* �����ļ�У���� */
#define GT911_FIRMWARE_VERSION_REG 0x8144 /* �̼��汾�� */


//rest--pd7
//int --pe7


/* ����GT911��λ���ж��������ӵ�GPIO�˿� */
#define GPIO_PORT_GT911_RST	GPIOD	/* GPIO�˿� */
#define GPIO_PORT_GT911_INT	GPIOE	/* GPIO�˿� */

#define GT911_RST_PIN	BIT7	/* ���ӵ�RSTʱ���ߵ�GPIO */
#define GT911_INT_PIN	BIT7	/* ���ӵ�INT�����ߵ�GPIO */

/* ����дRST��INT�ĺ� */
#define GT911_RST_1 GPIO_SetBit(GPIO_PORT_GT911_RST,GT911_RST_PIN)	/* RST = 1 */
#define GT911_RST_0 GPIO_ClrBit(GPIO_PORT_GT911_RST,GT911_RST_PIN)	/* RST = 0 */

#define GT911_INT_1 GPIO_SetBit(GPIO_PORT_GT911_INT,GT911_INT_PIN)	/* INT = 1 */
#define GT911_INT_0 GPIO_ClrBit(GPIO_PORT_GT911_INT,GT911_INT_PIN)	/* INT = 0 */

/*
GT911 �Ĵ��豸��ַ�������ѡ�������ַ�ֱ�Ϊ��0xBA/0xBB��0x28/0x29
*/
#define GT911_I2C_ADDR	0x28

typedef struct
{
    UINT8 Enable;
    UINT8 i2c_addr;

    UINT8 TouchpointFlag;

    UINT8 Touchkey1trackid;
    UINT16 X1;
    UINT16 Y1;
    UINT16 S1;

    UINT8 Touchkey2trackid;
    UINT16 X2;
    UINT16 Y2;
    UINT16 S2;

    UINT8 Touchkey3trackid;
    UINT16 X3;
    UINT16 Y3;
    UINT16 S3;

    UINT8 Touchkey4trackid;
    UINT16 X4;
    UINT16 Y4;
    UINT16 S4;

    UINT8 Touchkey5trackid;
    UINT16 X5;
    UINT16 Y5;
    UINT16 S5;
} GT911_T;
GT911_T g_GT911;

/*UINT8 s_GT911_CfgParams[] =
{
0x41,0x00,0x04,0x58,0x02,0x01,0x3D,0x00,0x01,0x08,\

0x28,0x08,0x50,0x3C,0x03,0x05,0x00,0x00,0x00,0x00,\

0x00,0x00,0x00,0x18,0x1A,0x1E,0x14,0x8C,0x28,0x0A,\

0x40,0x42,0xB5,0x06,0x00,0x00,0x00,0x02,0x02,0x1D,\

0x00,0x01,0x00,0x00,0x00,0x03,0x64,0x00,0x00,0x00,\

0x00,0x32,0x5A,0x94,0xC5,0x02,0x08,0x00,0x00,0x00,\

0x98,0x35,0x00,0x8A,0x3B,0x00,0x7A,0x43,0x00,0x6E,\

0x4B,0x00,0x62,0x55,0x00,0x62,0x00,0x00,0x00,0x00,\

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\

0x00,0x00,0x02,0x04,0x06,0x08,0x0A,0x0C,0x0E,0x10,\

0x12,0x14,0x16,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,\

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\

0x00,0x00,0x00,0x02,0x04,0x06,0x08,0x0A,0x0F,0x10,\

0x12,0x16,0x18,0x1C,0x1D,0x1E,0x1F,0x20,0x21,0x22,\

0x24,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,\

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\

0x00,0x00,0x00,0x00,0x60,0x01};*/
/* GT911�����������ò�����һ����д�� */
//UINT8 s_GT911_CfgParams[] =
//{

//    0x41,	//0x8047	�汾��
//    0x58, 0x02,	//0x8048/8049	X����������ֵ480
//    0x00, 0x04,	//0x804a/804b	Y����������ֵ854
//    0x01,	//0x804c	��������������
//    0x35,	//0x804d	������룬�½��ش���
//    0x00,	//0x804e	reserved
//    0x02,	//0x804f	��ָ����ȥ��������
//    0x08,	//0x8050	ԭʼ���괰���˲�ֵ	10
//    0x28,	//0x8051	������������
//    0x0A,	//0x8052	��������ֵ
//    0x5A,	//0x8053	���ϴ�������޵��е���ֵ
//    0x46,	//0x8054	���ϴ�������е��޵���ֵ
//    0x03,	//0x8055	���͹���ʱ��	s
//    0x05,	//0x8056	�����ϱ���
//    0x00,	//0x8057	X�������������
//    0x00,	//0x8058	Y�������������
//    0x00, 0X00,	//0x8059-0x805a	reserved	20
//    0x00,	//0x805b	reserved
//    0x00,	//0x805c	reserved
//    0x00,	//0x805d	���߹�����Сfilter����
//    0x18,	//0x805e	��������	1	ϵ��
//    0x1A,	//0x805f	��������	2	ϵ��
//    0x1E,	//0x8060	��������	3	ϵ��
//    0x14,	//0x8061	�������������
//    0x8C,	//0x8062	����
//    0x28,	//0x8063	����
//    0x0A,	//0x8064	����	30
//    0x71,	//0x8065	������A������Ƶ�ʱ�Ƶϵ��
//    0x73,	//0x8066	������B������Ƶ�ʱ�Ƶϵ��
//    0xB2,	//0x8067	������A��B�Ļ�Ƶ
//    0x04,	//0x8068
//    0x00,	//0x8069	�������������ź����ʱ����
//    0x00,	//0x806a
//    0x00,	//0x806b	����
//    0x02,	//0x806c	����
//    0x03,	//0x806d	ԭʼֵ�Ŵ�ϵ��
//    0x1D,	//0x806e	����
//    0x00,	//0x806f	reserved
//    0x01,	//0x8070	����
//    0x00, 0x00,	//reserved
//    0x00,	//0x8073	����
//    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	//0x8071 - 0x8079 reserved
//    0x50,	//0x807a	��Ƶ��Χ�����Ƶ��
//    0xA0,	//0x807b	��Ƶ��Χ���յ�Ƶ��
//    0x94,	//0x807c	�����������ȷ����������1-63��Ч
//    0xD5,	//0x807d	������ⳬʱʱ��
//    0x02,	//0x807e	����
//    0x07,	//0x807f	�б��и��ŵ�����
//    0x00, 0x00,	//0x8081	reserved
//    0x04,	//0x8082	��Ƶ�������Ƶ��1���ĵ��Ƶ������������A��B��
//    0xA4,	//0x8083
//    0x55,	//0x8084	��Ƶ�������Ƶ��1���ĵ㱶Ƶϵ��
//    0x00,	//0x8085	��Ƶ�������Ƶ��2���ĵ��Ƶ(����A��B�ڴ˻����ϻ���)
//    0x91,	//0x8086
//    0x62,	//0x8087	��Ƶ�������Ƶ��2���ĵ㱶Ƶϵ��
//    0x00,	//0x8088	��Ƶ�������Ƶ��3���ĵ��Ƶ������������A��B��
//    0x80,	//0x8089
//    0x71,	//0x808a	��Ƶ�������Ƶ��3���ĵ㱶Ƶϵ��
//    0x00,	//0x808b	��Ƶ�������Ƶ��4���ĵ��Ƶ������������A��B��
//    0x71,	//0x808c
//    0x82,	//0x808d	��Ƶ�������Ƶ��4���ĵ㱶Ƶϵ��
//    0x00,	//0x808e	��Ƶ�������Ƶ��5���ĵ��Ƶ������������A��B��
//    0x65,	//0x808f
//    0x95,	//0x8090	��Ƶ�������Ƶ��5���ĵ㱶Ƶϵ��
//    0x00,	0x65,	//reserved
//    0x00,	//0x8093	key1λ��	0���ް���
//    0x00,	//0x8094	key2λ��	0���ް���
//    0x00,	//0x8095	key3λ��	0���ް���
//    0x00,	//0x8096	key4λ��	0���ް���
//    0x00,	//0x8097	reserved
//    0x00,	//0x8098	reserved
//    0x00,	//0x8099	reserved
//    0x00,	//0x809a	reserved
//    0x00,	//0x809b	reserved
//    0x00,	//0x809c	reserved
//    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	//0x809d-0x80b2	reserved
//    0x00,	//0x80b3	�Ͽ����
//    0x00,	//0x80b4
//    0x00, 0x00,	//0x80b6	reserved
//    0x06,	//0x80b7
//    0x08,	//0x80b8
//    0x0A,	//0x80b9
//    0x0C,	//0x80ba
//    0x0E,	//0x80bb
//    0x10,	//0x80bc
//    0x12,	//0x80bd
//    0x14,	//0x80be
//    0x16,	//0x80bf
//    0x18,	//0x80c0
//    0x1A,	//0x80c1
//    0x1C,	//0x80c2
//    0xFF,	//0x80c3
//    0xFF,	//0x80c4
//    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//    0x00, 0x00, 0x00, 0x00,	//0x80c5-0x80d4	reserved
//    0x00,	//0x80d5
//    0x02,	//0x80d6
//    0x04,	//0x80d7
//    0x06,	//0x80d8
//    0x08,	//0x80d9
//    0x0A,	//0x80da
//    0x0C,	//0x80db
//    0x0F,	//0x80dc
//    0x10,	//0x80dd
//    0x12,	//0x80de
//    0x13,	//0x80df
//    0x14,	//0x80e0
//    0x16,	//0x80e1
//    0x18,	//0x80e2
//    0x1C,	//0x80e3
//    0x1D,	//0x80e4
//    0x1E,	//0x80e5
//    0x1F,	//0x80e6
//    0x20,	//0x80e7
//    0x21,	//0x80e8
//    0xFF,	//0x80e9
//    0xFF,	//0x80ea
//    0xFF,	//0x80eb
//    0xFF,	//0x80ec
//    0xFF,	//0x80ed
//    0xFF,	//0x80ee
//    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//    0x00, 0x00, 0x00, 0x00,	//0x80ef-0x80fe	reserved
//    0x0B,	//0x80ff	������ϢУ��
//    0x01	//0x8100	�����Ը��±��
//};

uint8_t s_GT911_CfgParams[] = {	
/*	0x50,0x20,0x03,0xE0,0x01,0x05,0x0D,0x00,0x01,0x0A,0x28,0x0F,0x50,0x32,0x03,0x05,0x00,0x00,0x00,0x00,
	0x00,0x00,0x08,0x17,0x19,0x1C,0x14,0x87,0x29,0x0A,0x4E,0x50,0xEB,0x04,0x00,0x00,0x00,0x00,0x02,0x11,
	0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x46,0x64,0x94,0xC5,0x02,0x07,0x00,0x00,0x04,
	0x9E,0x48,0x00,0x8D,0x4D,0x00,0x7F,0x53,0x00,0x73,0x59,0x00,0x67,0x60,0x00,0x67,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x04,0x06,0x08,0x0A,0x0C,0x0E,0x10,
	0x12,0x14,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x02,0x04,0x06,0x08,0x0A,0x0C,0x1D,0x1E,0x1F,0x20,0x21,0x22,0x24,0x26,0x28,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xB6,0x01	*/
////????????	
	0x50,             //0x8047        ???
	0x20,0x03,        //0x8048/0x8049 X???????800		 0x20,0x03,//
	0xE0,0x01,        //0x804A/0x804B Y???????480		 0xE0,0x01,	//
	0x05,             //0x804C        ????????
	0x0D,             //0x804D        ???? ?????
	0x00,             //0x804E        reserved
	0x01,             //0x804F        ?????????  	     0x01
	0x0A,             //0x8050        ?????????        0x0A 
	0x28,             //0x8051        ???????
	0x0F,             //0x8052        ?????
	0x50,             //0x8053        ????????????	 0x50	????
	0x32,             //0x8054        ????????????	 0x32	????
	0x03,             //0x8055        ???????? s
	0x05,             //0x8056        ?????
	0x00,             //0x8057        X???????    
	0x00,             //0x8058        Y???????
	0x00,0x00,        //0x8059-0x805A reserved
	0x00,             //0x805B        reserved  ??????
	0x00,             //0x805C        reserved  ??????
	0x08,             //0x805D        ??????filter??
	0x17,             //0x805E        ????1??
	0x19,             //0x805F        ????2??
	0x1C,             //0x8060        ????3??
	0x14,             //0x8061        ???????
	0x87,             //0x8062
	0x29,             //0x8063
	0x0A,             //0x8064
	0x4E,             //0x8065        ???A?????????        
	0x50,             //0x8066        ???B?????????
	0xEB,             //0x8067        ???A?B??????        
	0x04,             //0x8068        ???A?B??????
	0x00,             //0x8069        ?????????????????
	0x00,             //0x806A        ?????????????????
	0x00,             //0x806B
	0x00,             //0x806C
	0x02,             //0x806D        ???????  0x02
	0x11,             //0x806E
	0x00,             //0x806F        reserved
	0x01,             //0x8070    
	0x00,             //0x8071        reserved
	0x00,             //0x8072        reserved
	0x00,             //0x8073        reserved
	0x00,0x00,0x00,0x00,0x00,0X00,	//0x8074-0x8079 reserved
	0x46,             //0x807A        ?????????
	0x64,             //0x807B        ?????????
	0x94,             //0x807C        ???????????? 1-63??
	0xC5,             //0x807D        ????????
	0x02,             //0x807E
	0x07,             //0x807F        ????????
	0x00,0x00,        //0x8080-0x8081 reserved
	0x04,             //0x8082        ????????1?????(?????A?B)
	0x9E,             //0x8083        
	0x48,             //0x8084        ????????1???????
	0x00,             //0x8085        ????????2?????(??A?B???????)
	0x8D,             //0x8086       
	0x4D,             //0x8087        ????????2???????
	0x00,             //0x8088        ????????3?????(?????A?B)
	0x7F,             //0x8089        
	0x53,             //0x808A        ????????3???????
	0x00,             //0x808B        ????????4?????(?????A?B)
	0x73,             //0x808C        
	0x59,             //0x808D        ????????4???????
	0x00,             //0x808E        ????????5?????(?????A?B)
	0x67,             //0x808F
	0x60,             //0x8090        ????????5???????
	0x00,0x67,        //0x8091-0x8092 reserved
	0x00,             //0x8093        key1??0:???
	0x00,             //0x8094        key2??0:???
	0x00,             //0x8095        key3??0:???
	0x00,             //0x8096        key4??0:???
	0x00,             //0x8097        reserved
	0x00,             //0x8098        reserved
	0x00,             //0x8099        reserved
	0x00,             //0x809A        reserved
	0x00,             //0x809B        reserved
	0x00,             //0x809C        reserved
	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,    //0x809D-0x80B2
	0x00,             //0x80B3        ????
	0x00,             //0x80B4
	0x00,0x00,        //0x80B5-0x80B6 reserved
	0x02,             //0x80B7
	0x04,             //0x80B8
	0x06,             //0x80B9
	0x08,             //0x80BA
	0x0A,             //0x80BB
	0x0C,             //0x80BC
	0x0E,             //0x80BD
	0x10,             //0x80BE
	0x12,             //0x80BF
	0x14,             //0x80C0
	0xFF,             //0x80C1
	0xFF,             //0x80C2
	0xFF,             //0x80C3
	0xFF,             //0x80C4
	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,              //0x80C5-0x80D4 reserved
	0x00,             //0x80D5
	0x02,             //0x80D6
	0x04,             //0x80D7
	0x06,             //0x80D8
	0x08,             //0x80D9
	0x0A,             //0x80DA
	0x0C,             //0x80DB
	0x1D,             //0x80DC
	0x1E,             //0x80DD
	0x1F,             //0x80DE
	0x20,             //0x80DF
	0x21,             //0x80E0
	0x22,             //0x80E1
	0x24,             //0x80E2
	0x26,             //0x80E3
	0x28,             //0x80E4
	0xFF,             //0x80E5
	0xFF,             //0x80E6
	0xFF,             //0x80E7
	0xFF,             //0x80E8
	0xFF,             //0x80E9
	0xFF,             //0x80EA
	0xFF,             //0x80EB
	0xFF,             //0x80EC
	0xFF,             //0x80ED
	0xFF,             //0x80EE
	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,              //0x80EF-0x80FE reserved
	0xB6,             //0x80FF        ??????
	0x01	          //0x8100        ???????
};

UINT8 s_GT911_ClearStatue[] =
{
    0x00
};


static void GT911_WriteReg(UINT16 _usRegAddr, UINT8 *_pRegBuf, UINT8 _ucLen);
static void GT911_ReadReg(UINT16 _usRegAddr, UINT8 *_pRegBuf, UINT8 _ucLen);


/*
*********************************************************************************************************
*	�� �� ��: GT911_INT_GPIO_Input_Init
*	����˵��: ��ʼ��RST����Ϊ���������INT����Ϊ��©���
*	�� ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void GT911_RST_INT_GPIO_Init(void)
{
    GPIO_OpenBit(GPIO_PORT_GT911_RST, GT911_RST_PIN, DIR_OUTPUT, PULL_UP);
    GPIO_OpenBit(GPIO_PORT_GT911_INT, GT911_INT_PIN, DIR_OUTPUT, PULL_UP);
}
/*
*********************************************************************************************************
*	�� �� ��: GT911_INT_GPIO_Input_Init
*	����˵��: �趨INT����Ϊ��������
*	�� ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void GT911_INT_GPIO_Input_Init(void)
{
    GPIO_SetBitDir(GPIO_PORT_GT911_INT, GT911_INT_PIN, DIR_INPUT);
}


/*
*********************************************************************************************************
*	�� �� ��: GT911_Reset_Sequence
*	����˵��: G911Ӳ��λ����,RSTΪ�͵�ƽʱ��INT����Ϊ�͵�ƽ��1ms��RST��Ϊ�ߵ�ƽ��10ms��INT����Ϊ���룬
*	ʹGT911��ַ�趨Ϊ0xBA/0xBB��
*	�� ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void GT911_Reset_Sequence(UINT8 ucAddr)
{
    GT911_RST_INT_GPIO_Init();
    switch(ucAddr)
    {
    case 0xBA:
    GT911_RST_0; //RST���ŵ͵�ƽ
    GT911_INT_0; //INT���ŵ͵�ƽ
    delay_ms(30); //��ʱ30ms�����1
    GT911_RST_1; //RST���Ÿߵ�ƽ
    GT911_INT_0; //INT���ŵ͵�ƽ
    delay_ms(30); //��ʱ30ms�����20
    GT911_INT_0;
    delay_ms(30); //��ʱ30ms�����20
    GT911_INT_1;
        break;
    case 0x28:
        GT911_RST_0; //RST���ŵ͵�ƽ
        GT911_INT_1; //INT���Ÿߵ�ƽ
        delay_ms(30); //��ʱ30ms�����1
        GT911_RST_1; //RST���Ÿߵ�ƽ
        GT911_INT_1; //INT���Ÿߵ�ƽ
        delay_ms(30); //��ʱ30ms�����20
        GT911_INT_0;
        delay_ms(30); //��ʱ30ms�����20
        GT911_INT_1;
        break;
    default: //ȱʡΪ0xBA
        GT911_RST_0; //RST���ŵ͵�ƽ
        GT911_INT_0; //INT���ŵ͵�ƽ
        delay_ms(30); //��ʱ30ms�����1
        GT911_RST_1; //RST���Ÿߵ�ƽ
        GT911_INT_0; //INT���ŵ͵�ƽ
        delay_ms(30); //��ʱ30ms�����20
        GT911_INT_0;
        delay_ms(30); //��ʱ30ms�����20
        GT911_INT_1;
        break;

    }
}
/*
*********************************************************************************************************
*	�� �� ��: GT911_Soft_Reset
*	����˵��: G911��λ������
*	�� ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void GT911_Soft_Reset(void)
{
    UINT8 buf[1];
    buf[0] = 0x01;
    GT911_WriteReg(GT911_COMMAND_REG, (UINT8 *)buf, 1);
}
/*
*********************************************************************************************************
*	�� �� ��: GT911_ReadStatue
*	����˵��: G911����ƷID�����ò����汾�š�
*	�� ��: ��
*	�� �� ֵ: ���ò����汾��
*********************************************************************************************************
*/
UINT8 GT911_ReadStatue(void)
{
    UINT8 buf[4];
    GT911_ReadReg(GT911_PRODUCT_ID_REG, (UINT8 *)&buf[0], 3);
    GT911_ReadReg(GT911_CONFIG_VERSION_REG, (UINT8 *)&buf[3], 1);
    sysprintf("TouchPad_ID:%c,%c,%c\r\nTouchPad_Config_Version:%2x\r\n", buf[0], buf[1], buf[2], buf[3]);
    return buf[3];
}
/*
*********************************************************************************************************
*	�� �� ��: GT911_Exti_Int
*	����˵��: G911�ж����ų�ʼ����
*	�� ��: ��
*	�� �� ֵ: ���ò����汾��
*********************************************************************************************************
*/
void GT911_Exti_Int(void)
{

}

/*
*********************************************************************************************************
*	�� �� ��: GT911_InitHard
*	����˵��: ���ô���оƬ.
*	�� ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void GT911_InitHard(void)
{
    UINT8 config_Checksum = 0, i;
    g_GT911.i2c_addr = GT911_I2C_ADDR;

    GT911_Reset_Sequence(g_GT911.i2c_addr); /*��λGT911���趨�豸��ַΪ0xBA/0xBB*/

    GT911_Soft_Reset(); /*��λ*/

    /*��ȡ�����ļ��汾������У���*/
    s_GT911_CfgParams[0] = GT911_ReadStatue();
    for(i = 0; i < sizeof(s_GT911_CfgParams) - 2; i++)
    {
        config_Checksum += s_GT911_CfgParams[i];
    }
    s_GT911_CfgParams[184] = (~config_Checksum) + 1;

    /* ����������Ϣ���� */
    GT911_WriteReg(GT911_CONFIG_REG, (UINT8 *)s_GT911_CfgParams, sizeof(s_GT911_CfgParams));

    GT911_INT_GPIO_Input_Init(); //�趨INT����Ϊ��������

    GT911_Soft_Reset(); /*��λ*/

    /*��ʼ��У׼���ȴ�200ms*/
    delay_ms(200);

    g_GT911.Enable = 1;
}

/*
*********************************************************************************************************
*	�� �� ��: GT911_ReadFirmwareVersion
*	����˵��: ���GT911��оƬ�̼��汾
*	�� ��: ��
*	�� �� ֵ: 16λ�汾��
*********************************************************************************************************
*/
UINT16 GT911_ReadFirmwareVersion(void)
{
    UINT8 buf[2];

    GT911_ReadReg(GT911_FIRMWARE_VERSION_REG, buf, 2);

    return ((UINT16)buf[1] << 8) + buf[0];
}



/*
*********************************************************************************************************
*	�� �� ��: GT911_WriteReg
*	����˵��: д1���������Ķ���Ĵ���
*	�� ��: _usRegAddr : �Ĵ�����ַ
*	_pRegBuf : �Ĵ������ݻ�����
*	_ucLen : ���ݳ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void GT911_WriteReg(UINT16 _usRegAddr, UINT8 *_pRegBuf, UINT8 _ucLen)
{
    UINT8 i;

    IIC_Start();	/* ���߿�ʼ�ź� */

    IIC_Send_Byte(g_GT911.i2c_addr);	/* �����豸��ַ+д�ź� */
    IIC_Wait_Ack();
//sysprintf("A��%d",IIC_Wait_Ack());
    IIC_Send_Byte(_usRegAddr >> 8);	/* ��ַ��8λ */
    
    IIC_Wait_Ack();
//sysprintf("B��%d",IIC_Wait_Ack());
    IIC_Send_Byte(_usRegAddr);	/* ��ַ��8λ */
    
    IIC_Wait_Ack();
//sysprintf("C��%d\r\n",IIC_Wait_Ack());
    for (i = 0; i < _ucLen; i++)
    {
        IIC_Send_Byte(_pRegBuf[i]);	/* �Ĵ������� */
        IIC_Wait_Ack();
    }

    IIC_Stop(); /* ����ֹͣ�ź� */
}

/*
*********************************************************************************************************
*	�� �� ��: GT911_ReadReg
*	����˵��: ��1���������Ķ���Ĵ���
*	�� ��: _usRegAddr : �Ĵ�����ַ
*	_pRegBuf : �Ĵ������ݻ�����
*	_ucLen : ���ݳ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void GT911_ReadReg(UINT16 _usRegAddr, UINT8 *_pRegBuf, UINT8 _ucLen)
{
    UINT8 i;

    IIC_Start();	/* ���߿�ʼ�ź� */

    IIC_Send_Byte(g_GT911.i2c_addr);	/* �����豸��ַ+д�ź� */
    IIC_Wait_Ack();
   
    IIC_Send_Byte(_usRegAddr >> 8);	/* ��ַ��8λ */
    IIC_Wait_Ack();

    IIC_Send_Byte(_usRegAddr);	/* ��ַ��8λ */
    IIC_Wait_Ack();
    IIC_Stop();
    IIC_Start();
    IIC_Send_Byte(g_GT911.i2c_addr + 0x01);	/* �����豸��ַ+���ź� */
    IIC_Wait_Ack();

    for (i = 0; i < _ucLen - 1; i++)
    {
        _pRegBuf[i] = IIC_Read_Byte(1);	/* ���Ĵ������� */
    }

    /* ���һ������ */
    _pRegBuf[i] = IIC_Read_Byte(0);	/* ���Ĵ������� */

    IIC_Stop();	/* ����ֹͣ�ź� */
}


/*
*********************************************************************************************************
*	�� �� ��: GT911_OnePiontScan
*	����˵��: ��ȡGT911�������ݣ��������ȡһ�������㡣
*	�� ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void GT911_OnePiontScan(void)
{
    UINT8 buf[9];
    static UINT8 s_tp_down = 0;

    if (g_GT911.Enable == 0)
    {
        return;
    }

    /* ��ȡ�Ĵ�����0x814E R Bufferstatus Large_Detect number of touch points */

    /* �ж��Ƿ��£�û�а��£�ֱ���˳� */
    GT911_ReadReg(GT911_READ_XY_REG, (UINT8 *)buf, 1);
    if ((buf[0] & 0x01) == 0)
    {
        if (s_tp_down == 1)
        {
            /* State.x��State.y����ֵ������£�State��ȫ�ֱ���������ľ������һ�ε���ֵ */
            s_tp_down = 0;
        }
        GT911_WriteReg(GT911_CLEARBUF_REG, (UINT8 *)&buf[8], 1);
        return;
    }

    /* ��ȡ��һ��������1 */
    GT911_ReadReg(GT911_READ_XY_REG + 1, &buf[1], 7);
    GT911_WriteReg(GT911_CLEARBUF_REG, (UINT8 *)&buf[8], 1);
    /*
    0x814E R/W Bufferstatus Large_Detect number of touch points
    0x814F R track id

    0x8150 R Point1Xl ������ 1��X ����� 8 λ
    0x8151 R Point1Xh ������ 1��X ����� 8 λ
    0x8152 R Point1Yl ������ 1��Y ����� 8 λ
    0x8153 R Point1Yh ������ 1��Y ����� 8 λ
    0x8154 R Point1 ������ 1����������� 8 λ
    0x8155 R Point1 ������ 1����������� 8 λ
    */
    g_GT911.TouchpointFlag = buf[0];
    g_GT911.Touchkey1trackid = buf[1];

    g_GT911.X1 = ((UINT16)buf[3] << 8) + buf[2];
    g_GT911.Y1 = ((UINT16)buf[5] << 8) + buf[4];
    g_GT911.S1 = ((UINT16)buf[7] << 8) + buf[6];

    /* ��ⰴ�� */

    if (s_tp_down == 0)
    {
        s_tp_down = 1;
//State.x = g_GT911.X1;
//State.y = g_GT911.Y1;
//State.Pressed = 1;
//GUI_PID_StoreState(&State);
    }
    else
    {
//State.x = g_GT911.X1;
//State.y = g_GT911.Y1;
//State.Pressed = 1;
//GUI_PID_StoreState(&State);
    }

#if 1
    sysprintf("X1��%5d,Y1��%5d,S1��%3d\r\n", g_GT911.X1, g_GT911.Y1, g_GT911.S1); //���ڴ�ӡ����ֵ
#endif
}

/*
*********************************************************************************************************
*	�� �� ��: GT911_Scan
*	����˵��: ��ȡGT911�������ݡ���ȡȫ�������ݡ�
*	�� ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void GT911_Scan(void)
{
    UINT8 buf[40];
    UINT8 Clearbuf = 0;
    UINT8 i;
    static UINT8 s_tp_down = 0;

    if (g_GT911.Enable == 0)
    {
        return;
    }

    GT911_ReadReg(GT911_READ_XY_REG, buf, 1);
    if ((buf[0] & 0x0F) == 0)
    {
        GT911_WriteReg(GT911_CLEARBUF_REG, (UINT8 *)&Clearbuf, 1);
        return;
    }

    GT911_ReadReg(GT911_READ_XY_REG + 1, &buf[1], 39);
    GT911_WriteReg(GT911_CLEARBUF_REG, (UINT8 *)&Clearbuf, 1);


    g_GT911.TouchpointFlag = buf[0];

    g_GT911.Touchkey1trackid = buf[1];
    g_GT911.X1 = ((UINT16)buf[3] << 8) + buf[2];
    g_GT911.Y1 = ((UINT16)buf[5] << 8) + buf[4];
    g_GT911.S1 = ((UINT16)buf[7] << 8) + buf[6];

    g_GT911.Touchkey2trackid = buf[9];
    g_GT911.X2 = ((UINT16)buf[11] << 8) + buf[10];
    g_GT911.Y2 = ((UINT16)buf[13] << 8) + buf[12];
    g_GT911.S2 = ((UINT16)buf[15] << 8) + buf[14];

    g_GT911.Touchkey3trackid = buf[17];
    g_GT911.X3 = ((UINT16)buf[19] << 8) + buf[18];
    g_GT911.Y3 = ((UINT16)buf[21] << 8) + buf[20];
    g_GT911.S3 = ((UINT16)buf[23] << 8) + buf[22];

    g_GT911.Touchkey4trackid = buf[25];
    g_GT911.X4 = ((UINT16)buf[27] << 8) + buf[26];
    g_GT911.Y4 = ((UINT16)buf[29] << 8) + buf[28];
    g_GT911.S4 = ((UINT16)buf[31] << 8) + buf[30];

    g_GT911.Touchkey5trackid = buf[33];
    g_GT911.X5 = ((UINT16)buf[35] << 8) + buf[34];
    g_GT911.Y5 = ((UINT16)buf[37] << 8) + buf[36];
    g_GT911.S5 = ((UINT16)buf[39] << 8) + buf[38];


    if (s_tp_down == 0)
    {
        s_tp_down = 1;
//touch down
    }
    else
    {
//touch move
    }

#if 1
    sysprintf("\r\n");
    sysprintf("(%5d,%5d,%3d) ", g_GT911.X1, g_GT911.Y1, g_GT911.S1);
    sysprintf("\r\n");
#endif
}
uint8_t GT911_get_xy(int16_t *x,int16_t *y)
{
    UINT8 buf[40];
    UINT8 Clearbuf = 0;
    UINT8 i;
    static UINT8 s_tp_down = 0;

    if (g_GT911.Enable == 0)
    {
        return;
    }

    GT911_ReadReg(GT911_READ_XY_REG, buf, 1);
    if ((buf[0] & 0x0F) == 0)
    {
        GT911_WriteReg(GT911_CLEARBUF_REG, (UINT8 *)&Clearbuf, 1);
        return 0;
    }

    GT911_ReadReg(GT911_READ_XY_REG + 1, &buf[1], 39);
    GT911_WriteReg(GT911_CLEARBUF_REG, (UINT8 *)&Clearbuf, 1);


    g_GT911.TouchpointFlag = buf[0];

    g_GT911.Touchkey1trackid = buf[1];
    g_GT911.X1 = ((UINT16)buf[3] << 8) + buf[2];
    g_GT911.Y1 = ((UINT16)buf[5] << 8) + buf[4];
    g_GT911.S1 = ((UINT16)buf[7] << 8) + buf[6];

    g_GT911.Touchkey2trackid = buf[9];
    g_GT911.X2 = ((UINT16)buf[11] << 8) + buf[10];
    g_GT911.Y2 = ((UINT16)buf[13] << 8) + buf[12];
    g_GT911.S2 = ((UINT16)buf[15] << 8) + buf[14];

    g_GT911.Touchkey3trackid = buf[17];
    g_GT911.X3 = ((UINT16)buf[19] << 8) + buf[18];
    g_GT911.Y3 = ((UINT16)buf[21] << 8) + buf[20];
    g_GT911.S3 = ((UINT16)buf[23] << 8) + buf[22];

    g_GT911.Touchkey4trackid = buf[25];
    g_GT911.X4 = ((UINT16)buf[27] << 8) + buf[26];
    g_GT911.Y4 = ((UINT16)buf[29] << 8) + buf[28];
    g_GT911.S4 = ((UINT16)buf[31] << 8) + buf[30];

    g_GT911.Touchkey5trackid = buf[33];
    g_GT911.X5 = ((UINT16)buf[35] << 8) + buf[34];
    g_GT911.Y5 = ((UINT16)buf[37] << 8) + buf[36];
    g_GT911.S5 = ((UINT16)buf[39] << 8) + buf[38];


    if (s_tp_down == 0)
    {
        s_tp_down = 1;
//touch down
    }
    else
    {
//touch move
    }

#if 0
    sysprintf("\r\n");
    sysprintf("(%5d,%5d,%3d) ", g_GT911.X1, g_GT911.Y1, g_GT911.S1);
    sysprintf("\r\n");
#endif
    *x=g_GT911.X1;
    *y=g_GT911.Y1;
    return 1;
}
/*
*********************************************************************************************************
*	�� �� ��: GT911_ReadProductID
*	����˵��: ʶ����ʾģ����𡣶�ȡGT911 ProductID��
*	�� ��: ��
*	�� �� ֵ: ��ʾģ�����
*********************************************************************************************************
*/
UINT32 GT911_ReadProductID(void)
{
    UINT8 buf[4];
    UINT32 value = 0;
    /* Product_ID*/
    GT911_ReadReg(GT911_PRODUCT_ID_REG, buf, 4);
    value = ((UINT32)buf[3] << 24) + ((UINT32)buf[2] << 16) + ((UINT32)buf[1] << 8) + buf[0];
    return value;
}
