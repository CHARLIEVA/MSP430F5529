

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
    uint8_t contrast    = *( (unsigned char *) contrastSetpointAddress);        /* 读取FLASH中对比度值 */
    uint8_t brightness  = *( (unsigned char *) brightnessSetpointAddress);      /* 读取FLASH中背光值 */

    /* Initialize accelerometer offset from flash */
    Cma3000_setAccel_offset( *( (unsigned char *) accelXcalibrationAddress),        /* 初始化加速度偏移量 */
                 *( (unsigned char *) accelYcalibrationAddress),
                 *( (unsigned char *) accelZcalibrationAddress) );

    /* Stop WDT */
    WDTCTL = WDTPW + WDTHOLD;                                                       /* 关闭看门狗 */

    /* Basic GPIO initialization */
    Board_init();                                                                   /* 初始化GPIO */

    /* Set Vcore to accomodate for max. allowed system speed */
    SetVCore( 3 );                                                                  /* 设VCore为最大 */

    /* Use 32.768kHz XTAL as reference */
    LFXT_Start( XT1DRIVE_0 );                                                       /* 利用LFXT1（32.768kHZ）作为时钟参考 */

    /* Set system clock to max (25MHz) */
    Init_FLL_Settle( 25000, 762 );                                                  /* 利用FLL（锁频环）将系统时钟设为最大25MHZ */

    SFRIFG1 = 0;                                                                    /* 清中断标志 */
    SFRIE1  |= OFIE;                                                                /* 使能晶振失效中断 */

    /* Globally enable interrupts */
    __enable_interrupt();                                                           /* 使能全局中断 */

    /*
     * Setup real time clock
     * SetupRTC();                                  //设置实时时钟
     */

    /* Set up LCD */
    Dogs102x6_init();                                                               /* 初始化LCD */
    Dogs102x6_backlightInit();                                                      /* 背光初始化 */

    /* Contrast not programed in Flash Yet */
    if ( contrast == 0xFF )                                                         /* 若当前FLASH中无对比度值，则将对比度值设为11（默认） */
        /* Set Default Contrast */
        contrast = 11;

    /* Brightness not programed in Flash Yet */
    if ( brightness == 0xFF )                                                       /* 若当前FLASH中无背光值，则将背光值设为11（默认） */
        /* Set Default Brightness */
        brightness = 11;

    Dogs102x6_setBacklight( brightness );                                           /* 设置初始背光值 */
    Dogs102x6_setContrast( contrast );                                              /* 设置初始对比度值 */
    Dogs102x6_clearScreen();                                                        /* 清屏 */

    /* Set up wheel */
    Wheel_init();                                                                   /* 初始化齿轮电位计 */
    Buttons_init( BUTTON_ALL );                                                     /* 初始化按键 */
    Buttons_interruptEnable( BUTTON_ALL );                                          /* 使能所有按键中断 */
    buttonsPressed = 0;                                                             /* 键值清零 */


    Dogs102x6_clearScreen();                                                        /* 清除屏幕显示 */
    buttonsPressed = 0;


    /* 主循环 */
    while ( 1 )
    {
        readDS18B20andDisplay();                                                                /* 试验七程序 */
    }
}


