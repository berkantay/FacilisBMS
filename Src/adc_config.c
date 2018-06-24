#include "adc_config.h"

void ADC_Init(ADC_HandleTypeDef *hadc) {




	ADC_ChannelConfTypeDef sConfig;

	hadc->Instance = ADC1;
	hadc->Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	hadc->Init.Resolution = ADC_RESOLUTION_12B;
	hadc->Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc->Init.ScanConvMode = ENABLE;
	hadc->Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc->Init.LowPowerAutoWait = DISABLE;
	hadc->Init.LowPowerAutoPowerOff = DISABLE;
	hadc->Init.ContinuousConvMode = ENABLE;
	hadc->Init.DiscontinuousConvMode = DISABLE;
	hadc->Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC4;
	hadc->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc->Init.DMAContinuousRequests = ENABLE;
	hadc->Init.Overrun = ADC_OVR_DATA_PRESERVED;
	hadc->DMA_Handle = &hdma_adc;

	HAL_ADC_Init(hadc);

	sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;

	sConfig.Channel = ADC_CHANNEL_VREFINT;
	sConfig.Rank = 1;
	HAL_ADC_ConfigChannel(hadc, &sConfig);

	sConfig.Channel = ADC_TEMP_OUT_CHANNEL;
	sConfig.Rank = 2;
	HAL_ADC_ConfigChannel(hadc, &sConfig);

	sConfig.Channel = ADC_CUR_OUT_CHANNEL;
	sConfig.Rank = 3;
	HAL_ADC_ConfigChannel(hadc, &sConfig);

	sConfig.Channel = ADC_V1_OUT_CHANNEL;
	sConfig.Rank = 4;
	HAL_ADC_ConfigChannel(hadc, &sConfig);

	sConfig.Channel = ADC_V2_OUT_CHANNEL;
	sConfig.Rank = 5;
	HAL_ADC_ConfigChannel(hadc, &sConfig);

	sConfig.Channel = ADC_V3_OUT_CHANNEL;
	sConfig.Rank = 6;
	HAL_ADC_ConfigChannel(hadc, &sConfig);

	sConfig.Channel = ADC_V4_OUT_CHANNEL;
	sConfig.Rank = 7;
	HAL_ADC_ConfigChannel(hadc, &sConfig);

}

