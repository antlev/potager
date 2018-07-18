/*
    Antoine LEVY - Clémentine Thornary
    game-project - client.c
    Version 1.0 - 20/03/2018
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "client.h"

void printMenu(int humidity, int temperature);
void printConf1(int th);
void printConf2();
int selectionMenu();
int main(int argc , char *argv[]){
  /*  Connection connection;
    initConnection(&connection);

    while(1){
        gardenStatus(&connection);
        sleep(5);
    }
    close(connection.socket_fd);
    return 0;
    */
    while(1){
        printMenu(1,2);
        printConf1(60);
        selectionMenu();

     
    }
   
}
void initConnection(Connection* connection){
    int read_size;
    char server_message[2048];
    //Create socket
    connection->socket_fd = socket(AF_INET , SOCK_STREAM , 0);
    
    if(connection->socket_fd == -1){
        printf("Can't create socket\n");
        exit(-1);
    }
    printf("socket created : %d\n",connection->socket_fd );

    connection->server.sin_addr.s_addr = inet_addr("127.0.0.1");
    connection->server.sin_family = AF_INET;
    connection->server.sin_port = htons( 1664 );
 
    //Connect to remote server
    if (connect(connection->socket_fd , (struct sockaddr *)&connection->server , sizeof(connection->server)) < 0)
    {
        perror("connect failed. Error");
        exit(-1);
    }         
    printf("Connected\n");
}

void gardenStatus(Connection* connection){
    int choixMenu;

    //printf("___________________________________________________\n");
    //printf("%40s","|             Mon potager connecté !              |\n");
    char* message = "status_request";
    char server_message[2048];
    int read_size;
    if( send(connection->socket_fd , message , strlen(message) , 0) < 0){
        printf("Send failed\n");
    }
    if( (read_size = read(connection->socket_fd , server_message , 2048 )) < 0){
        printf("read failed\n");
    }
     
    //printf("| Taux d'humidité :                     ");
    for (int i = 0; i < read_size; ++i)
    {
        printf("%c", server_message[i]);
    }
    //printf(" |");
    //printf("\n");
    //printf("| Tempréature :                       ");
    /*for (int i = 0; i < read_size; ++i)
    {
        printf("%c", server_message[i]);
    }*/
    /*printf("            |");
    printf("\n");
    printf("|_________________________________________________|\n");
    printf("|             Menu de configuration               |\n");
    printf("| 1. Configuration taux d'humidité                |\n");
    printf("| 2. Configuration débit d'eau                    |\n");
    printf("|_________________________________________________|\n");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n");
    scanf("%d", &choixMenu);
    switch (choixMenu)
    {
        case 1:
            printf("Alex est trop beau\n");
            break;
        case 2:
            printf("Alex est trop intelligent\n");
            break;
    }*/
}

void printMenu(int humidity, int temperature){
    printf("___________________________________________________\n");
    printf("%40s","|             Mon potager connecté !              |\n");
    printf("| Taux d'humidité :                     %d\n", humidity );
    printf("| Tempréature :                         %d\n", temperature);
    printf("|_________________________________________________|\n");
    printf("|             Menu de configuration               |\n");
    printf("| 1. Configuration taux d'humidité                |\n");
    printf("| 2. Configuration débit d'eau                    |\n");
    printf("|_________________________________________________|\n");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n");
    
}

int selectionMenu(){
    int th;
    int choixMenu;
    fflush(stdin);
    scanf("%d", &choixMenu);

        switch (choixMenu)
        {
            case 1:
                printConf1(th);
                break;

            case 2:
                printConf2();
                break;

            default:
                printf("Mauvais choix\n");
                return 0;
        }
}

void printConf1(int th){
    printf("___________________________________________________\n");
    printf("%40s","|             Configuration humidité              |\n");
    printf("Taux d'humidité max (en pourcentage) : %d\n", th);
}

void selectionMenuConf1(){
    int choixMenuConf1;
    switch (choixMenuConf1)
    {
        case 1:


        case 2:
            printf("Exit\n");

    }
}

void printConf2(){
    printf("___________________________________________________\n");
    printf("%40s","|             Configuration débit d'eau             |\n");
    printf("Durée d'ouverture du débit d'eau (en s):  5 \n");
}
