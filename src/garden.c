/*
    Alexandre GARCIA - Antoine LEVY - Clémentine Thornary
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
#include <fcntl.h>
#include <unistd.h>
// #include <sys/types.h> 
// #include <sys/wait.h>
pthread_mutex_t* humidity_mutex;
pthread_mutex_t* temperature_mutex;

int main(){
	GardenStatus gardenStatus;

	if (init(&gardenStatus) == -1){
		exit(-1);
	}
	pthread_t thread_id;

	pthread_create(&thread_id, NULL, (void*) *gardenServer, &gardenStatus);

	monitorGarden(&gardenStatus);
	close(gardenStatus.serialfd);

	return 0;
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
	
	gardenStatus->serialfd = open("/dev/ttyACM0", O_RDONLY | O_NOCTTY | O_NDELAY);
    if (gardenStatus->serialfd == -1)
        perror("Error opening the serial port");
        return 0;

	if(readValueFromArduino(gardenStatus)){
		getHumidity(gardenStatus);
	} else{
		printf("could not read value from arduino...");
		gardenStatus->humidity = -1;
		gardenStatus->arduinoData[0] = "";
	}
	
	gardenStatus->tapStatus = getTapStatus(gardenStatus);
	gardenStatus->last_tap_open = currentTime-(gardenStatus->config_humidityWait);
    if(getTapStatus(gardenStatus)){
    	closeTap(gardenStatus);
    }
    return 0;
}
void isParameter(char* parameterName){
	/*char* parameters;
	strtok(parameters," ");
	if (parameters == parameterName){		
	}*/
}
void monitorGarden(GardenStatus* gardenStatus){
	while(1){
		printf("Arduino message : %s\n", gardenStatus->arduinoData);
		monitorHumidity(gardenStatus);
		sleep(3);
	}
}
int monitorHumidity(GardenStatus* gardenStatus){
	struct timeval tv;
	gettimeofday(&tv,NULL);
	unsigned long currentTime = 1000000 * tv.tv_sec + tv.tv_usec;

	if(readValueFromArduino(gardenStatus)){
		getHumidity(gardenStatus);
	}else{
		printf("could not update value from arduino...");
	}

	if (gardenStatus->last_tap_open + gardenStatus->config_humidityWait  > currentTime ){
		printf("Tap has been open just now\n");
		return 0;
	}
	if ( gardenStatus->humidity < gardenStatus->config_minHumidity ){
		increaseHumidity(gardenStatus);
		return 1;
	}
	return 0;
}
void getHumidity(GardenStatus* gardenStatus){
	// Get value from arduino data
	char* raw_val_arduino = readValueFromArduino();
	int raw_length = strlen(raw_val_arduino);
	int start, end = -1;
	int length_temperature, temperature;
	for (int i = 0; i < raw_length; ++i)
	{
		if (raw_val_arduino[i] == '#')
		{
			start = i;
		} else if (raw_val_arduino[i] == '*')
		{
			end = i;
		}
	}
	if (start != -1 && end != -1 && start < end) {
		int length_temperature = end - start - 4;
		char* str_temp[length_temperature];
		for (int i = 0; i < length_temperature; ++i)
		{
			str_temp[i] = raw_val_arduino[start + i + 2];
		}
		temperature = atoi(str_temp);
	}
	pthread_mutex_lock(humidity_mutex);
	// Set humidity value
	pthread_mutex_unlock(humidity_mutex);
}
int readValueFromArduino(GardenStatus* gardenStatus){
    fcntl(gardenStatus->serialfd, F_SETFL, 0);
    read(gardenStatus->serialfd, gardenStatus->arduinoData, 512);
	return 1;	
}
void getTemperature(GardenStatus* gardenStatus){
	pthread_mutex_lock(temperature_mutex);
	printf("Arduino returns humidity of %f\n",gardenStatus->humidity);
	pthread_mutex_unlock(temperature_mutex);
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
