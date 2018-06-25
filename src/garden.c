/*
    Antoine LEVY - Clémentine Thornary
	garden-project - garden.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include "garden.h"
#include "gardenServer.h"
// #include <sys/types.h> 
// #include <sys/wait.h>
pthread_mutex_t* humidity_mutex;

void main(){
	GardenStatus gardenStatus;

	if (init(&gardenStatus) == -1){
		exit(-1);
	}
	pthread_t thread_id;

	pthread_create(&thread_id, NULL, (void*) *monitorGarden, &gardenStatus);

	gardenServer(&gardenStatus);
}
int init(GardenStatus* gardenStatus){
	srand(time(NULL));   // should only be called once

    // Default settings
	gardenStatus->config_humidityWait=10000000; // 10 sec
	gardenStatus->config_openTapTime=1; // 1 sec
	gardenStatus->config_minHumidity=100;
	// Insert a possibility to pass a configuration file
    // FILE *confFile;
    // char * line = NULL;
    // size_t len = 0;
    // ssize_t read;
    // confFile = fopen (CONF_FILE_PATH,"r");
    // if (confFile == NULL){
    //     return -1;
    // }
	struct timeval tv;
	gettimeofday(&tv,NULL);
	unsigned long currentTime = 1000000 * tv.tv_sec + tv.tv_usec;


	humidity_mutex = &(pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	gardenStatus->humidity = 10;
	getHumidity(gardenStatus);
	gardenStatus->tapStatus = getTapStatus(gardenStatus);
	gardenStatus->last_tap_open = currentTime-(gardenStatus->config_humidityWait);
    if(getTapStatus){
    	closeTap(gardenStatus);
    }
    return 0;
}
void isParameter(char* parameterName){
	char* parameters;
	strtok(parameters," ");
	if (parameters == parameterName){		
	}
}
void monitorGarden(GardenStatus* gardenStatus){
	while(1){
		monitorHumidity(gardenStatus);
		sleep(3);
	}
}
int monitorHumidity(GardenStatus* gardenStatus){
	struct timeval tv;
	gettimeofday(&tv,NULL);
	unsigned long currentTime = 1000000 * tv.tv_sec + tv.tv_usec;

	printf("currentTime is :%lu\n",currentTime );
	printf("lto + humidity :%lu\n",gardenStatus->last_tap_open + gardenStatus->config_humidityWait );
 	printf("diff=%lu \n",currentTime-(gardenStatus->last_tap_open + gardenStatus->config_humidityWait) );
 
	getHumidity(gardenStatus);

	if (gardenStatus->last_tap_open + gardenStatus->config_humidityWait  > currentTime ){
		printf("Tap has been open just now\n");
		return 0;
	}
	if ( gardenStatus->humidity < gardenStatus->config_minHumidity ){
		increaseHumidity(gardenStatus);
		return 1;
	}
}
void getHumidity(GardenStatus* gardenStatus){
	pthread_mutex_lock(humidity_mutex);
	printf("Arduino returns humidity of %f\n",gardenStatus->humidity);
	// Wait for arduino value -> we decrease the humidity
	gardenStatus->humidity=gardenStatus->humidity - rand()%4;
	pthread_mutex_unlock(humidity_mutex);
}
void increaseHumidity(GardenStatus* gardenStatus){
	openTap(gardenStatus);
	sleep(gardenStatus->config_openTapTime);
	pthread_mutex_lock(humidity_mutex);
	gardenStatus->humidity+=10; // 
	pthread_mutex_unlock(humidity_mutex);
	closeTap(gardenStatus);
}
void openTap(GardenStatus* gardenStatus){
	// arduino openTap
	printf("The tap is opening\n");
	gardenStatus->tapStatus = 1;
	struct timeval tv;
	gettimeofday(&tv,NULL);
	gardenStatus->last_tap_open=1000000 * tv.tv_sec + tv.tv_usec;
}
void closeTap(GardenStatus* gardenStatus){
	// arduino closeTap
	gardenStatus->tapStatus = 0;
	printf("The tap is closing\n");
}
short getTapStatus(GardenStatus* gardenStatus){
	// arduino tapStatus
	return gardenStatus->tapStatus;
}