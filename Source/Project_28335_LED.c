/*
 * Project_28335_LED.c
 *
 *  Created on: 2021年9月14日
 *      Author: YellowNew
 */

#include "Project_28335_LED.h"
Uint16 ledCnt1 = 50;

void setupLEDGPIO()
{
    EALLOW;
    // 用于闪烁提示状态
    GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 0;    // GPIO pin
    GpioCtrlRegs.GPBDIR.bit.GPIO60 = 1;     // Output pin

    GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 0;    // GPIO pin
    GpioCtrlRegs.GPBDIR.bit.GPIO61 = 1;     // Output pin

    GpioDataRegs.GPBSET.bit.GPIO61 = 1;

    EDIS;
}

// toggleLED()
void toggleLED(void)
{
    if(ledCnt1==0)
        {
        GpioDataRegs.GPBTOGGLE.bit.GPIO60=1;
        ledCnt1=50;
        }
    else
        ledCnt1--;
}
