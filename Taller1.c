#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h> //confirmar

//SOCK_STREAM -> TCP
//SOCK_DGRAM -> UDP

int main(){
    
    int conection = 0;
    int socket_id = 0;
    char msg[] = "GET / HTTP/1.1\r\n\r\n";
    char rep[5000];
    socket_id = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("142.250.78.142");
    server.sin_port = htons(80);

    conection = connect(socket_id, (struct sockaddr*)&server, sizeof(server));
    if(conection < 0){
        printf("COnnect errorr\n");
        return -1;
    }

    printf("connected!\r\n");

    //sending the request to the server
    if (send(socket_id, msg, sizeof(msg), 0)<0)
    {
        printf("Send failed");
        return -1;
    }
    printf("message sent\r\n");

    //receiving data from the server
    if (recv(socket_id, rep, 5000, 0)<0)
    {
        printf("Failed");
        return -1;
    }
    printf("Data received");
    puts(rep);
    close(socket_id);
    
          
    return(0);   
}