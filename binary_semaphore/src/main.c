#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

SemaphoreHandle_t binary_semaphore;
char dato=0;

void delay_ms(int ms){
    vTaskDelay(ms/portTICK_PERIOD_MS);
}

void task_productor(){
    while (1)
    {
        printf("iniciando procesamiento...\r\n");
        dato++;
        xSemaphoreGive(binary_semaphore);
        printf("dato entregado\r\n");
        delay_ms(1000);
    }
    
}

void task_consumidor(){
    while (1)
    {
        if (xSemaphoreTake(binary_semaphore,(200/portTICK_PERIOD_MS))==pdPASS)
        {
            printf("dato consumido= %d\r\n", dato);
        }else
        {
            printf("Timeout\r\n");
        }
        
    }
    
}

void app_main() {
    binary_semaphore = xSemaphoreCreateBinary();
    xTaskCreate(task_productor,"Productor", 2000, NULL, 2, NULL);
    xTaskCreate(task_consumidor,"Consumidor", 2000, NULL, 1, NULL);

}