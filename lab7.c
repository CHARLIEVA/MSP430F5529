#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "msp430.h"
#include "HAL_PMM.h"
#include "HAL_UCS.h"
#include "HAL_Buttons.h"
#include "HAL_Dogs102x6.h"
#include "HAL_Board.h"
#include "HAL_Menu.h"
#include "HAL_Wheel.h"
#include "MassStorage.h"
#include "mouse.h"
#include "descriptors.h"
#include "usb.h"                            /* USB-specific functions */
#include "UsbCdc.h"
#include "Demo_Cube.h"
#include "CTS_Layer.h"
#include "LaunchpadDef.h"
#include "Random.h"
#include "msp430.h"
#include "HAL_PMM.h"
#include "HAL_UCS.h"
#include "HAL_Board.h"
#include "HAL_Buttons.h"
#include "HAL_Dogs102x6.h"
#include "HAL_Menu.h"
#include "HAL_Wheel.h"
#include "Clock.h"
#include "LPM.h"
#include "Random.h"
#include "PMM.h"
#include "Demo_Cube.h"
#include "CTS_Layer.h"
#include "stdlib.h"

#define CPU_F ( (double) 25000000)                      /* �ⲿ��Ƶ����8MHZ */
/* #define CPU_F ((double)32768)   //�ⲿ��Ƶ����32.768KHZ */
#define delay_us( x )   __delay_cycles( (long) (CPU_F * (double) x / 1000000.0) )
#define delay_ms( x )   __delay_cycles( (long) (CPU_F * (double) x / 1000.0) )

unsigned int temp_value = 0; /* �¶� */
unsigned int temp_2 = 27; /* �¶� */
unsigned int x1 = 13;
unsigned int temp_value_1 = 0;
unsigned int temp_3 = 0;
/*********************************
 **          ���Ŷ���            **
 *********************************/
#define DQ_1    P3OUT   |= BIT7
#define DQ_0    P3OUT   &= ~BIT7
#define DQ_in   P3DIR   &= ~BIT7
#define DQ_out  P3DIR   |= BIT7
#define DQ_val  (P3IN & BIT7)

/**********************************
 ****       �����ַ�����       *****
 **********************************/
#define Read_ROM        0x33    /* ��ROM */
#define Match_ROM       0x55    /* ƥ��ROM */
#define Skip_ROM        0xcc    /* ����ROM */
#define Search_ROM      0xf0    /* ����ROM */
#define Alarm_Search        0xec    /* �澯���� */
#define Convert_Temperature 0x44    /* �¶�ת�� */
#define Read_Scratchpad     0xbe    /* ���ݴ�洢��9�ֽ����� */
#define Write_Scratchpad    0x4e    /* д�ݴ�洢����д����TH and TL �����ŷ�����λ���ݾͿ��� */

unsigned int DS18b20_Init(void)
{
    unsigned Check_val;
    DQ_out;
    DQ_0;
    delay_us(600);
    DQ_1;
    delay_us(60);
    DQ_in;
    _NOP();

    if ( DQ_val == 1)
    {
        Check_val = 0; /* ��ʼ��ʧ�� */
    }
    if ( DQ_val == 0)
    {
        Check_val = 1; /* ��ʼ���ɹ� */
    }
    delay_us(10);
    DQ_out;
    DQ_1;
    delay_us(100);
    return (Check_val);
}

void DS18b20_write_byte(unsigned int dat)
{
    unsigned int i;
    for (i = 0; i < 8; i++)
    {
        DQ_0;
        delay_us(2);
        if (dat & 0X01)
            DQ_1;
        else
            DQ_0;
        delay_us(60);
        dat >>= 1;
        ;
        DQ_1;
        delay_us(10);
    }
}

unsigned int DS18b20_read_byte(void)
{
    unsigned i;
    unsigned int byte = 0;
    for (i = 0; i < 8; i++)
    {
        byte >>= 1;
        DQ_0;
        delay_us(2);
        DQ_1;
        delay_us(2);
        DQ_in;
        _NOP();
        if ( DQ_val)
            byte |= 0x80;
        delay_us(60);
        DQ_out;
        DQ_1;
        delay_us(10);
    }
    return (byte);
}

