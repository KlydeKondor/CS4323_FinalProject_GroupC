// Kyle Kentner
// 11/10/2021
// kyle.kentner@okstate.edu
// Defining functions to handle database operations

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "database.h"

int main() {
	// TODO: Uniqueness checking
	// Insert a dummy row
	char dummyData[] = "5|Jane Doe|555-867-5309|123 Address Road|";
	registerClient(dummyData, 1);
	
	// Update the dummy row (comment out to verify that the row  is being added)
	char dummyUpdate[] = "5|tesT|Dadt|Foo|";
	updateClient(dummyUpdate, 1);
	
	// Select the dummy row
	selectClient(1);
	
	// Delete the dummy row (comment out to verify that the row is being added/updated)
	removeClient(1);
	
	return 0;
}