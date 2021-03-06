/*
    Antoine LEVY - Clémentine Thornary
    game-project - client.c
    Version 1.0 - 20/03/2018
*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "client.h"

int main(int argc , char *argv[]){
    Connection connection;
    initConnection(&connection);

    while(1){
        gardenStatus(&connection);
        sleep(5);
    }
    close(connection.socket_fd);
    return 0;
}
void initConnection(Connection* connection){
    int read_size;
    char server_message[2048];
    //Create socket
    connection->socket_fd = socket(AF_INET , SOCK_STREAM , 0);
    
    if(connection->socket_fd == -1){
        printf("Can't create socket\n");
    }
    printf("socket created : %d\n",connection->socket_fd );

    connection->server.sin_addr.s_addr = inet_addr("127.0.0.1");
    connection->server.sin_family = AF_INET;
    connection->server.sin_port = htons( 1664 );
 
    //Connect to remote server
    if (connect(connection->socket_fd , (struct sockaddr *)&connection->server , sizeof(connection->server)) < 0)
    {
        perror("connect failed. Error");
    }         
    printf("Connected\n");
}
void gardenStatus(Connection* connection){
    char* message = "status_request";
    char server_message[2048];
    int read_size;
    if( send(connection->socket_fd , message , strlen(message) , 0) < 0){
        printf("Send failed\n");
    }
    if( (read_size = read(connection->socket_fd , server_message , 2048 )) < 0){
        printf("read failed\n");
    }
     
    printf("Server reply : humidity status is \n");
    for (int i = 0; i < read_size; ++i)
    {
        printf("%c", server_message[i]);
    }
    printf("\n");
}