/*
 *  FileName:      main.c
 *  Describe:      2019年电赛B题--单相不间断电源
 *  Creator:       YellowNew
 *  Encoding:      GBK
 *  Date:          2021.9.14
 */

/* 头文件包含 */
#include "main.h"

/* 任务函数的声明  */
// 分支父节点
void A0(void);
void B0(void);

// A分支
void A1(void);								// 前级boost和后级逆变有效值ADC采样滤波
void A2(void);								// 拟双恒压恒流PID

// B分支
void B1(void);								// 拟定Oled显示

/* 全局变量声明 */
void (*Alpha_State_Ptr)(void);		        // 基础状态指针，AB分支父
void (*A_Task_Ptr)(void);				    // A任务分支函数指针
void (*B_Task_Ptr)(void);				    // B任务分支函数指针
interrupt void EPWM2_INT(void);


SPWM_VRB m_spwm;
ADC_VAB m_adc;
PID_DUNC m_PidBoost;
PID_DUNC m_PidInverter;

char Str[10];
//Uint16 Key_flg = 0;
int Key_flag = 0;

/* 主函数 */
int main(void)
    {
//TODO: 初始化系统
 	InitSystem();

//TODO: 初始化片内外设
	InitTaskTimer(TASKA_FRQ,TASKB_FRQ);
	InitEpwm();
	InitAdcToDMA();
 	UARTa_Init(115200);
    IICA_Init();
    OLED_Init();

//TODO: 参数初始化
	Init_Spwm(&m_spwm);
	PidInit_Boost(&m_PidBoost);
	PidInit_Inverter(&m_PidInverter);
	setupLEDGPIO();

//TODO: 使能GPIO22引脚用于监控
	EALLOW;
	GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 0;    // GPIO pin
	GpioCtrlRegs.GPADIR.bit.GPIO20 = 1;     // Output pin

    GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 0;    // GPIO pin
    GpioCtrlRegs.GPADIR.bit.GPIO21 = 1;     // Output pin
	EDIS;

//TODO: OLED 显示初始化
    OLED_ColorTurn(0);
    OLED_ShowString(32,0,"YellowNew",16,1);
    OLED_ShowString(2,24,"Uac_out:",16,1);
    OLED_ShowString(2,48,"Udc_out:",16,1);

//TODO: 初始化任务指针
	Alpha_State_Ptr = &A0;
	A_Task_Ptr = &A1;
	B_Task_Ptr = &B1;

	while(1)
	{
		// 状态机入口
		//====================================
		(*Alpha_State_Ptr)();	// 零跳转 (A0,B0,...)
		//====================================
	}
}

//===================================
 /*
  * 状态机第零节点任务与定步长
  */
//====================================
void A0(void)
{
	// A任务同步时钟循环等待
	if(CpuTimer0Regs.TCR.bit.TIF == 1)
	{
		CpuTimer0Regs.TCR.bit.TIF = 1;	// clear flag

		//-----------------------------------------------------------
		(*A_Task_Ptr)();		// 跳转至A任务分支 (A1,A2,A3,...)
		//-----------------------------------------------------------
	}

	Alpha_State_Ptr = &B0;		// 跳去B0
}

void B0(void)
{
	// A任务同步时钟循环等待
	if(CpuTimer1Regs.TCR.bit.TIF == 1)
	{
		CpuTimer1Regs.TCR.bit.TIF = 1;				// clear flag

		//-----------------------------------------------------------
		(*B_Task_Ptr)();		//跳转至B任务分支 (B1,B2,B3,...)
		//-----------------------------------------------------------

	}

	Alpha_State_Ptr = &A0;		// 跳去A0
}

//-----------------------------------------------------------
 /*
  * A任务分支
  * A1：采样数据处理
  * A2：PID闭环控制
  */
//-----------------------------------------------------------
void A1(void)
{
    register Uint16 i,j,t;
    register Uint16 *pYn_Adc_A0;
    register Uint16 *pYn_Adc_B0;
    register volatile Uint16 *P_DMABuf;
    pYn_Adc_B0 = m_adc.Yn_Adc_B0;
    pYn_Adc_A0 = m_adc.Yn_Adc_A0;
    P_DMABuf = DMABuf;

    while(DmaRegs.CH1.CONTROL.bit.RUNSTS);

//TODO: 将DMA传出的数据拿出后立即启动下一次转换。
//TODO: 取A0采样表
        for(i=0;i<(2*ADC_DMATRSIZE-1);i+=2)
        {
                *pYn_Adc_A0++ = *(P_DMABuf+i);
        }
//TODO: 取B0采样表
        for(i=1;i<=(2*ADC_DMATRSIZE-1);i+=2)
        {
                *pYn_Adc_B0++ = *(P_DMABuf+i);
        }
//TODO: 重新启动DMA
    StartDMACH1();
//TODO: 采样滤波
    pYn_Adc_A0 = m_adc.Yn_Adc_A0;
    pYn_Adc_B0 = m_adc.Yn_Adc_B0;

        for(i=0;i<ADC_DMATRSIZE-1;i++)  // 连个FOR循环 完成采样数组的从小到大排序
        for(j=0;j<ADC_DMATRSIZE-1-i;j++)
        {
            if(*(pYn_Adc_A0+j)>*(pYn_Adc_A0+j+1))
            {
                t=*(pYn_Adc_A0+j);
                *(pYn_Adc_A0+j)=*(pYn_Adc_A0);
                *(pYn_Adc_A0+j+1)=t;
            }

            if(*(pYn_Adc_B0+j)>*(pYn_Adc_B0+j+1))
            {
                t=*(pYn_Adc_B0+j);
                *(pYn_Adc_B0+j)=*(pYn_Adc_B0);
                *(pYn_Adc_B0+j+1)=t;
            }
        }
        m_adc.sumA0=0,m_adc.sumB0=0;
        for(i=0;i<10;i++)   //取最中间的十个数
        {
            m_adc.sumA0+=*(pYn_Adc_A0+i+10);
            m_adc.sumB0+=*(pYn_Adc_B0+i+10);
        }
        m_adc.Voltage_A0=(m_adc.sumA0/10.0*3.0/4096.0);    //按照公式转换电压
        m_adc.Voltage_B0=(m_adc.sumB0/10.0*3.0/4096.0);    //按照公式转换电压

#if SCOPE_FRE
            //TODO: 监视频率是否正确
       GpioDataRegs.GPADAT.bit.GPIO20 = ~GpioDataRegs.GPADAT.bit.GPIO20;
#endif

	A_Task_Ptr = &A2;	// 跳去A2
}

