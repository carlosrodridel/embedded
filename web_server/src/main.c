#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "string.h"

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"

#include "nvs_flash.h"
#include "esp_vfs.h"
#include "esp_vfs_fat.h"

#include "tcpip_adapter.h"
#include "esp_event.h"

#define WIFI_SSID "linux"
#define WIFI_PASS "s5F1iqZM"

static wl_handle_t s_wl_handle = WL_INVALID_HANDLE;

xSemaphoreHandle client_connected;

char html[]="<!DOCTYPE html> <html lang=\"en\"> <head> <meta charset=\"UTF-8\"> <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> <script src=\"https://unpkg.com/scrollreveal\"></script> <script src=\"https://kit.fontawesome.com/cef798d11a.js\" crossorigin=\"anonymous\"></script> <link rel=\"preconnect\" href=\"https://fonts.gstatic.com\"> <link href=\"https://fonts.googleapis.com/css2?family=Roboto+Condensed:ital,wght@0,300;0,400;0,700;1,300;1,400;1,700&display=swap\" rel=\"stylesheet\"> <title>Carlos Rodriguez D</title> </head> <body> <style> html { box-sizing: border-box; font-size: 16px; } *, *:before, *:after { box-sizing: inherit; } *{ margin: 0; padding: 0; box-sizing: border-box; font-family: 'Roboto Condensed', sans-serif; } :root { --dark: #384259; --red: #F73859; --green: #7AC7C4; --green-light: #C4EDDE; } #navbar{ display: flex; flex-direction: row; justify-content: flex-end; position: fixed; top: 0; left: 0; width: 100%; background-color: var(--red); z-index:999; } .navbar-list{ display: flex; margin-right: 3rem; list-style-type: none; } .navbar-list a{ display: block; padding: 2rem; font-size: 1.5em; text-decoration: none; color: white; font-weight: 500; } .navbar-list a:hover{ background-color: var(--green); } #welcome-section{ display: flex; flex-direction: column; justify-content: center; align-items: center; width: 100%; height: 100vh; background-image: linear-gradient(45deg, rgba(75,88,117,.95) 21%, rgba(56,66,89,.95) 100%), url(\"https://images.pexels.com/photos/546819/pexels-photo-546819.jpeg?auto=compress&cs=tinysrgb&dpr=2&h=650&w=940\"); background-size:cover; } h1{ font-size: 4.5em; font-weight: 700; color: var(--green-light); } #welcome-section > p{ color: var(--red); margin-top: 5px; font-size: 1.5em; font-weight: 400; letter-spacing: 2px; font-style: italic; } #projects { background-color: var(--green); text-align: center; padding: 50px 2rem; width: 100%; } h2{ color: white; font-size: 2.8em; border-bottom: 2px white solid; max-width: 600px; margin: auto; margin-bottom: 80px; } .project-img { height: calc(100% - 4rem); width: 100%; object-fit:fill; } .project-tile { background: var(--dark); overflow: hidden; border-radius: 10px; text-decoration: none; box-shadow:  12px 12px 24px #70b7b4, -12px -12px 24px #84d7d4; transition:  transform .2s; } .project-tile:hover{ transform: scale(1.05); } .projects-container{ display: grid; grid-template-columns: repeat(2, minmax(320px,450px)); justify-content: center; grid-gap: 5rem; width: 100%; margin-bottom: 80px; } .project-tittle { display: block; color: white; font-size: 1.2em; letter-spacing: 2px; font-weight: 400; padding: 15px; } #profile-link{ margin-top: 50px; text-decoration: none; color: white; padding: 20px 40px; font-size: 1.2em; background: var(--dark); transition: background-color .3s; box-shadow:  12px 12px 24px #70b7b4, -12px -12px 24px #84d7d4; } #profile-link:hover{ background-color: var(--red); } i { color:white; margin-left: 5px; transition:  transform .3s ease-out; } #profile-link:hover > i{ transform: translate(15px, 0%); } #contact { background-color: var(--dark); display: flex; width: 100%; height: 70vh; padding: 30px; flex-direction: column; justify-content: center; align-items: center; text-align: center; color: white; } h3{ font-size: 3em; font-weight: 800; margin-bottom: 50px; } .contact-medias{ display: flex; flex-direction: row; width: 100%; justify-content: center; align-items: center; align-content: center; flex-wrap: wrap; } .contact-medias-link{ letter-spacing: 2px; font-size: 1.6em; padding: 10px 30px; text-decoration:none; color: white; } .contact-medias-link:hover{ transform: scale(1.1); } @media (max-width: 750px) { html { font-size: 12px; } .navbar-list { margin: auto; } .projects-container{ grid-template-columns: minmax(320px,450px); grid-template-rows: auto-fill; } } </style> <nav id=\"navbar\"> <ul class=\"navbar-list\"> <li><a href=\"#welcome-section\">Welcome</a></li> <li><a href=\"#projects\">Projects</a></li> <li><a href=\"#contact\">Contact</a></li> </ul> </nav> <section id=\"welcome-section\"> <h1>Carlos Rodriguez D</h1> <p>Frontend Web Developer</p> </section> <section id=\"projects\"> <h2>Here are some of my projects</h2> <div class=\"projects-container\"> <a target=\"_BLANK\" href=\"https://codepen.io/transductancia/pen/mdWGoov\" class=\"project-tile\"> <img class=\"project-img\" src=\"https://github.com/Transconductancia/FreeCodeCamp/blob/main/ResponsiveWeb/PersonalPortfolio/Tribute.png?raw=true\" alt=\"Project 1: Tibute page\"> <p class=\"project-tittle\">Tribute page</p> </a> <a target=\"_BLANK\" href=\"https://codepen.io/transductancia/pen/MWpPvom\" class=\"project-tile\"> <img class=\"project-img\" src=\"https://github.com/Transconductancia/FreeCodeCamp/blob/main/ResponsiveWeb/PersonalPortfolio/Survey.png?raw=true\" alt=\"Project 2: Survey form\"> <p class=\"project-tittle\">Survey form</p> </a> <a  target=\"_BLANK\" href=\"https://codepen.io/transductancia/pen/wvJNyea\" class=\"project-tile\"> <img class=\"project-img\" src=\"https://github.com/Transconductancia/FreeCodeCamp/blob/main/ResponsiveWeb/PersonalPortfolio/Product.png?raw=true\" alt=\"Project 3: Product Page\"> <p class=\"project-tittle\">Product page</p> </a> <a target=\"_BLANK\" href=\"https://codepen.io/transductancia/pen/eYvXZjG\" class=\"project-tile\"> <img class=\"project-img\" src=\"https://github.com/Transconductancia/FreeCodeCamp/blob/main/ResponsiveWeb/PersonalPortfolio/Documentation.png?raw=true\" alt=\"Project 4: Documentation page\"> <p class=\"project-tittle\">Documentation page</p> </a> </div> <a  target=\"_blank\" href=\"https://codepen.io/transductancia\" id=\"profile-link\"> More info <i class=\"fas fa-angle-double-right\"></i> </a> </section> <section id=\"contact\"> <div> <h3>Are you interested in my work?...</h3> </div> <div class=\"contact-medias\"> <a class=\"contact-medias-link\" href=\"https://www.linkedin.com/in/carlos-rodriguez-delghams-0b4297149/\" target=\"_blank\" rel=\"noopener noreferrer\"> <i class=\"fab fa-linkedin\"></i> LinkedIn </a> <a class=\"contact-medias-link\" href=\"https://github.com/Transconductancia\" target=\"_blank\" rel=\"noopener noreferrer\"> <i class=\"fab fa-github-square\"></i> GitHub </a> <a class=\"contact-medias-link\" href=\"https://www.instagram.com/carlos.rodriguezdel/\" target=\"_blank\" rel=\"noopener noreferrer\"> <i class=\"fab fa-instagram-square\"></i> Instagram </a> <a class=\"contact-medias-link\" href=\"mailto: carlos.rodridelghams@gmail.com\" target=\"_blank\" rel=\"noopener noreferrer\">               <i class=\"fas fa-envelope-square\"></i> Send me an Email </a> </div> </section> </body> </html>";
// Wifi event handler
static esp_err_t event_handler(void *ctx, system_event_t *event)
{
	printf("Eventos de Wifi %d\r\n", event->event_id);
    switch(event->event_id){

       case SYSTEM_EVENT_AP_START:
           printf("AP inicializado OK");
           break;

	   case SYSTEM_EVENT_AP_STACONNECTED:
          xSemaphoreGive(client_connected);
          break;
    
	   default:
          break;
    }

	return ESP_OK;
}


