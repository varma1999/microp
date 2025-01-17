/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <string.h>
#include <stdlib.h>


#define GPIO_OUTPUT_IO_0    2
#define GPIO_OUTPUT_PIN_SEL (1ULL<<GPIO_OUTPUT_IO_0)
//#define GPIO_INPUT_IO_1     0
//#define GPIO_INPUT_PIN_SEL  (1ULL<<GPIO_INPUT_IO_1)

SemaphoreHandle_t xMutexSemaphore;
xBinarySemaphore xSemaphoreCreateMutex();

static void gpio_isr_handler(void *arg)
{
	uint32_t gpio_num = (uint32_t)arg;
	xSemaphoreGiveFromISR(xMutexSemaphore, signed BaseType_t *pxHigherPriorityTaskWoken);
}

static void gpio_task_example(void *arg)
{
	uint32_t io_num;

	for (;;) {
		if (xSemaphoreTake(xMutexSemaphore, portMAX_DELAY) == pdTRUE)
		{
			xSemaphoreGiveFromISR(xMutexSemaphore);
		}
	}
}

void app_main()
{
	gpio_config_t io_conf;
	//disable interrupt
	io_conf.intr_type = GPIO_INTR_DISABLE;
	//set as output mode
	io_conf.mode = GPIO_MODE_OUTPUT;
	//bit mask of the pins that you want to set,e.g.GPIO15/16
	io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
	//disable pull-down mode
	io_conf.pull_down_en = 0;
	//disable pull-up mode
	io_conf.pull_up_en = 0;
	//configure GPIO with the given settings
	gpio_config(&io_conf);

	void gpio_task_example()
	{
		for (;; )
		{

			gpio_set_level(GPIO_OUTPUT_IO_0, 0);
			vTaskDelay(1000 / portTICK_PERIOD_MS);
		}
	}

	void gpio_task_example2()
	{
		for (;; )
		{

			gpio_set_level(GPIO_OUTPUT_IO_0, 1);
			vTaskDelay(1000 / portTICK_PERIOD_MS);
		}
	}

	void gpio_task_example3()
	{
		for (;; )
		{

			printf("Status message printed");
			vTaskDelay(1000 / portTICK_PERIOD_MS);
		}
	}

	xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL);
	xTaskCreate(gpio_task_example2, "gpio_task_example2", 2048, NULL, 9, NULL);
	xTaskCreate(gpio_task_example3, "gpio_task_example3", 2048, NULL, 8, NULL);


	

 
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}
