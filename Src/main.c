#include "main.h"
#include "stm32f0xx_hal.h"

#define HT_LED_ON()     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET)
#define OC_LED_ON() 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET)
#define RELAY_PIN_ON()  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET)
#define LV_LED_ON()  	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET)

ADC_HandleTypeDef hadc;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC_Init(void);

uint32_t temp = 0;

int main(void) {
	HAL_Init();

	SystemClock_Config();
	MX_GPIO_Init();
	MX_ADC_Init();

	HAL_ADC_Start(&hadc);
	while (1) {
		if (HAL_ADC_PollForConversion(&hadc, 5) == HAL_OK) {
			temp = HAL_ADC_GetValue(&hadc);
		} else {
			HT_LED_ON();
		}
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

static void MX_ADC_Init(void) {

	ADC_ChannelConfTypeDef sConfig;

	hadc.Instance = ADC1;
	hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	hadc.Init.Resolution = ADC_RESOLUTION_12B;
	hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc.Init.ScanConvMode = DISABLE;
	hadc.Init.EOCSelection = DISABLE;
	hadc.Init.LowPowerAutoWait = DISABLE;
	hadc.Init.LowPowerAutoPowerOff = DISABLE;
	hadc.Init.ContinuousConvMode = ENABLE;
	hadc.Init.DiscontinuousConvMode = DISABLE;
	hadc.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC4;
	hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc.Init.DMAContinuousRequests = DISABLE;
	hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	if (HAL_ADC_Init(&hadc) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	sConfig.Channel = ADC_CHANNEL_1;
	sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
	sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
//	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
//		_Error_Handler(__FILE__, __LINE__);
//	}
//
//	/**Configure for the selected ADC regular channel to be converted.
//	 */
//	sConfig.Channel = ADC_CHANNEL_2;
//	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
//		_Error_Handler(__FILE__, __LINE__);
//	}
//
//	/**Configure for the selected ADC regular channel to be converted.
//	 */
//	sConfig.Channel = ADC_CHANNEL_3;
//	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
//		_Error_Handler(__FILE__, __LINE__);
//	}
//
//	/**Configure for the selected ADC regular channel to be converted.
//	 */
//	sConfig.Channel = ADC_CHANNEL_4;
//	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
//		_Error_Handler(__FILE__, __LINE__);
//	}

	/**Configure for the selected ADC regular channel to be converted.
	 */
	sConfig.Channel = ADC_CHANNEL_6;
	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	/**Configure for the selected ADC regular channel to be converted.
	 */
//	sConfig.Channel = ADC_CHANNEL_7;
//	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
//		_Error_Handler(__FILE__, __LINE__);
//	}

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
