#include <stdio.h>


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void delay_ms(int ms){
    vTaskDelay(ms/portTICK_PERIOD_MS);
}

void task1(){
    while (1)
    {
        printf("task 1 executing\r\n");
        delay_ms(1000);
    }
    
}

void task2 (){
    while (1)
    {
        printf("task 2 executing\r\n");
        delay_ms(1000);
    }
    
}

void app_main() {

    xTaskCreate(task1, "tarea 1", 1000, NULL, 1, NULL);
    xTaskCreate(task2, "tarea 2", 1000, NULL, 1, NULL);
    
}
