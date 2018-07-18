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
pthread_mutex_t* humidity_mutex;
pthread_mutex_t* temperature_mutex;
char* CONF_FILE_PATH;
int main(){
	GardenStatus gardenStatus;

	if (init(&gardenStatus) == -1){
		exit(-1);
	}
	pthread_t thread_id[2];
	pthread_create(&thread_id[0], NULL, (void*) *gardenServer, &gardenStatus);
	pthread_create(&thread_id[1], NULL, (void*) *monitorGarden, &gardenStatus);


	pthread_join(thread_id[0], NULL);
	pthread_join(thread_id[1], NULL);
	close(gardenStatus.serialfd);
	return 0;
}
int init(GardenStatus* gardenStatus){
	srand(time(NULL));
	setConfFile(gardenStatus);
	struct timeval tv;
	gettimeofday(&tv,NULL);
	unsigned long currentTime = 1000000 * tv.tv_sec + tv.tv_usec;
	humidity_mutex = &(pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	
	gardenStatus->serialfd = open("/dev/ttyACM0", O_RDONLY | O_NOCTTY | O_NDELAY);
    if (gardenStatus->serialfd == -1)
        perror("Garden >Error opening the serial port");
        return 0;

	if(readValueFromArduino(gardenStatus)){
		setHumidity(gardenStatus);
	} else{
		printf("Garden >Could not read value from arduino...");
		gardenStatus->humidity = -1;
		gardenStatus->temperature = -1;
		gardenStatus->arduinoData[0] = ' ';
	}
	
	gardenStatus->tapStatus = getTapStatus(gardenStatus);
	gardenStatus->lastTapOpen = currentTime-(gardenStatus->config_humidityWait);
    if(getTapStatus(gardenStatus)){
    	closeTap(gardenStatus);
    }
    return 0;
}
void setConfFile(GardenStatus* gardenStatus){
	FILE *confFile;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    confFile = fopen (CONF_FILE_PATH,"r");
    if (confFile == NULL){
        printf("Garden >No configuration file found : setting to default value\n");
        gardenStatus->config_humidityWait=10000000; // 10 sec
    	gardenStatus->config_openTapTime=1; // 1 sec
		gardenStatus->config_minHumidity=100;
    }
	// char* parameterValue;
	// // READ FILE LINI BY LINE
 //    if(line[0] != '#'){
 //    	if(strCompare(line, "HUMIDITY_WAIT")){
	// 		parameterValue = strtok(line," ");
 //    		gardenStatus->config_humidityWait = strToInt(parameterValue);
 //    	}
 //    	if(strCompare(line, "OPEN_TAP_TIME")){
	// 		parameterValue = strtok(line," ");
 //    		gardenStatus->config_openTapTime = strToInt(parameterValue);
 //    	}
 //    	if(strCompare(line, "MIN_HUMIDITY")){
	// 		parameterValue = strtok(line," ");
 //    		gardenStatus->config_minHumidity = strToInt(parameterValue);
 //    	}
 //    }
}
void monitorGarden(GardenStatus* gardenStatus){
	while(1){
		printf("Garden >Arduino message : %s\n", gardenStatus->arduinoData);
		monitorHumidity(gardenStatus);
		sleep(3);
	}
}
int monitorHumidity(GardenStatus* gardenStatus){
	struct timeval tv;
	gettimeofday(&tv,NULL);
	unsigned long currentTime = 1000000 * tv.tv_sec + tv.tv_usec;
	if(readValueFromArduino(gardenStatus)){
		setHumidity(gardenStatus);
		printf("Garden >ArduinoData has been updated...");
	}else{
		printf("Garden >Could not update value from arduino...");
	}
	if (gardenStatus->lastTapOpen + gardenStatus->config_humidityWait  > currentTime ){
		printf("Garden >Tap has been open just now\n");
		return 0;
	}
	if ( gardenStatus->humidity < gardenStatus->config_minHumidity ){
		increaseHumidity(gardenStatus);
		return 1;
	}
	return 0;
}
void setHumidity(GardenStatus* gardenStatus){
	// Get value from arduino data
	printf("Garden >setHumidity called\n");
	// Get value from arduino data
	int raw_length = strlen(gardenStatus->arduinoData);
	int start, end = -1;
	int length_humidity, humidity;
	for (int i = 0; i < raw_length; ++i){
		if (gardenStatus->arduinoData[i] == '#'){
			start = i;
		} else if (gardenStatus->arduinoData[i] == '*'){
			end = i;
		}
	}
	if (start != -1 && end != -1 && start < end) {
		length_humidity = end - start - 4;
		char str_temp[length_humidity];
		for (int i = 0; i < length_humidity; ++i){
			str_temp[i] = gardenStatus->arduinoData[start + i + 2];
		}
		humidity = atoi(str_temp);
	}
	pthread_mutex_lock(humidity_mutex);
	gardenStatus->humidity = humidity;
	pthread_mutex_unlock(humidity_mutex);
}
void getTemperature(GardenStatus* gardenStatus){
	// Get value from arduino data
	pthread_mutex_lock(temperature_mutex);
	// Get value from arduino data
	int raw_length = strlen(gardenStatus->arduinoData);
	int start, end = -1;
	int length_temperature, temperature;
	for (int i = 0; i < raw_length; ++i){
		if (gardenStatus->arduinoData[i] == '#'){
			start = i;
		} else if (gardenStatus->arduinoData[i] == '*'){
			end = i;
		}
	}
	if (start != -1 && end != -1 && start < end) {
		int length_temperature = end - start;
		char str_temp[length_temperature];
		for (int i = 0; i < length_temperature; ++i){
			str_temp[i] = gardenStatus->arduinoData[start + i + 1];
		}
		temperature = atoi(str_temp);
	}
	gardenStatus->temperature = temperature;

	// Set humidity value
	pthread_mutex_unlock(temperature_mutex);
}
int readValueFromArduino(GardenStatus* gardenStatus){
    fcntl(gardenStatus->serialfd, F_SETFL, 0);
    read(gardenStatus->serialfd, gardenStatus->arduinoData, 512);
	printf("Garden >Arduino value updated\n");
	return 1;	
}
void increaseHumidity(GardenStatus* gardenStatus){
	openTap(gardenStatus);
	sleep(gardenStatus->config_openTapTime);
	pthread_mutex_lock(humidity_mutex);
	gardenStatus->humidity+=10; // SIMULATION
	pthread_mutex_unlock(humidity_mutex);
	closeTap(gardenStatus);
}
void openTap(GardenStatus* gardenStatus){
	// arduino openTap
	printf("Garden >The tap is opening\n");
	gardenStatus->tapStatus = 1;
	struct timeval tv;
	gettimeofday(&tv,NULL);
	gardenStatus->lastTapOpen=1000000 * tv.tv_sec + tv.tv_usec;
}
void closeTap(GardenStatus* gardenStatus){
	// arduino closeTap
	gardenStatus->tapStatus = 0;
	printf("Garden >The tap is closing\n");
}
short getTapStatus(GardenStatus* gardenStatus){
	// arduino tapStatus
	return gardenStatus->tapStatus;
}
