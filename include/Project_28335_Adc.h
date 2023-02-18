/*
 * Project_28335_Adc.h
 *
 *  Created on: 2021年9月14日
 *      Author: YellowNew
 */

#ifndef INCLUDE_PROJECT_28335_ADC_H_
#define INCLUDE_PROJECT_28335_ADC_H_

#include "main.h"
/* 宏定义 */
#if (CPU_FRQ_150MHZ)     // Default - 150 MHz SYSCLKOUT
 #define ADC_MODCLK 0x3 // HSPCLK = SYSCLKOUT/2*ADC_MODCLK2 = 150/(2*3)   = 25.0 MHz
 #define CPU_FRQ 150000000
#endif
#if (CPU_FRQ_100MHZ)
 #define ADC_MODCLK 0x2 // HSPCLK = SYSCLKOUT/2*ADC_MODCLK2 = 100/(2*2)   = 25.0 MHz
 #define CPU_FRQ 100000000
#endif
#if (CPU_FRQ_150MHZ)                // Default - 150 MHz SYSCLKOUT
  #define ADC_MODCLK 0x3        // HSPCLK = SYSCLKOUT/2*ADC_MODCLK2 = 150/(2*3)   = 25.0 MHz
#endif
#if (CPU_FRQ_100MHZ)
  #define ADC_MODCLK 0x2        // HSPCLK = SYSCLKOUT/2*ADC_MODCLK2 = 100/(2*2)   = 25.0 MHz
#endif
#define ADC_CKPS    0x0             // ADC module clock = HSPCLK/2*ADC_CKPS   = 25.0MHz/(1*1) = 25MHz
#define ADC_SHCLK  0x3              // S/H width in ADC module periods                        = 4 ADC clocks



/*函数声明 */
void InitAdcToDMA(void);

#endif /* INCLUDE_PROJECT_28335_ADC_H_ */
