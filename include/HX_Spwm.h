/*
 * HX_Spwm.h
 *
 *  Created on: 2021年9月14日
 *      Author: YellowNew
 */

#ifndef INCLUDE_HX_SPWM_H_
#define INCLUDE_HX_SPWM_H_

#include "main.h"

typedef struct{
    Uint16 N;                   // 载波比
    float frequency;           // 频率
    float M;                   // 调制比
    Uint16 Counter;
    Uint16 CMPA_data[400];
    Uint16 Change_Flag;
    Uint16 Pid_Flag;
    Uint32 Period;
}SPWM_VRB;


void Change_M(SPWM_VRB *Task1);
void Init_Spwm(SPWM_VRB *Init);



#endif /* INCLUDE_HX_SPWM_H_ */
