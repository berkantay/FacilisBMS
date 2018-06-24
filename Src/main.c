#include "main.h"
#include "stm32f0xx_hal.h"
#include "gpio_config.h"
#include "gpio.h"
#include "adc_config.h"
#include "dma_config.h"
#include <stdbool.h>
#include "systemclock_config.h"
#include "adc.h"



#define TEMP_OFF 500
#define VREF_3v3_CONST 1532
#define VREF_COEFFICIENT (3300 * VREF_3v3_CONST) / 4095

__IO Values ExactValues;

ADC_HandleTypeDef hadc;

void SystemClock_Config(void);
void ledAnimation(void);

float base_current;

float calculateTemperature(uint32_t temp);
bool checkTemperature(uint32_t temp);

float calculateCurrent(uint32_t current_adc);
bool checkCurrent(float current);

float calculate_vref(uint32_t vref_adc) {

	return (float) VREF_COEFFICIENT / vref_adc;

}

int main(void) {
	HAL_Init();
	SystemClock_Config();
	GPIO_Init();
	DMA_Init();
	ADC_Init(&hadc);
	HAL_ADC_Start_DMA(&hadc, buffer, 7);
	ledAnimation();

	base_current = (buffer[CUR_INDEX]) * (calculate_vref(ADCValues.vref));

	RELAY_PIN_ON();

	while (1) {

		ExactValues.vref = calculate_vref(buffer[VREF_INDEX]);
		ExactValues.temperature = calculateTemperature(buffer[TEMP_INDEX]);
		checkTemperature(ExactValues.temperature);
		ExactValues.current = calculateCurrent(ADCValues.current);
		checkCurrent(ExactValues.current);

		if (checkCurrent(ExactValues.current)) {
			RELAY_PIN_OFF();
			OC_LED_ON();
		}

		if (checkTemperature(ExactValues.temperature)) {
			RELAY_PIN_OFF();
			HT_LED_ON();
		}
	}
}

float calculateTemperature(uint32_t temp) {
	if (temp > 0) {
		return (((temp) * (ExactValues.vref)) - (TEMP_OFF)) / (10);
	}
	return 0;
}

bool checkTemperature(uint32_t temp) {
	return temp > 70;
}

bool checkCurrent(float current) {
	return current > 20;
}

float calculateCurrent(uint32_t current_adc) {
	return ((current_adc * ExactValues.vref) - (base_current)) / 66;
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


