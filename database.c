// Kyle Kentner
// 11/3/2021
// kyle.kentner@okstate.edu
// Defining functions to handle database operations

#include <stdio.h>
#include <string.h>

// DB-operation constants
#define SELECT 1
#define INSERT 2
#define UPDATE 3
#define DELETE 4

// Column constants
#define PRODUCT_ID 0 // Quantity available (productInformation)

#define QUANTITY_A 3 // Quantity available (productInformation)
#define QUANTITY_P 2 // Quantity purchased (customerOrder)

int database(int cmdType, int col, const char* tbl, char* setVal, int whereCol, char* whereVal) {
	int rowsAffected = 0;
	
	// File pointer for the client's table
	FILE* fDB;
	
	switch (cmdType) {
	case 1: // SELECT
		fDB = fopen(tbl, "r");
		fDB.close();
		break;
	case 2: // INSERT
		fDB = fopen(tbl, "a");
		fDB.close();
		break;
	case 3: // UPDATE
		fDB = fopen(tbl, "r+");
		fDB.close();
		break;
	case 4: // DELETE
		fDB = fopen(tbl, "r+");
		fDB.close();
		break;
	default:
		rowsAffected = -1;
	}
	
	return rowsAffected;
}

int registerClient() {
	int registerSuccess = 0;
	
	// Insert into sellerInformation or customerInformation
	
	return registerSuccess;
}

int updateInfo() {
	int rowsAffected = 0;
	
	// Update sellerInformation or customerInformation
	
	return rowsAffected;
}

int addRemoveProduct() {
	int addRemoveSuccess = 0;
	
	// Insert or delete from productInformation
	
	return addRemoveSuccess;
}

int updateQuantityPrice() {
	int updateSuccess = 0;
	
	// Update the quantity or price of a product in productInformation
	
	return updateSuccess;
}

int purchaseReturnProduct(char* productID, char* customerID, char* orderID, char* change) {
	int purchaseReturnSuccess = 0;
	
	// If able to read, get the current quantity available
	// TODO: Get value from database, see if purchase can be made
	purchaseReturnSuccess = database(SELECT, QUANTITY_A, "productInformation.txt", PRODUCT_ID, productID); 
	
	// Update the quantity of a product in productInformation; reflect changes in billingInformation and customerOrder
	// TODO: Check if client has permission to access productInformation, billingInformation, and customerOrder
	purchaseReturnSuccess = database(UPDATE, QUANTITY_A, "productInformation.txt", PRODUCT_ID, productID);
	
	return purchaseReturnSuccess;
}