// Main task
void main_task()
{
	int sock, sock_client, c;
	struct sockaddr_in server, client;
	unsigned char buffer[50] = {0};

	while(1){
		xSemaphoreTake(client_connected, portMAX_DELAY);
		printf("Cliente Conectado\r\n");
		while(1){
			sock = socket(AF_INET, SOCK_STREAM, 0);

			server.sin_family = AF_INET;
			server.sin_addr.s_addr = INADDR_ANY;
			server.sin_port = htons(80);

			if(bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0){
				printf("Error de binding\r\n");
			}

			printf("Enlace Ok\r\n");

			listen(sock, 3);

			while(1){
				printf("Esperando Conexiones\r\n");
				c = sizeof(struct sockaddr_in);
				sock_client = accept(sock, (struct sockaddr *)&client, (socklen_t*)&c);
				if (sock_client < 0)
				{
					printf("conexión rechazada");
					return;
				}

				printf("Cliente Conectado\r\n");
				while(1){
					memset(buffer, 0, sizeof(buffer));
					//Recibir mensajes
					
					while (recv(sock_client , buffer , sizeof(buffer) , 0)>0)
					{
						if (strncmp("GET", &buffer, 3)==0){
							printf("enviando HTML\r\n");
							send(sock_client, html, sizeof(html), 0);
							vTaskDelay(500/portTICK_RATE_MS);
							close(sock_client);
							break;		
						}
						printf("%s", buffer);
					}
					break;
				}
				


			}
			vTaskDelay(1000/portTICK_PERIOD_MS);
		}
	}
}


