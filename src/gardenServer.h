/*
    Antoine LEVY - Clémentine Thornary
	garden-project - gardenServer.h
*/
#ifndef GARDEN_SERVER_H
#define GARDEN_SERVER_H
#include "utils/list.h"
#include <arpa/inet.h>

typedef struct Connection {
	int socket_fd;
	int client_sock;
	struct sockaddr_in client;
	struct sockaddr_in server;
	int nb_connections_max;
	List* connections_list;
} Connection;

/* ------------------- Prototypes ------------------- */
int prepareConnection(Connection* connection);
int acceptIncomingConnections(Connection* connection);
int gardenServer();
#endif // GARDEN_SERVER_H