/*
 * Project_28335_InitEpwm.c
 *
 *  Created on: 2021年9月14日
 *      Author: YellowNew
 */

/* 头文件包含 */
#include "Project_28335_InitEpwm.h"

/*结构体*/
volatile struct EPWM_REGS *ePWM[PWM_CH] = { &EPwm2Regs, &EPwm2Regs, &EPwm3Regs};

/*函数体*/

void InitEpwm()
{
    DINT;
 // 将PWM中断程序入口地址写入向量表，优先级按数字顺序，低则高
    EALLOW;

    PieVectTable.EPWM2_INT = &EPWM2_INT;

    EDIS;

// Epwm--Gpio初始化
//    InitEPwm1Gpio();
    InitEPwm2Gpio();
    InitEPwm3Gpio();
    InitEPwm4Gpio();

// Epwm配置
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EDIS;

    Config_Epwm23();
    Config_EPwm4();

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;

// Enable CPU INT3 which is connected to EPWM1-3 INT:
    IER |= M_INT3;

// Enable EPWM INTn in the PIE: Group 3 interrupt 1-3
    PieCtrlRegs.PIEIER3.bit.INTx2 = 1;

// Enable global Interrupts and higher priority real-time debug events:
    EINT;   // Enable Global interrupt INTM
    ERTM;   // Enable Global realtime interrupt DBGM
}

/*函数体*/

// EPWM123_Init - Initialize EPWM123 configuration
void  Config_Epwm23()
{
    Uint16 j;

     for (j=1;j<PWM_CH;j++)
     {
         //
         // Setup TBCLK
         //
         (*ePWM[j]).TBPRD =7500-1;;             // PWM frequency = 1 / period
         (*ePWM[j]).TBPHS.all = 0x0000;      // Phase is 0
         (*ePWM[j]).TBCTR = 0x0000;               // Clear counter

         //
         //Setup counter mode
         //
         (*ePWM[j]).TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Up and Down
         (*ePWM[j]).TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
         (*ePWM[j]).TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
         (*ePWM[j]).TBCTL.bit.CLKDIV = TB_DIV1;

         //
         // Setup shadowing
         //
         (*ePWM[j]).CMPCTL.bit.SHDWAMODE = CC_SHADOW;
         (*ePWM[j]).CMPCTL.bit.SHDWBMODE = CC_SHADOW;
         (*ePWM[j]).CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // Load on Zero
         (*ePWM[j]).CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

         //
         //Setup  actions
         //
         (*ePWM[j]).AQCTLA.bit.ZRO = AQ_SET;   // PWM toggle high/low
         (*ePWM[j]).AQCTLA.bit.CAU = AQ_CLEAR;
         (*ePWM[j]).AQCTLA.bit.CAD = AQ_SET;
         (*ePWM[j]).AQCTLB.bit.CBU = AQ_SET;
         (*ePWM[j]).AQCTLB.bit.CBD = AQ_CLEAR;

         //
         //Active Low PWMs - Setup Deadband
         //
         (*ePWM[j]).DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
         (*ePWM[j]).DBCTL.bit.POLSEL = DB_ACTV_HIC;
         (*ePWM[j]).DBCTL.bit.IN_MODE = DBA_ALL;
         (*ePWM[j]).DBRED = 150;
         (*ePWM[j]).DBFED = 150;

         //
         // Interrupt where we will change the Deadband
         //
         (*ePWM[1]).ETSEL.bit.INTSEL = ET_CTR_ZERO;    // Select INT on Zero event
         (*ePWM[1]).ETSEL.bit.INTEN = 1;               // Enable INT
         (*ePWM[1]).ETPS.bit.INTPRD = ET_1ST;          // Generate INT on 3rd event

         (*ePWM[1]).ETSEL.bit.SOCAEN = 1;              // Enable Soc
         (*ePWM[1]).ETSEL.bit.SOCASEL = 1;             // TBCTR = 0
         (*ePWM[1]).ETPS.bit.SOCAPRD = 1;              // 一次事件触发一次


     }
}

void Config_EPwm4()
{

    EPwm4Regs.TBPRD = 7500-1;                    // PWM frequency = 1 / period
    EPwm4Regs.TBPHS.all = 0x0000;                  // Phase is 0
    EPwm4Regs.TBCTR = 0x0000;                      // Clear counter

    EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;     // Up and Down
    EPwm4Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm4Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
    EPwm4Regs.TBCTL.bit.CLKDIV = TB_DIV1;

    EPwm4Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm4Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm4Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // Load on Zero
    EPwm4Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    EPwm4Regs.AQCTLA.bit.ZRO = AQ_SET;
    EPwm4Regs.AQCTLA.bit.CAU = AQ_CLEAR;          // 当计数器A等于CMPA时，EPWM1A置高
    EPwm4Regs.AQCTLB.bit.ZRO = AQ_CLEAR;
    EPwm4Regs.AQCTLB.bit.CAU = AQ_SET;

    EPwm4Regs.CMPA.half.CMPA = 2265;

    EPwm4Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
    EPwm4Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
    EPwm4Regs.DBCTL.bit.IN_MODE = DBA_ALL;
    EPwm4Regs.DBRED = 150;
    EPwm4Regs.DBFED = 150;

}







