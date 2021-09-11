#include <stdio.h>


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define mainDELAY_LOOP_COUNT        ( 0xffffff )


void delay_ms(int ms){
    vTaskDelay(ms/portTICK_RATE_MS);
}

void task1(){

    volatile unsigned long ul;
    
    portTickType xLastWakeTime;

    /* The xLastWakeTime variable needs to be initialized with the current tick
    count. Note that this is the only time the variable is written to explicitly.
    After this xLastWakeTime is updated automatically internally within
    vTaskDelayUntil(). */
    xLastWakeTime = xTaskGetTickCount();

    while (1)
    {
        printf("task 1 executing\r\n");
        /* This task should execute exactly every 250 milliseconds. As per
        the vTaskDelay() function, time is measured in ticks, and the
        portTICK_RATE_MS constant is used to convert milliseconds into ticks.
        xLastWakeTime is automatically updated within vTaskDelayUntil() so is not
        explicitly updated by the task. */
        vTaskDelayUntil( &xLastWakeTime, ( 500 / portTICK_RATE_MS ) );
    }
    
}

void task2 (){

    volatile unsigned long ul;
    portTickType xLastWakeTime;

    xLastWakeTime = xTaskGetTickCount();
    
    while (1)
    {
        printf("task 2 executing\r\n");
        vTaskDelayUntil( &xLastWakeTime, ( 500 / portTICK_RATE_MS ) );
        
    }
    
}

void app_main() {

    xTaskCreate(task1, "tarea 1", 1000, NULL, 1, NULL);
    xTaskCreate(task2, "tarea 2", 1000, NULL, 2, NULL);
    
}
