/*
    Antoine LEVY - Clémentine Thornary
	garden-project - garden.c
*/
#ifndef GARDEN_H
#define GARDEN_H
int init();
void updateParameter(char* parameter, size_t len);	
void isParameter(char* parameterName);
void monitorGarden();
int monitorHumidity();
void increaseHumidity();
void openTap();
void closeTap();
short isTapOpen();
void networkAgent();
float getHumidity();
#endif // GARDEN_H