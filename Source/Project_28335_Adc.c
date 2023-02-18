/*
 * Project_28335_Adc.c
 *
 *  Created on: 2021年9月14日
 *      Author: YellowNew
 */

/* 头文件包含 */
#include "Project_28335_Adc.h"

#pragma DATA_SECTION(DMABuf,"DMARAML4");

volatile Uint16 DMABuf[BUF_SIZE];

volatile Uint16 *DMADest;
volatile Uint16 *DMASource;


void InitAdcToDMA(void)
{
    Uint16 i;

    EALLOW;
    SysCtrlRegs.HISPCP.all = ADC_MODCLK;
    EDIS;

    InitAdc();
    AdcRegs.ADCTRL1.bit.ACQ_PS = ADC_SHCLK;                 // 4个时钟采样窗
    AdcRegs.ADCTRL3.bit.ADCCLKPS = ADC_CKPS;                // 不分频
    AdcRegs.ADCTRL1.bit.SEQ_CASC = 0;                       // 双排序器模式
    AdcRegs.ADCTRL3.bit.SMODE_SEL = 1;                      // 同步采样模式
    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x00;                 // 同步采样A0和B0
    AdcRegs.ADCMAXCONV.all = 0x0000;                        // 最大通道数2

    AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 1;                 // Enable SOCA from ePWM to start SEQ1
    AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;                   // Enable SEQ1 interrupt (every EOS)
    AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;                       // 开始



// Initialize DMA
    DMAInitialize();

    for (i=0; i<BUF_SIZE; i++)
    {
        DMABuf[i] = 0;
    }

// Configure DMA Channel
    DMADest   = &DMABuf[0];                            // Point DMA destination to the beginning of the array
    DMASource = &AdcMirror.ADCRESULT0;
    DMACH1AddrConfig(DMADest,DMASource);
    DMACH1BurstConfig(1,1,1);                           // 同步采样通道A0，A1的值，一帧两个值。
    DMACH1TransferConfig((ADC_DMATRSIZE-1),1,1);        // 存ADC_DMATRSIZE帧数据,放在0~(2ADC_DMATRSIZE-1)
    DMACH1WrapConfig(0,0,0xFFFF,0);                     // 每8次返回到结果寄存器0的地址。
    DMACH1ModeConfig                                    // 手动启动模式，一次只采样2ADC_DMATRSIZE个数据
        (DMA_SEQ1INT,
        PERINT_ENABLE,
        ONESHOT_DISABLE,
        CONT_DISABLE,
        SYNC_DISABLE,
        SYNC_SRC,
        OVRFLOW_DISABLE,
        SIXTEEN_BIT,
        CHINT_END,
        CHINT_DISABLE);

    StartDMACH1();
}
