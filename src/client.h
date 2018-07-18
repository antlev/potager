/*
    Alexandre GARCIA - Antoine LEVY - Clémentine Thornary
    garden-project - client.h
*/
#ifndef CLIENT_H
#define CLIENT_H
typedef struct Connection {
	int socket_fd;
	struct sockaddr_in server;
} Connection;
int main(int argc , char *argv[]);
void gardenStatus(Connection* connection);
void initConnection(Connection* connection);
#endif // CLIENT_H