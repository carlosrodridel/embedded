#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

typedef struct 
{
    char id_task;
    char dato; 
}pkt;


xQueueHandle cola;


void delay_ms(int ms){
    vTaskDelay(ms/portTICK_PERIOD_MS);
}

void task_productor(){
    pkt data;
    data.id_task=1;
    data.dato =5;
    while (1)
    {
        xQueueSend(cola, &data, portMAX_DELAY);
        delay_ms(1000);
    }
}

void task_productor2(){
    
    pkt data;
    data.id_task=2;
    data.dato =15;
    while (1)
    {
        xQueueSend(cola, &data, portMAX_DELAY);
        delay_ms(1000);
    }
}

void task_consumidor(){
    pkt dato;
    while (1)
    {
        xQueueReceive(cola, &dato, portMAX_DELAY);
        printf("dato recivido desde tarea %d = %d \r\n", dato.id_task, dato.dato);        
    }
    
}

void app_main() {
    
    cola = xQueueCreate(3, sizeof(pkt));
    xTaskCreatePinnedToCore(task_productor,"Productor", 2000, NULL, 2, NULL,1);
    xTaskCreatePinnedToCore(task_productor2,"Productor2", 2000, NULL, 2, NULL,1);
    xTaskCreatePinnedToCore(task_consumidor,"Consumidor", 2000, NULL, 1, NULL,1);

}