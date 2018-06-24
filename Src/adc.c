#include "adc.h"

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
	if (__HAL_ADC_GET_FLAG(hadc, ADC_FLAG_EOS)) {

		ADCValues.vref = buffer[VREF_INDEX];
		ADCValues.temperature = buffer[TEMP_INDEX];
		ADCValues.current = buffer[CUR_INDEX];
		ADCValues.v1 = buffer[V1];
		ADCValues.v2 = buffer[V2];
		ADCValues.v3 = buffer[V3];
		ADCValues.v4 = buffer[V4];
	}
}
