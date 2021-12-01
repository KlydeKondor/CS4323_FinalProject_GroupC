// Kyle Kentner
// 11/24/2021
// kyle.kentner@okstate.edu
// Defining functions to handle database operations

#include <stdio.h>
#include "database.h"

int main() {
	// First four operations in PDF
	printf("VIEW ALL...\n");
	char* allProducts = viewAllProducts();
	printf("%s\n", allProducts);
	free(allProducts);
	
	printf("INSERT TEST...\n");
	char dummyData[] = "5|Jane Doe|555-867-5309|123 Address Road|";
	registerClient(dummyData, 1);
	
	// Update the dummy row (comment out to verify that the row  is being added)
	printf("UPDATE TEST...\n");
	char dummyUpdate[] = "5|tesT|Dadt|Foo|";
	updateClient(dummyUpdate, 1);
	
	// Select the dummy rows
	printf("SELECT TEST...\n");
	//selectClient(1);
	
	// Delete the dummy row (comment out to verify that the row is being added/updated)
	printf("DELETE TEST...\n");
	removeClient(1);
	
	// Last five operations in PDF
	printf("VIEW PRODUCTS SELLER...\n");
	printf("%s\n", viewProductsSeller("4"));
	
	printf("VIEW ORDERS SELLER...\n");
    printf("%s\n", viewOrdersSeller("3"));
	
	printf("VIEW ORDERS BUYER...\n");
    printf("%s\n", viewOrdersSeller("2"));
	
	printf("VIEW PRODUCTS BUYER...\n");
    printf("%s\n", viewProductsBuyer("3"));
	
	printf("VIEW BILLING INFO...\n");
    //printf("%s\n", viewBillingInfo("2"));
	
	printf("\nADD PRODUCT...\n");
	addProduct("The Product!|4|10|1.54");
	
	printf("\nDROP PRODUCT...\n");
	removeProduct("6");
	
	printf("\nUPDATE QUANTITY...\n");
	updateQuantity("8|5|");
	
	printf("\nUPDATE PRICE...\n");
	updatePrice("8|40.00|");
	
	return 0;
}
