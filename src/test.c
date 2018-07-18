#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
// #include <sys/types.h> 
// #include <sys/wait.h>
pthread_mutex_t* humidity_mutex;
pthread_mutex_t* temperature_mutex;

void getHumidity(){
	// Get value from arduino data
	// readValueFromArduino(gardenStatus);
	printf("HUMIIDITY ----\n");
	char* raw_val_arduino = "#127*";
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
		char str_temp[length_temperature];
		for (int i = 0; i < length_temperature; ++i)
		{
			str_temp[i] = raw_val_arduino[start + i + 2];
		}
		temperature = atoi(str_temp);
	}
	printf("%d\n", temperature);
}

int main(){

	// if (init(&gardenStatus) == -1){
	// 	exit(-1);
	// }
	// pthread_t thread_id;

	// pthread_create(&thread_id, NULL, (void*) *gardenServer, &gardenStatus);

	// monitorGarden(&gardenStatus);
	// close(gardenStatus.serialfd);

	// return 0;
	printf("MAIN --- \n");
	getHumidity();
	printf("END --- \n");

	return 0;
}


// 	pthread_mutex_lock(humidity_mutex);
// 	// Set humidity value
// 	pthread_mutex_unlock(humidity_mutex);
// }