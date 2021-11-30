// Kyle Kentner
// 11/24/2021
// kyle.kentner@okstate.edu
// Defining functions to handle database operations

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "database.h"

int main() {
	// First four operations in PDF
	// TODO: Uniqueness checking
	// Insert a dummy row
	printf("\nINSERT TEST...\n");
	char dummyData[] = "Jane Doe|555-867-5309|123 Address Road|";
	registerClient(dummyData, 1);
	
	// Update the dummy row (comment out to verify that the row  is being added)
	printf("\nUPDATE TEST...\n");
	char dummyUpdate[] = "5|tesT|Dadt|Foo|";
	updateClient(dummyUpdate, 1);
	
	// Select the dummy row
	printf("\nSELECT TEST...\n");
	selectClient(1, "5");
	
	// Delete the dummy row (comment out to verify that the row is being added/updated)
	printf("\nDELETE TEST...\n");
	removeClient(1);
	
	// Last five operations in PDF
	printf("\nVIEW PRODUCTS SELLER...\n");
	viewProductsSeller("4");
	
	printf("\nVIEW ORDERS SELLER...\n");
	viewOrdersSeller("3");
	
	printf("\nVIEW ORDERS BUYER...\n");
	viewOrdersSeller("2");
	
	printf("\nVIEW PRODUCTS BUYER...\n");
	viewProductsBuyer("3");
	
	printf("\nVIEW BILLING INFO...\n");
	viewBillingInfo("2");
	
	printf("\nADD PRODUCT...\n");
	addProduct("The Product!", "4", 10, 1.54f);
	
	printf("\nDROP PRODUCT...\n");
	removeProduct("6");
	
	printf("\nUPDATE QUANTITY...\n");
	updateQuantity("8", 5);
	
	return 0;
}
