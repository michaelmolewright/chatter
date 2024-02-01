#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

typedef struct{
    int IP;
    int PORT;
    int socketdesc;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
}Server;

int createserver(Server *s, int port){
    s->IP = AF_INET;
    s->PORT = port;
    s->socketdesc = socket(s->IP, SOCK_STREAM, 0);

    if(s->socketdesc == -1){
        perror("Error");
        return -1;
    }

    char *saddress = "127.0.0.1";
    s->server_address.sin_family = AF_INET;
    s->server_address.sin_port = htons(port);
    s->server_address.sin_addr.s_addr = inet_addr(saddress);

    int bindres = bind(s->socketdesc, (struct sockaddr *)&s->server_address, sizeof(s->server_address));
    if(bindres < 0){
        perror("Error");
        return -1;
    }

    if(listen(s->socketdesc, 10) < 0){
        perror("Error");
        return -1;
    }

    printf("Successfully created a Server!\nSocket file Descriptor: %d\nPort: %d\nServer Address: %s\n", s->socketdesc, port, saddress);
    return 0;
}

int connectwithclient(Server *s){
    int client_size = sizeof(s->client_address);
    int client_sock = accept(s->socketdesc, (struct sockaddr*)&s->client_address, &client_size);

    if(client_sock < 0){
        perror("Error");
        return -1;
    }

    return client_sock;
}


int main(void){

    Server server;
    int res = createserver(&server, 2000);
    int client_soc = connectwithclient(&server);

    char buffer[255];
    recv(client_soc, buffer, 255, 0);
    
    printf("%s", buffer);

    close(client_soc);
    close(server.socketdesc);
    return 1;
}