#ifndef _ADC_CONFIG_H
#define _ADC_CONFIG_H
#include "stm32f0xx_hal.h"

#define ADC_V1_OUT_CHANNEL ADC_CHANNEL_2
#define ADC_V2_OUT_CHANNEL ADC_CHANNEL_1
#define ADC_V3_OUT_CHANNEL ADC_CHANNEL_4
#define ADC_V4_OUT_CHANNEL ADC_CHANNEL_3
#define ADC_TEMP_OUT_CHANNEL ADC_CHANNEL_6
#define ADC_CUR_OUT_CHANNEL ADC_CHANNEL_7

DMA_HandleTypeDef hdma_adc;

void ADC_Init(ADC_HandleTypeDef* hadc);

#endif
