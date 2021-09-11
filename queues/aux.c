#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

xQueueHandle cola;


void delay_ms(int ms){
    vTaskDelay(ms/portTICK_PERIOD_MS);
}

void task_productor(){
    char dato1=5, dato2=10, dato3=15;
    while (1)
    {
        printf("enviando dato 1\r\n");
        xQueueSend(cola, &dato1, portMAX_DELAY);
        printf("enviando dato 2\r\n");
        xQueueSend(cola, &dato2, portMAX_DELAY);
        printf("enviando dato 3\r\n");
        xQueueSend(cola, &dato3, portMAX_DELAY);
        delay_ms(1000);
    }
    
}

void task_consumidor(){
    char i=0;
    char dato=0;
    while (1)
    {
        for ( i = 0; i < 3; i++)
        {       
            xQueueReceive(cola, &dato, portMAX_DELAY);
            printf("dato recivido= %d \r\n", dato);
        }
        
    }
    
}

void app_main() {
    
    cola = xQueueCreate(3, sizeof(char));
    xTaskCreatePinnedToCore(task_productor,"Productor", 2000, NULL, 1, NULL,1);
    xTaskCreatePinnedToCore(task_consumidor,"Consumidor", 2000, NULL, 2, NULL,1);

}