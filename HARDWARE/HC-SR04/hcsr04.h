#ifndef __HCSR04_H
#define __HCSR04_H
#include "sys.h"

extern u32 overflow_cnt;

void UltrasonicWave_Init(void);
void UltrasonicWave_StartMeasure(void);
double UltrasonicWave_Measure(void);
#endif

