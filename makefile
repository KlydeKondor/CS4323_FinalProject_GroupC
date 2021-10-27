all: dataServer server client
dataServer:
	gcc -o ataServer.out dataServer_main.c michael_somdecerff.c -pthread -std=c11
server:
	gcc -o server.out server_main.c michael_somdecerff.c -pthread -std=c11
client:
	gcc -o client.out client_main.c michael_somdecerff.c -pthread -std=c11

clean:
	rm *.out