// Main application
void app_main()
{
	//Deshabilitamos log eventos wifi
	esp_log_level_set("wifi", ESP_LOG_NONE);

	//Inicialización NVS (Non Volatile Storage)
	ESP_ERROR_CHECK(nvs_flash_init());

	tcpip_adapter_init();

	ESP_ERROR_CHECK(tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_AP));
	tcpip_adapter_ip_info_t info;
	memset(&info, 0, sizeof(info));
	IP4_ADDR(&info.ip, 192, 168, 10, 1);
	IP4_ADDR(&info.gw, 192, 168, 10, 1);
	IP4_ADDR(&info.netmask, 255, 255, 255, 0);
	ESP_ERROR_CHECK(tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_AP, &info));
	ESP_ERROR_CHECK(tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_AP));


	ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));

	wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));
	ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));

	wifi_auth_mode_t auth = WIFI_AUTH_OPEN;

	wifi_config_t ap_config = {
		.ap = {
			.ssid = "CarlosESP",
			.password = "1234567890",
			.ssid_len = 0,
			.channel = 10,
			.authmode = auth,
			.ssid_hidden = 0,
			.max_connection = 4,
			.beacon_interval = 100,
		},
	};

	ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_config));
	ESP_ERROR_CHECK(esp_wifi_start());

	printf("Inicializando AP\r\n");

	client_connected = xSemaphoreCreateBinary();

	//Tarea principal
    xTaskCreatePinnedToCore(main_task, "main_task", 20000, NULL, 5, NULL, 1);
    //xTaskCreatePinnedToCore(task1, "task1", 20000, NULL, 4, NULL, 1);
}
