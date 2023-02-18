/*
 * Project_28335_InitSystem.c
 *
 *  Created on: 2021年9月14日
 *      Author: YellowNew
 */

/* 头文件包含 */
#include "Project_28335_InitSystem.h"

/* 函数体 */
void InitSystem(void)
{
// 初始化系统：
	InitSysCtrl();

// 清除所有的中断和初始化PIE向量表：
	DINT;
	InitPieCtrl();
	IER = 0x0000;
	IFR = 0x0000;
	InitPieVectTable();
#if  ROM
	{
		MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
		InitFlash();
	}
#endif
}
