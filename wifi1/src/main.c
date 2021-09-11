#define SSID "COOKIE 1003_NORMAL"
#define PASS "30127580"

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
//librerias para Wifi
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "esp_event.h"
//librerias para tcp
#include "netinet/tcp.h"
//librerias para lwip - adan dunkels
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"

xSemaphoreHandle reconnection;
xSemaphoreHandle internet;


static esp_err_t event_handler(void *ctx, system_event_t *event){
	printf("Id del evento -> %d\r\n", event->event_id);
	switch (event->event_id)
	{
	case SYSTEM_EVENT_STA_START:
		esp_wifi_connect();
		break;
	
	case SYSTEM_EVENT_STA_GOT_IP:
		xSemaphoreGive(internet);
		break;

	case SYSTEM_EVENT_STA_DISCONNECTED:
	    xSemaphoreGive(reconnection);
		break;

	default:
		break;
	}
	
	return(ESP_OK);
}

void task_reconnect(){	
	while(1){		
		xSemaphoreTake(reconnection, portMAX_DELAY);
		esp_wifi_connect();	
		while(xSemaphoreTake(internet, 5000/portTICK_PERIOD_MS != pdPASS)){
			printf("Conecting...\r\n");
			esp_wifi_connect();		
		}			
	}
}

void task_tcp_client(){
	while(1){
		int sock = 0;
		char request[] = "Esp32 2021\r\n";				
		printf("Esperando Internet...\r\n");
		xSemaphoreTake(internet, portMAX_DELAY);
		printf("Conectado a Internet...\r\n");

		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr("192.168.1.5");
		addr.sin_port = htons(12000);

		sock = socket(AF_INET, SOCK_STREAM, 0);

		if(connect(sock, (struct sockaddr*)&addr, sizeof(addr)) >= 0){
			printf("Conectado con servidor 192.168.20.22\r\n");			
			while(1){
				send(sock, request, strlen(request), 0);
				vTaskDelay(5000/portTICK_PERIOD_MS);
			}
		}
		else{
			printf("Error de conexión\r\n");
		}
	}
}

void app_main()
{
    reconnection = xSemaphoreCreateBinary();
	internet = xSemaphoreCreateBinary();

	ESP_ERROR_CHECK(nvs_flash_init()); //Inicialización non volatile storage
	//stack protocolo TCP
	tcpip_adapter_init();

	ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL)); //Presentación event handler
	
	/////Configuración del WiFi//////
	wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));
	ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

	//////Configuracion de la red////
	wifi_config_t wifi_config = {
		.sta = {
			.ssid = SSID,
			.password = PASS, //password
		},
	};

	ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

	ESP_ERROR_CHECK(tcpip_adapter_set_hostname(TCPIP_ADAPTER_IF_STA, "EspCarlosR")); //Cambiar Nombre Host

	xTaskCreate(task_tcp_client, "Consumidor", 2000, NULL, 1, NULL);
	xTaskCreate(task_reconnect, "Recconect", 2000, NULL, 2, NULL);
}