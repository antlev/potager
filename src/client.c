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

int openConnection();

int main(int argc , char *argv[])
{
    if( argc > 1){

        for(int i=0;i<100; i++){
            if (fork()==0){
                printf("Hello from Child! nb %d\n",i);
                return openConnection();
            }
 
        // parent process because return value non-zero.
        else    
            printf("Hello from Parent!\n"); 
            sleep(1);
        }
    }else{  
        return openConnection();
    }
    return 0;
}


int openConnection(){
        int socketfd, read_size;
        struct sockaddr_in server;
        char message[2048] , server_message[2048];
        //Create socket
        socketfd = socket(AF_INET , SOCK_STREAM , 0);
        
        if(socketfd == -1){
            printf("Can't create socket\n");
        }
        printf("socket created : %d\n",socketfd );

        server.sin_addr.s_addr = inet_addr("127.0.0.1");
        server.sin_family = AF_INET;
        server.sin_port = htons( 1664 );
     
        //Connect to remote server
        if (connect(socketfd , (struct sockaddr *)&server , sizeof(server)) < 0)
        {
            perror("connect failed. Error");
            return 1;
        }
         
        printf("Connected\n");
         
        //keep communicating with server
        while(1)
        {
            printf("Enter message : ");
            scanf("%s" , message);
             
            //Send some data
            if( send(socketfd , message , strlen(message) , 0) < 0)
            {
                printf("Send failed\n");
                return 1;
            }
             
            //Receive a reply from the server
            if( (read_size = read(socketfd , server_message , 2048 )) < 0)
            {
                printf("read failed\n");
                return -3;
            }
             
            printf("Server reply :\n");
            for (int i = 0; i < read_size; ++i)
            {
                printf("%c", server_message[i]);
            }
            printf("\n");
        }
         
        close(socketfd);
}