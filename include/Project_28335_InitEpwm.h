/*
 * Project_28335_InitEpwm.h
 *
 *  Created on: 2021年9月14日
 *      Author: YellowNew
 */

#ifndef INCLUDE_PROJECT_28335_INITEPWM_H_
#define INCLUDE_PROJECT_28335_INITEPWM_H_

/* 头文件包含 */
#include "main.h"

/*宏定义*/
#define PWM_CH 4

void InitEpwm(void);
void  Config_Epwm23();;
void Config_EPwm4(void);
interrupt void EPWM2_INT(void);

#endif /* INCLUDE_PROJECT_28335_INITEPWM_H_ */
