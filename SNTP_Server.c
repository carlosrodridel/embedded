#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SNTP_T 2208988800ull //NTP time-stamp of 1 Jan 1970

typedef struct
{

    char li_vn_mode; // 8 bits. li, vn, and mode.
                     // li.   2 bits.   Leap indicator, set to 0.
                     // vn.   3 bits. Version number of the protocol, set to 3.
                     // mode. 3 bits. Client set to 3 for client.

    char stratum;   // 8 bits. Stratum reply: 0-15.
    char poll;      // 8 bits. Maximum interval between successive messages in sec 4 (16s) to 17(36h aprox).
    char precision; // 8 bits. Precision of the local clock -6 to -20 as lower the number, less precision.

    unsigned int rootDelay;      // 32 bits. Total round trip delay time.
    unsigned int rootDispersion; // 32 bits. Max error aloud from primary clock source.
    char refId[4];               // 32 bits. Reference clock identifier.

    unsigned int refTm_s; // 32 bits. Reference time-stamp seconds.
    unsigned int refTm_f; // 32 bits. Reference time-stamp fraction of a second.

    unsigned int origTm_s; // 32 bits. Originate time-stamp seconds.
    unsigned int origTm_f; // 32 bits. Originate time-stamp fraction of a second.

    unsigned int rxTm_s; // 32 bits. Received time-stamp seconds.
    unsigned int rxTm_f; // 32 bits. Received time-stamp fraction of a second.

    unsigned int txTm_s; // 32 bits most important field the client cares about. Transmit time-stamp seconds.
    unsigned int txTm_f; // 32 bits. Transmit time-stamp fraction of a second.

} sntp_packet;

int main()
{
    int socket_id;
    sntp_packet packet={ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };//packet initialized to 0

    packet.li_vn_mode=0x1b; //00011011 == li=00  vn=011   mode=011

    socket_id=socket(PF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("162.159.200.1");
    server.sin_port = htons(123);

    if (connect(socket_id, (const struct sockadd*)&server, sizeof(server)) < 0)
    {
        printf("Connect error\r\n");
        return -1;
    }
    
    printf("Connected\r\n");

    //sending the SNTP packet request to the server
    if (send(socket_id, (char*)&packet, sizeof(sntp_packet), 0)<0)
    {
        printf("Send failed");
        return -1;
    }
    printf("message sent\r\n");


    //receiving the SNTP packet back from the server

    if (recv(socket_id, (char*)&packet, sizeof(sntp_packet), 0)<0)
    {
        printf("Failed\r\n");
        return -1;
    }
    printf("Data received\r\n");



    packet.txTm_s = ntohl( packet.txTm_s ); //ntohl() converts the bits order from the network's to host's 

    time_t txTm = ( time_t ) ( packet.txTm_s - SNTP_T ); //transform NTP epoch to UNIX epoch

    printf( "Time: %s", ctime( ( const time_t* ) &txTm ) );

    return 0;
}
