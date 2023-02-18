/**********************************************************************
* File: F28335_example.h
* Device: TMS320F28335
* Author: David M. Alter, Texas Instruments Inc.
* Description: Include file for example project.  Include this file in
*   all C-source files.
* Notes:
*   1) The following constants may be defined in the CCS project build
*      options:
*        EXAMPLE_NONBIOS
*        EXAMPLE_BIOS
*        EXAMPLE_FLASH
*        EXAMPLE_RAM
* History:
*   03/15/08 - Added prototype for InitXintf() (D. Alter)
*   12/13/07 - original (D. Alter)
**********************************************************************/

#ifndef F28335_EXAMPLE_H
#define F28335_EXAMPLE_H


//---------------------------------------------------------------------------
// Constant Definitions
//
#define ADC_BUF_LEN			50				// ADC buffer length
#define ADC_SAMPLE_PERIOD	2999			// 2999 = 50 KHz sampling w/ 150 MHz SYSCLKOUT
#define PWM_HALF_PERIOD		37500			// period/2 for 2 kHz symmetric PWM
#define PWM_DUTY_CYCLE		28125			// 25% duty cycle


//---------------------------------------------------------------------------
// Include Standard C Language Header Files
//
#include <string.h>

#ifndef EXAMPLE_NONBIOS
	#define EXAMPLE_NONBIOS
#endif
#ifndef EXAMPLE_FLASH
	#define EXAMPLE_FLASH
#endif
//---------------------------------------------------------------------------
// Include any other Header Files
//
#ifdef EXAMPLE_NONBIOS
	#include "DSP2833x_DefaultIsr.h"		// ISR definitions
#endif

#ifdef EXAMPLE_BIOS
	#ifdef EXAMPLE_FLASH
	    #include "F28335_example_BIOS_flashcfg.h"
	#endif

	#ifdef EXAMPLE_RAM
	    #include "F28335_example_BIOS_ramcfg.h"
	#endif
#endif


//---------------------------------------------------------------------------
// Function Prototypes
//
extern void DelayUs(Uint16);
extern void InitAdc(void);
extern void InitECap(void);
extern void InitEPwm(void);

#ifdef EXAMPLE_FLASH
	extern void InitFlash(void);
#endif

extern void InitGpio(void);
extern void InitPieCtrl(void);
extern void InitSysCtrl(void);
extern void InitWatchdog(void);
extern void InitXintf(void);
extern void SetDBGIER(Uint16);

#ifdef EXAMPLE_BIOS
	extern void UserInit(void);
#endif


//---------------------------------------------------------------------------
// Global symbols defined in the linker command file
//
#ifdef EXAMPLE_BIOS
	extern Uint16 hwi_vec_loadstart;
	extern Uint16 hwi_vec_loadend;
	extern Uint16 hwi_vec_runstart;
	extern Uint16 trcdata_loadstart;
	extern Uint16 trcdata_loadend;
	extern Uint16 trcdata_runstart;
#endif

#ifdef EXAMPLE_FLASH
	extern Uint16 secureRamFuncs_loadstart;
	extern Uint16 secureRamFuncs_loadend;
	extern Uint16 secureRamFuncs_runstart;
#endif


//---------------------------------------------------------------------------
// Global Variable References
//
extern Uint16 AdcBuf[ADC_BUF_LEN];			// ADC data buffer allocation
extern Uint32 PwmDuty;						// Measured PWM duty cycle
extern Uint32 PwmPeriod;					// Measured PWM period

#ifdef EXAMPLE_NONBIOS
	extern const struct PIE_VECT_TABLE PieVectTableInit;	// Pie vector table (non-BIOS only)
#endif


//---------------------------------------------------------------------------
// Macros
//
#define ADC_cal_func_ptr (void (*)(void))0x380080


//---------------------------------------------------------------------------
#endif  // end of F28335_EXAMPLE_H definition


//--- end of file -----------------------------------------------------
