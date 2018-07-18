### compilation garden 

gcc src/garden.c src/gardenServer.c src/utils/list.c src/utils/tools.c -o bin/garden -lpthread -lm

### compilation client

gcc src/client.c -o bin/client