unsigned int get_one_temperature(void) /* ֻ��ȡ��������û��ȡС���Ĳ��� */
{
    unsigned int Temp_l = 0, Temp_h = 0, Temp = 0;
    float f_temp;

    /* delay_ms( 1000 ); */

    DS18b20_Init();
    DS18b20_write_byte( Skip_ROM);
    DS18b20_write_byte( Read_Scratchpad);
    Temp_l = DS18b20_read_byte();
    Temp_h = DS18b20_read_byte();

    DS18b20_Init();
    DS18b20_write_byte( Skip_ROM);
    DS18b20_write_byte( Convert_Temperature);

    Temp_h <<= 8;
    Temp = Temp_h + Temp_l;
    f_temp = Temp * 0.0625; /* 18b20�ķֱ�����0.0625 */
    Temp = f_temp * 10 + 0.5; /* ����10��ʾС�������ȡһλ����0.5���������� */
    return (Temp); /* Temp������ */
}

void readDS18B20andDisplay(void)
{

    //Dogs102x6_stringDraw(0, 0, "   HUATU   ", DOGS102x6_DRAW_NORMAL); /* ��ʼ����ʾ���� */
    unsigned char dis[10];
    unsigned char time_cyc = 0;
    while (1)
    {

        delay_ms(10);
        time_cyc = (time_cyc + 1) % 30;


        if (time_cyc == 1) /* 30 ��10msִ��һ�� */
        {
//        	Dogs102x6_clearScreen();
            temp_value = get_one_temperature(); /* ��ȡ�¶� */
            dis[0] = 'T';
            dis[1] = 'e';
            dis[2] = 'm';
            dis[3] = 'p';
            dis[4] = ':';
            dis[5] = temp_value % 1000 / 100 + '0';
            dis[6] = temp_value % 100 / 10 + '0';
            dis[7] = '.';
            dis[8] = temp_value % 10 + '0';
            dis[9] = 'C';
            dis[10] = 0;
            Dogs102x6_stringDraw(0, 0, dis, DOGS102x6_DRAW_NORMAL);

            temp_3=58-temp_value;

//            Dogs102x6_pixelDraw(x1,temp_2,0);
            delay_us( 10 );
            Dogs102x6_lineDraw(x1,temp_2,x1+1,temp_3,0);
            temp_2=temp_3;
//            Dogs102x6_verticalLineDraw(temp_2,temp_3,x1,0);

            x1 += 1;



        	Dogs102x6_verticalLineDraw(10, 63, 13, DOGS102x6_DRAW_NORMAL);          //����ֱ�¶�������
        	Dogs102x6_pixelDraw(12,11, DOGS102x6_DRAW_NORMAL);
        	Dogs102x6_pixelDraw(11,12, DOGS102x6_DRAW_NORMAL);
        	Dogs102x6_pixelDraw(14,11, DOGS102x6_DRAW_NORMAL);
        	Dogs102x6_pixelDraw(15,12, DOGS102x6_DRAW_NORMAL);
        	Dogs102x6_horizontalLineDraw(15, 95, 63, DOGS102x6_DRAW_NORMAL);       //��ˮƽʱ��������
        	Dogs102x6_pixelDraw(94,62, DOGS102x6_DRAW_NORMAL);
        	Dogs102x6_pixelDraw(93,61, DOGS102x6_DRAW_NORMAL);
        	Dogs102x6_pixelDraw(94,64, DOGS102x6_DRAW_NORMAL);
        	Dogs102x6_pixelDraw(93,65, DOGS102x6_DRAW_NORMAL);

        	Dogs102x6_stringDraw(1, 0, "31", DOGS102x6_DRAW_NORMAL);               //д��ֱ�¶�����ֵ
        	Dogs102x6_stringDraw(2, 0, "30", DOGS102x6_DRAW_NORMAL);
        	Dogs102x6_stringDraw(3, 0, "29", DOGS102x6_DRAW_NORMAL);
        	Dogs102x6_stringDraw(4, 0, "28", DOGS102x6_DRAW_NORMAL);
        	Dogs102x6_stringDraw(5, 0, "27", DOGS102x6_DRAW_NORMAL);
        	Dogs102x6_stringDraw(6, 0, "26", DOGS102x6_DRAW_NORMAL);
        	Dogs102x6_stringDraw(7, 0, "25", DOGS102x6_DRAW_NORMAL);



            if(x1>95)
            {
            	Dogs102x6_clearScreen();
            	x1=13;
            }
        }


    }
}

