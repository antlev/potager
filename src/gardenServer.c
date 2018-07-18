/*
    Alexandre GARCIA - Antoine LEVY - Clémentine Thornary
	garden-project - gardenServer.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <assert.h>
#include <string.h>
#include "utils/list.h"
#include "gardenServer.h"
#include "garden.h"
#include "utils/tools.h"
int gardenServer(GardenStatus* gardenStatus){

	Connection server_connection;

	if ( prepareConnection(&server_connection) == 0){
		acceptIncomingConnections(&server_connection, gardenStatus);
	}
    
	return 0;

}

int prepareConnection(Connection* connection){
	
	connection->socket_fd = socket(AF_INET , SOCK_STREAM , 0);
	connection->nb_connections_max = 20;

	if(connection->socket_fd == -1){
		printf("GardenServer >Can't create socket\n");
	}
	printf("GardenServer >socket created : %d\n",connection->socket_fd );

	connection->server.sin_family = AF_INET;
	connection->server.sin_addr.s_addr = INADDR_ANY;
    connection->server.sin_port = htons( 1664 );

    if( bind(connection->socket_fd,(struct sockaddr *)&connection->server , sizeof(connection->server)) < 0){
        perror("bind failed");
        return 1;
    }
    printf("GardenServer >bind done\n");

    listen(connection->socket_fd , connection->nb_connections_max);

    return 0;
}

int acceptIncomingConnections(Connection* connection, GardenStatus* gardenStatus){
   	int i, c, read_size, maxfd=connection->nb_connections_max, accept_return, selectReturn;
	char client_message[2048];
	char response[2048];

	connection->connections_list = malloc(sizeof(List));
	list_init(connection->connections_list);

	fd_set rdfs;

    c = sizeof(struct sockaddr_in);
    
    while(1){
		FD_ZERO(&rdfs);
		FD_SET(connection->socket_fd, &rdfs);	
	    for(i=0;i<list_size(connection->connections_list);i++){
			FD_SET(list_get(connection->connections_list, i), &rdfs);
		}
	    printf("GardenServer >Waiting for incoming connections... or messages....\n");
	    selectReturn = select(maxfd, &rdfs, NULL, NULL, NULL);
    	if (selectReturn > 0){
		    if( FD_ISSET(connection->socket_fd, &rdfs)){
	   		    if ((accept_return = accept(connection->socket_fd, (struct sockaddr *) &(connection->client), (socklen_t*) &c)) >= 0){
				    list_append(connection->connections_list, accept_return);
			   		printf("GardenServer >Connection accepted\n" );
		    	}else{
		        	printf("GardenServer >Accept failed");
		    	}	
			}
		    for(i=0;i<list_size(connection->connections_list);i++){
		    	if(FD_ISSET(list_get(connection->connections_list, i), &rdfs)){
				    if( (read_size = read(list_get(connection->connections_list, i) , client_message , 2048)) > 0 )
				    {
				    	printf("GardenServer >Received message : %s from connection number %d connection_fd = %d\n",client_message, i+1, list_get(connection->connections_list, i) );
				        ftoa(gardenStatus->humidity, response, 4);
				        printf("GardenServer >Send back >%s<\n", response);
				        write(list_get(connection->connections_list, i) , response , read_size);
    				}else{
    					printf("GardenServer >Connection (connection_fd=%d) closed by remote user\n", list_get(connection->connections_list, i));
    					list_delete(connection->connections_list, list_get(connection->connections_list, i));
    				}
		    	}
		    }
	    }
		sleep(1);
    }
    return 0;
}
