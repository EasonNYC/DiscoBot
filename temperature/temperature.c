#include "stm32f4xx_adc.h"

// initialize the temperature sensor
void init_temperature_sensor(){
	ADC_InitTypeDef ADC_InitStruct;
	ADC_CommonInitTypeDef ADC_CommonInitStruct;

	// RCC Initialization
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	// Common ADC Initialization
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div8;
	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStruct);

	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStruct);

	// ADC Channel 1 Configuration
	ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor, 1, ADC_SampleTime_144Cycles);

	// Enable Temperature Sensor
	ADC_TempSensorVrefintCmd(ENABLE);

	// Enable ADC
	ADC_Cmd(ADC1, ENABLE);
}


// get one temperature reading (in degrees C)
float read_temperature_sensor(){
	float temp; // temperature reading

	ADC_SoftwareStartConv(ADC1); //Start the conversion

	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); //Wait for ADC conversion to finish
	temp = (float)ADC_GetConversionValue(ADC1); //Get one ADC reading -- as a number from 0 to 4095 (12 bits)

	// Convert ADC reading to voltage
	temp /= 4095.0f; 
	temp *= 3.3f;	

	temp -= 0.760f; 
	temp /= .0025f; 

	temp += 25.0f; 

	return temp;
}
