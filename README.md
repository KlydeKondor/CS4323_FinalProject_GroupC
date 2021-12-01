# CS4323_FinalProject_GroupC
Final project for CS-4323: Operating Systems

# Compiling
To compile different parts of the project, use the following make commands
## All
Compiles the entire project
```cmd
make
```

## Data Server
Compiles just the data server
```commandline
make dataServer
```
OR
```commandline
gcc -o dataServer.out dataServer_main.c michael_somdecerff.c -lpthread -lrt -std=c11
```

## Server
Compiles just the server
```commandline
make server
```
OR
```commandline
gcc -o server.out server_main.c michael_somdecerff.c -lpthread -lrt -std=c11
```

## Client
Compiles just the client
```commandline
make client
```
OR
```commandline
gcc -o client.out client_main.c michael_somdecerff.c HunterHolstead.c -lpthread -lrt -std=c11
```

# Running
In order to run correctly, the data server MUST be launched first, the servers MUST be launched second, and the clients MUST be launched third. 

## Data Server
```commandline
./dataServer.out
```

## Server
Which port is the server will open on
```commandline
./server.out <port>
```

## Client
Which port the client will connect to. Must match an open server port
```commandline
./client.out <port>
```