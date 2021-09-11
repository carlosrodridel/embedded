#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

SemaphoreHandle_t lcd; //recurso
char dato = 0;

void delay(int ms){    
    vTaskDelay(ms/portTICK_PERIOD_MS);
}

void task_productor(){
    while(1){
        xSemaphoreTake(lcd, portMAX_DELAY);
        printf("productor usando la LCD\r\n");        
        xSemaphoreGive(lcd);
        delay(1000);
    }     
}

void task_consumidor(){
    while(1){       
        if(xSemaphoreTake(lcd, 200/portMAX_DELAY) == pdPASS){
            printf("consumidor usando LCD\r\n");
            xSemaphoreGive(lcd);
            
        }else{
            printf("LCD ocupada\r\n");    
        }           
        delay(500);        
    }
}

void app_main() {

   lcd = xSemaphoreCreateMutex(); 
   xTaskCreate(task_productor, "Productor", 2000, NULL, 1, NULL);
   xTaskCreate(task_consumidor, "Consumidor", 2000, NULL, 2, NULL);
}
