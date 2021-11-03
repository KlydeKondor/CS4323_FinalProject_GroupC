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

## Server
Compiles just the server
```commandline
make server
```

## Client
Compiles just the client
```commandline
make client
```

# Running
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