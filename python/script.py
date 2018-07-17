import serial
from filelock import FileLock

FILENAME="../data/sensor_data"

ser=serial.Serial("/dev/ttyACM0",9600)  #change ACM number as found from ls /dev/tty/ACM*
while(1):
    
    line = ser.readline()
    if(line != None):
        print(">" +line + "<" )
	with FileLock(FILENAME):
        	data_file = open("sensor_data", "w")
        	data_file.write(line)
        	data_file.close()


