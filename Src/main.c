#include "main.h"
#include "stm32f0xx_hal.h"
#include <stdio.h>
#include <stdlib.h>

#define ADC_V1_OUT_CHANNEL ADC_CHANNEL_2
#define ADC_V2_OUT_CHANNEL ADC_CHANNEL_1
#define ADC_V3_OUT_CHANNEL ADC_CHANNEL_4
#define ADC_V4_OUT_CHANNEL ADC_CHANNEL_3
#define ADC_TEMP_OUT_CHANNEL ADC_CHANNEL_6
#define ADC_CUR_OUT_CHANNEL ADC_CHANNEL_7

#define TEMP_OFF 500

#define VREF_3v3_CONST 1532
#define VREF_COEFFICIENT (3300 * VREF_3v3_CONST) / 4095

#define HT_LED_ON()     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET)
#define HT_LED_OFF()     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET)
#define OC_LED_ON() 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET)
#define OC_LED_OFF() 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET)
#define RELAY_PIN_ON()  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET)
#define RELAY_PIN_OFF()  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET)
#define LV_LED_ON()  	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET)
#define LV_LED_OFF()  	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET)

ADC_HandleTypeDef hadc;
DMA_HandleTypeDef hdma_adc;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC_Init(void);
static void MX_DMA_Init(void);

void ledAnimation(void);

float calculateTemperature(uint32_t temp);
void checkTemperature(uint32_t temp);

float calculateCurrent(uint32_t current_adc);
void checkCurrent(float current);

float base_current;

typedef struct Values {
	float temperature;
	float current;
	float v1;
	float v2;
	float v3;
	float v4;
} Values;

enum buffer_index {
	V3, V4, V1, V2, TEMP_INDEX, CUR_INDEX, VREF_INDEX,
};

uint32_t buffer[7];

float scale_multiplier;

uint32_t vref = 0;

__IO Values ADCValues;
__IO Values ExactValues;

int temp_flag = 0;
int overcur_flag = 0;

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
	if (__HAL_ADC_GET_FLAG(hadc, ADC_FLAG_EOS)) {
		vref = buffer[VREF_INDEX];
		scale_multiplier = (float) VREF_COEFFICIENT / vref;
		ADCValues.temperature = buffer[TEMP_INDEX];
		ADCValues.current = buffer[CUR_INDEX];
		ADCValues.v1 = buffer[V1];
		ADCValues.v2 = buffer[V2];
		ADCValues.v3 = buffer[V3];
		ADCValues.v4 = buffer[V4];
	}
}

int main(void) {
	HAL_Init();

	SystemClock_Config();

	MX_GPIO_Init();
	MX_DMA_Init();
	MX_ADC_Init();


	HAL_ADC_Start_DMA(&hadc, buffer, 7);

	ledAnimation();

	base_current = (buffer[CUR_INDEX])*(scale_multiplier);

	RELAY_PIN_ON();

	while (1) {

		ExactValues.temperature = calculateTemperature(buffer[TEMP_INDEX]);
		checkTemperature(ExactValues.temperature);
		ExactValues.current = calculateCurrent(ADCValues.current);
		checkCurrent(ExactValues.current);

		if (overcur_flag) {
			RELAY_PIN_OFF();
			OC_LED_ON();
		}

		if (temp_flag) {
			RELAY_PIN_OFF();
			HT_LED_ON();
		}
	}
}

float calculateTemperature(uint32_t temp) {
	if (temp > 0) {
		return (((temp) * (scale_multiplier)) - (TEMP_OFF)) / (10);
	}
	return 0;
}

void checkTemperature(uint32_t temp) {
	if (temp > 70) {
		temp_flag = 1;
	}
}

void checkCurrent(float current) {
	if (current > 3) {
		overcur_flag = 1;
	}
}

float calculateCurrent(uint32_t current_adc) {
	return ((current_adc * scale_multiplier) - (base_current)) / 66;
}


void ledAnimation(void) {
	for (int i = 0; i < 3; i++) {
		OC_LED_ON();
		HAL_Delay(50);
		HT_LED_ON();
		HAL_Delay(50);
		LV_LED_ON();
		HAL_Delay(50);
		LV_LED_OFF();
		HAL_Delay(50);
		HT_LED_OFF();
		HAL_Delay(50);
		OC_LED_OFF();
		HAL_Delay(50);
	}

	for (int i = 0; i < 3; i++) {
		OC_LED_ON();
		HT_LED_ON();
		LV_LED_ON();
		HAL_Delay(100);
		LV_LED_OFF();
		HT_LED_OFF();
		OC_LED_OFF();
		HAL_Delay(100);
	}
}

void SystemClock_Config(void) {

	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI
			| RCC_OSCILLATORTYPE_HSI14;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
	RCC_OscInitStruct.HSICalibrationValue = 16;
	RCC_OscInitStruct.HSI14CalibrationValue = 16;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

static void MX_DMA_Init(void) {
	__HAL_RCC_DMA1_CLK_ENABLE()
	;

	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

static void MX_ADC_Init(void) {

	ADC_ChannelConfTypeDef sConfig;

	hadc.Instance = ADC1;
	hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	hadc.Init.Resolution = ADC_RESOLUTION_12B;
	hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc.Init.ScanConvMode = ENABLE;
	hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc.Init.LowPowerAutoWait = DISABLE;
	hadc.Init.LowPowerAutoPowerOff = DISABLE;
	hadc.Init.ContinuousConvMode = ENABLE;
	hadc.Init.DiscontinuousConvMode = DISABLE;
	hadc.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC4;
	hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc.Init.DMAContinuousRequests = ENABLE;
	hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	hadc.DMA_Handle = &hdma_adc;

	HAL_ADC_Init(&hadc);

	sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;

	sConfig.Channel = ADC_CHANNEL_VREFINT;
	sConfig.Rank = 1;
	HAL_ADC_ConfigChannel(&hadc, &sConfig);

	sConfig.Channel = ADC_TEMP_OUT_CHANNEL;
	sConfig.Rank = 2;
	HAL_ADC_ConfigChannel(&hadc, &sConfig);

	sConfig.Channel = ADC_CUR_OUT_CHANNEL;
	sConfig.Rank = 3;
	HAL_ADC_ConfigChannel(&hadc, &sConfig);

	sConfig.Channel = ADC_V1_OUT_CHANNEL;
	sConfig.Rank = 4;
	HAL_ADC_ConfigChannel(&hadc, &sConfig);

	sConfig.Channel = ADC_V2_OUT_CHANNEL;
	sConfig.Rank = 5;
	HAL_ADC_ConfigChannel(&hadc, &sConfig);

	sConfig.Channel = ADC_V3_OUT_CHANNEL;
	sConfig.Rank = 6;
	HAL_ADC_ConfigChannel(&hadc, &sConfig);

	sConfig.Channel = ADC_V4_OUT_CHANNEL;
	sConfig.Rank = 7;
	HAL_ADC_ConfigChannel(&hadc, &sConfig);

}

static void MX_GPIO_Init(void) {

	GPIO_InitTypeDef GPIO_InitStruct;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5, GPIO_PIN_RESET);

	GPIO_InitStruct.Pin = GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

void _Error_Handler(char *file, int line) {
	while (1) {
	}
}
