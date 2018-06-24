#ifndef _ADC_H
#define _ADC_H
#include "stm32f0xx_hal.h"

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);



typedef struct Values {
	float temperature;
	float current;
	float v1;
	float v2;
	float v3;
	float v4;
	uint32_t vref ;
} Values;

enum buffer_index {
	V3, V4, V1, V2, TEMP_INDEX, CUR_INDEX, VREF_INDEX,
};

uint32_t buffer[7];
__IO Values ADCValues;

#endif
