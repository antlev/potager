/*
    Alexandre GARCIA - Antoine LEVY - Clémentine Thornary
	garden-project - garden.h
*/
#ifndef GARDEN_H
#define GARDEN_H
typedef struct GardenStatus {
	int config_humidityWait; // Minimum time between each tap opening in us
	int config_openTapTime; // Time while the tap is open in sec
	int config_minHumidity;
	char* arduinoData[512];
	int serialfd;
	float humidity;
	short tapStatus;
	unsigned long last_tap_open; // timestamp of the last time the tap was open
	pthread_mutex_t* humidity_mutex;
} GardenStatus;
int init(GardenStatus* gardenStatus);
void updateParameter(char* parameter, size_t len);	
void isParameter(char* parameterName);
void monitorGarden();
int monitorHumidity();
void increaseHumidity(GardenStatus* gardenStatus);
void openTap(GardenStatus* gardenStatus);
void closeTap(GardenStatus* gardenStatus);
short getTapStatus(GardenStatus* gardenStatus);
void getHumidity(GardenStatus* gardenStatus);
int readValueFromArduino(GardenStatus* gardenStatus);
#endif // GARDEN_H
