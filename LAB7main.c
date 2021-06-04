

#include <stdint.h>
#include "msp430.h"
#include "HAL_PMM.h"
#include "HAL_UCS.h"
#include "HAL_Board.h"
#include "HAL_Buttons.h"
#include "HAL_Cma3000.h"
#include "HAL_Dogs102x6.h"
#include "HAL_Menu.h"
#include "HAL_Wheel.h"
#include "LaunchpadDef.h"
#include "LPM.h"
#include "MassStorage.h"
#include "Mouse.h"
#include "PMM.h"
#include "lab7.h"


void main( void )
{
    uint8_t contrast    = *( (unsigned char *) contrastSetpointAddress);        /* ��ȡFLASH�жԱȶ�ֵ */
    uint8_t brightness  = *( (unsigned char *) brightnessSetpointAddress);      /* ��ȡFLASH�б���ֵ */

    /* Initialize accelerometer offset from flash */
    Cma3000_setAccel_offset( *( (unsigned char *) accelXcalibrationAddress),        /* ��ʼ�����ٶ�ƫ���� */
                 *( (unsigned char *) accelYcalibrationAddress),
                 *( (unsigned char *) accelZcalibrationAddress) );

    /* Stop WDT */
    WDTCTL = WDTPW + WDTHOLD;                                                       /* �رտ��Ź� */

    /* Basic GPIO initialization */
    Board_init();                                                                   /* ��ʼ��GPIO */

    /* Set Vcore to accomodate for max. allowed system speed */
    SetVCore( 3 );                                                                  /* ��VCoreΪ��� */

    /* Use 32.768kHz XTAL as reference */
    LFXT_Start( XT1DRIVE_0 );                                                       /* ����LFXT1��32.768kHZ����Ϊʱ�Ӳο� */

    /* Set system clock to max (25MHz) */
    Init_FLL_Settle( 25000, 762 );                                                  /* ����FLL����Ƶ������ϵͳʱ����Ϊ���25MHZ */

    SFRIFG1 = 0;                                                                    /* ���жϱ�־ */
    SFRIE1  |= OFIE;                                                                /* ʹ�ܾ���ʧЧ�ж� */

    /* Globally enable interrupts */
    __enable_interrupt();                                                           /* ʹ��ȫ���ж� */

    /*
     * Setup real time clock
     * SetupRTC();                                  //����ʵʱʱ��
     */

    /* Set up LCD */
    Dogs102x6_init();                                                               /* ��ʼ��LCD */
    Dogs102x6_backlightInit();                                                      /* �����ʼ�� */

    /* Contrast not programed in Flash Yet */
    if ( contrast == 0xFF )                                                         /* ����ǰFLASH���޶Աȶ�ֵ���򽫶Աȶ�ֵ��Ϊ11��Ĭ�ϣ� */
        /* Set Default Contrast */
        contrast = 11;

    /* Brightness not programed in Flash Yet */
    if ( brightness == 0xFF )                                                       /* ����ǰFLASH���ޱ���ֵ���򽫱���ֵ��Ϊ11��Ĭ�ϣ� */
        /* Set Default Brightness */
        brightness = 11;

    Dogs102x6_setBacklight( brightness );                                           /* ���ó�ʼ����ֵ */
    Dogs102x6_setContrast( contrast );                                              /* ���ó�ʼ�Աȶ�ֵ */
    Dogs102x6_clearScreen();                                                        /* ���� */

    /* Set up wheel */
    Wheel_init();                                                                   /* ��ʼ�����ֵ�λ�� */
    Buttons_init( BUTTON_ALL );                                                     /* ��ʼ������ */
    Buttons_interruptEnable( BUTTON_ALL );                                          /* ʹ�����а����ж� */
    buttonsPressed = 0;                                                             /* ��ֵ���� */


    Dogs102x6_clearScreen();                                                        /* �����Ļ��ʾ */
    buttonsPressed = 0;


    /* ��ѭ�� */
    while ( 1 )
    {
        readDS18B20andDisplay();                                                                /* �����߳��� */
    }
}


