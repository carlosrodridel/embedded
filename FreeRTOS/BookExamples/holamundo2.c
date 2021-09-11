#include <stdio.h>


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void delay_ms(int ms){
    vTaskDelay(ms/portTICK_PERIOD_MS);
}

void vTaskFunction( void *msj ){

    char *innermsj;
    //volatile unsigned long ul;
    
    /* The string to print out is passed in via the parameter. Cast this to a
    character pointer. */
    innermsj = ( char * ) msj;
    
    /* As per most tasks, this task is implemented in an infinite loop. */
    while(1){
        /* Print out the name of this task. */
        printf(innermsj);
        /* Delay for a period. */
        delay_ms(1000);
        /* This loop is just a very crude delay implementation. There is
        nothing to do in here. Later exercises will replace this crude
        loop with a proper delay/sleep function. */
    }
}

/* Define the strings that will be passed in as the task parameters. These are
defined const and not on the stack to ensure they remain valid when the tasks are
executing. */
static const char *pcTextForTask1 = "Task 1 is running\r\n";
static const char *pcTextForTask2 = "Task 2 is running\t\n";

int main(){

    xTaskCreate(vTaskFunction, "tarea numero 1", 1000, (void*) pcTextForTask1, 1, NULL);
    xTaskCreate(vTaskFunction, "tarea numero 2", 1000, (void*) pcTextForTask2, 1, NULL);
}
