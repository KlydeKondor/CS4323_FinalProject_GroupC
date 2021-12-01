all: dataServer server client
dataServer:
	gcc -o dataServer.out dataServer_main.c michael_somdecerff.c -lpthread -lrt -std=c11
server:
	gcc -o server.out server_main.c michael_somdecerff.c -lpthread -lrt -std=c11
client:
	gcc -o client.out client_main.c michael_somdecerff.c HunterHolstead.c -lpthread -lrt -std=c11

databaseTest:
	gcc -o databaseTest.out database.c michael_somdecerff.c -lpthread -lrt -std=c11

raceConditionBroken:
	gcc -o raceConditionBroken.out GROUPC_1/RaceConditions_Main.c -lpthread -lrt -std=c11

raceConditionFixed:
	gcc -o raceConditionFixed.out GROUPC_2/RaceConditions_Main.c -lpthread -lrt -std=c11

deadlockBroken:
	gcc -o deadlockBroken.out GROUPC_3/Deadlock_Main.c -lpthread -lrt -std=c11

deadlockFixed:
	gcc -o deadlockFixed.out GROUPC_4/Deadlock_Main.c -lpthread -lrt -std=c11

clean:
	rm *.out