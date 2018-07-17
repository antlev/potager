/*
    Alexandre Garcia - Antoine LEVY - Clémentine Thornary
	garden-project - reader.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
int main(){
    int serialfd = open("/dev/ttyACM0", O_RDONLY | O_NOCTTY | O_NDELAY);
    if (serialfd == -1)
        perror("Error opening the serial port");
    else
        fcntl(serialfd, F_SETFL, 0);
    fprintf(stdout, "Device is open, attempting read \n");
    fcntl(serialfd, F_SETFL, 0);
    char buf[512] = {0};
    read(serialfd, buf, 512);
    fprintf(stdout, "Buffer: %s", buf);
    close(serialfd);
	return 0;
}
