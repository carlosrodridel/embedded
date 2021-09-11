#include <stdio.h>


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define mainDELAY_LOOP_COUNT        ( 0xffffff )


void delay_ms(int ms){
    vTaskDelay(ms/portTICK_RATE_MS);
}

void task1(){
    
    while (1)
    {
        printf("task 1 executing\r\n");
        
    }
    
}

void task2 (){
    
    while (1)
    {
        printf("task 2 executing\r\n");

    }
    
}

void task3(){

    portTickType xLastWakeTime;

    xLastWakeTime = xTaskGetTickCount();

    while (1)
    {
        printf("task 3 periodically executing\r\n");
        
        vTaskDelayUntil(&xLastWakeTime, (200/ portTICK_RATE_MS));
    
    }
    
}

void app_main() {
    
    xTaskCreatePinnedToCore(task1, "tarea 1", 1000, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(task2, "tarea 2", 1000, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(task3, "periodic task", 1000, NULL, 2, NULL, 1);
    
}