void A2(void)
{
//  Float_Printf(Str,m_spwm.M,1,3);
//  UARTa_SendString((char *)",");
//  Float_Printf(Str,m_PidInverter.ActualSpeed,1,3);
//  UARTa_SendString((char *)"\n");

//TODO: Boost 闭环软起动
if (m_PidBoost.Soft_Start > Star_Flag)
    {m_PidBoost.Soft_Start--;}
else if (m_PidBoost.Soft_Start <= Star_Flag)
    {m_PidBoost.Soft_Start = 0;}

if(m_PidBoost.Soft_Start == Star_Flag)
{
    toggleLED();
    m_PidBoost.ActualSpeed = m_adc.Voltage_A0;
    PidRealize_Boost(&m_PidBoost);
    EPwm4Regs.CMPA.half.CMPA=m_PidBoost.OutPut;
}

# if DEBUG_BOOST

    Float_Printf(Str,m_PidBoost.SetSpeed,1,3);
    UARTa_SendString((char *)",");
    Float_Printf(Str,m_PidBoost.ActualSpeed,1,3);
    UARTa_SendString((char *)"\n");

# endif

if(m_PidBoost.Soft_Start == Star_Flag)
{
    if(m_spwm.Pid_Flag == 1)
    {
        if(m_adc.Voltage_B0 > 0.8 || Key_flag == 1)
        {
        if(m_adc.Voltage_B0 > 0.8) Key_flag = 1;
        m_PidInverter.ActualSpeed = m_adc.Voltage_B0;
        PidRealize_Inverter(&m_PidInverter);
        m_spwm.M = m_PidInverter.OutPut;
        Change_M(&m_spwm);
        m_spwm.Change_Flag =1;
        m_spwm.Pid_Flag = 0;

        }



    }
}

# if DEBUG_INVERTER

    Float_Printf(Str,m_PidInverter.SetSpeed,1,3);
    UARTa_SendString((char *)",");
    Float_Printf(Str,m_PidInverter.ActualSpeed,1,3);
    UARTa_SendString((char *)"\n");

# endif


	A_Task_Ptr = &A1; 	// 跳去A1
}

//-----------------------------------------------------------
 /*
  * B任务分支
  * B1：
  */
//--------j---------------------------------------------------
void B1(void)
{
    float temp,temp1;
    temp = m_adc.Voltage_B0*32.137;
    temp1 = m_adc.Voltage_A0*39.44;

    Float2Str(Str,temp,2,2);
    OLED_ShowString(72,32,(unsigned char *)Str,8,1);
    Float2Str(Str,temp1,2,2);
    OLED_ShowString(72,56,(unsigned char *)Str,8,1);

//TODO: 监视频率是否正确
# if SCOPE_FRE
    GpioDataRegs.GPADAT.bit.GPIO21 = ~GpioDataRegs.GPADAT.bit.GPIO21;
# endif
    B_Task_Ptr = &B1;
}

#pragma CODE_SECTION(EPWM2_INT,"ramfuncs");

interrupt void EPWM2_INT(void)
{



//
//   if(m_PidBoost.SPwm_Start == 0)
//   {
    Uint16 CCR_Date=m_spwm.CMPA_data[m_spwm.Counter++];

    if(m_spwm.Counter>=200)
    {
        m_spwm.Counter=0;
        m_spwm.Pid_Flag=1;
    }

    EPwm2Regs.CMPA.half.CMPA=CCR_Date;
    EPwm2Regs.CMPB=CCR_Date;

    EPwm3Regs.CMPA.half.CMPA=CCR_Date;
    EPwm3Regs.CMPB=CCR_Date;

//   }
   // Clear INT flag for this timer
   EPwm2Regs.ETCLR.bit.INT = 1;
   // Acknowledge this interrupt to receive more interrupts from group 3
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

}


