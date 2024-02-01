#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>


typedef struct{
    int IP;
    int PORT;
    int socketdesc;
    struct sockaddr_in server_address;
}Client;

int clientConnect(Client *c, int port){
    c->IP = AF_INET;
    c->PORT = port;


    c->socketdesc = socket(c->IP, SOCK_STREAM, 0);
    
    if(c->socketdesc == -1){
        perror("Error");
        return -1;
    }

    char *saddress = "127.0.0.1";
    c->server_address.sin_family = AF_INET;
    c->server_address.sin_port = htons(port);
    c->server_address.sin_addr.s_addr = inet_addr(saddress);
    
    if(connect(c->socketdesc, (struct sockaddr *)&c->server_address, sizeof(c->server_address)) == -1){
        perror("Error");
        return -1;
    }


    return 1;

}   



int main(){

    Client client;
    clientConnect(&client, 2000);

    char messagebuffer[255];

    printf("Enter message: ");
    gets(messagebuffer);

    if(send(client.socketdesc, messagebuffer, 255, 0) < 0){
        perror("Error");
        return -1;
    }



    return 1;
}