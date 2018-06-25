/*
    Antoine LEVY - Clémentine Thornary
	garden-project - garden.c
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include "garden.h"
#include "gardenServer.h"
// #include <sys/types.h> 
// #include <sys/wait.h>


int HUMIDITY_WAIT=10000000; // Minimum time between each tap opening in us
int OPEN_TAP_TIME=1; // Time while the tap is open in sec
int MIN_HUMIDITY=100;
char * PARAMETERS="HUMIDITY_WAIT OPEN_TAP_TIME MIN_HUMIDITY";
// char* CONF_FILE_PATH = "/etc/garden/garden.conf"
char* CONF_FILE_PATH="garden.conf";

float humidity=50;
short tapStatus=0;
unsigned long last_tap_open; // timestamp of the last time the tap was open

void main(){

	if (init() == -1){
		exit(-1);
	}

	pthread_t thread_id;
	pthread_mutex_t humidity_mutex = PTHREAD_MUTEX_INITIALIZER;

	pthread_create(&thread_id, NULL, (void*) *monitorGarden, &humidity_mutex);

	gardenServer();
}
int init(){
    FILE *confFile;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    // confFile = fopen (CONF_FILE_PATH,"r");
    // if (confFile == NULL){
    //     return -1;
    // }
    if(isTapOpen){
    	closeTap();
    }
	struct timeval tv;
	gettimeofday(&tv,NULL);
	unsigned long currentTime = 1000000 * tv.tv_sec + tv.tv_usec;
    last_tap_open = currentTime-HUMIDITY_WAIT;
    return 0;
}
void isParameter(char* parameterName){
	char* parameters = PARAMETERS;
	strtok(parameters," ");
	if (parameters == parameterName){		
	}
}
void monitorGarden(pthread_mutex_t humidity_mutex){
	while(1){
		monitorHumidity(humidity_mutex);
		sleep(3);
	}
}
int monitorHumidity(){
	struct timeval tv;
	gettimeofday(&tv,NULL);
	unsigned long currentTime = 1000000 * tv.tv_sec + tv.tv_usec;
	printf("currentTime is :%lu\n",currentTime );
	printf("lto + humidity :%lu\n",last_tap_open + HUMIDITY_WAIT );
 	printf("diff=%lu \n",currentTime-(last_tap_open + HUMIDITY_WAIT) );
 
	getHumidity();

	if (last_tap_open + HUMIDITY_WAIT  > currentTime ){
		printf("Tap has been open just now\n");
		return 0;
	}
	if ( humidity < MIN_HUMIDITY ){
		increaseHumidity();
		return 1;
	}
}
float getHumidity(){
	printf("Arduino returns humidity of %f\n",humidity);
	return humidity;
}
void increaseHumidity(){
	openTap();
	sleep(OPEN_TAP_TIME);
	humidity+=10;
	closeTap();
}
void openTap(){
	// arduino openTap
	printf("The tap is opening\n");
	struct timeval tv;
	gettimeofday(&tv,NULL);
	last_tap_open=1000000 * tv.tv_sec + tv.tv_usec;
}
void closeTap(){
	// arduino closeTap
	printf("The tap is closing\n");
}
short isTapOpen(){
	// arduino tapStatus
	return tapStatus;
}