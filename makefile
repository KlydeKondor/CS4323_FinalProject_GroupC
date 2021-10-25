all: dataServer server client
dataServer:
	gcc -o dataServer.out dataServer_main.c -pthread -std=c11
server:
	gcc -o server.out server_main.c -pthread -std=c11
client:
	gcc -o client.out client_main.c -pthread -std=c11