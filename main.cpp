/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"

// FREERTOS
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
// CURRENT include
#include "i2c_scan.h"
#include "lcd.h"
#include "onboard_temp.h"
#include "grove_map.h"

#include <iostream>
#include <vector>

void setup(void)
{
	stdio_init_all();
	adc_gpio_init(ADC_SENSOR_SOUND_PIN);
	adc_gpio_init(ADC_SENSOR_LIGHT_PIN);
	adc_init();
	adc_set_temp_sensor_enabled(true);
	sleep_ms(1000);
	lcd_init(i2c1, LCD_I2C_ADDRESS, LCD_RGB_ADDRESS);
	i2c_scan(i2c1);
	lcd_set_cursor(0,0);
}

void vDisplay_onboard_temperature(void * args)
{
	std::vector<float> t;
	for (;;)
	{
		lcd_clear();
		adc_select_input(4);
		float temperature = read_onboard_temperature('C');
		t.push_back(temperature);
		char str[20];
		char countstr[10];
		sprintf(str, "%.2f", temperature);
		sprintf(countstr, "%d", t.size());
	    	char * str_temp = strstr(str, ".00");
	    	if (str_temp) str_temp = 0x0;
		lcd_print("temp: ");
	    	lcd_print(str);
		//lcd_print("\nCount: ");
		//lcd_print(countstr);
		vTaskDelay(1000);
	}
}

void vDisplayHello(void * args) {
	for (;;)
	{
		lcd_clear();
		lcd_print("Hello\n FreeRTOS");
		vTaskDelay(1500);
	}
}

void vGetSoundSample(void * args)
{
	TickType_t last_time_tick;
	TickType_t period = 100;

        last_time_tick = xTaskGetTickCount();
        for(;;)
	{
		vTaskDelayUntil(&last_time_tick, period);
		printf("Sample sound: ");
		lcd_clear();
		adc_select_input(0);
		uint adc_raw = adc_read();
		char str[20];
		sprintf(str, "%.2f", adc_raw * ADC_SENSOR_SOUND_CONVERT);
		printf("%s\n", str);
		lcd_print("Sample:\n");
		lcd_print(str);
		vTaskDelay(4000);
	}
}

void vGetLightSample(void * args)
{
	for (;;)
	{
		printf("Light sensor: ");
		lcd_clear();
		adc_select_input(1);
		uint adc_raw = adc_read();
		char str[20];
		sprintf(str, "%.2f", adc_raw * ADC_SENSOR_SOUND_CONVERT);
		printf("%s\n", str);
		lcd_print("Light sample:\n");
		lcd_print(str);
		vTaskDelay(2700);
	}
}
int main(int argc, char ** argv)
{
	setup();
	xTaskCreate(vDisplayHello, "RTOS Hello", 128, NULL, 2, NULL);
	//xTaskCreate(vGetSoundSample, "RTOS Sound Sensor", 128, NULL, 1, NULL);
	//xTaskCreate(vGetLightSample, "RTOS Light Sensor", 128, NULL, 1, NULL);
	xTaskCreate(vDisplay_onboard_temperature, "RTOS Temp", 128, NULL, 1, NULL);
	vTaskStartScheduler();
}
