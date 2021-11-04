// Kyle Kentner
// 11/3/2021
// kyle.kentner@okstate.edu
// Defining functions to handle database operations

#include <stdio.h>
#include <string.h>

// Buffer variable
#define BUFF_SIZE 8192

// DB-operation constants
#define SELECT 1
#define INSERT 2
#define UPDATE 3
#define DELETE 4

// Column constants
#define PRODUCT_ID 0 // Quantity available (productInformation)

#define QUANTITY_A 3 // Quantity available (productInformation)
#define QUANTITY_P 2 // Quantity purchased (customerOrder)

struct sellerInfo {
	int sellerID;
	char* sellerName;
	char* contactNumber;
	char* contactAddress;
};

struct customerInfo {
	int customerID;
	char* customerName;
	char* contactNumber;
	char* contactAddress;
};

struct productInfo {
	int productID;
	char* productDesc;
	int sellerID;
	int quantityAvailable;
	float unitPrice;
};

struct billingInfo {
	int orderID;
	int customerID;
	char* customerAddress;
	float totalOrderPrice;
};

struct customerOrder {
	int orderID;
	int productID;
	int quantityOrdered;
	char* contactNumber;
	char* contactAddress;
};

int database(int cmdType, int col, const char* tbl, char* setVal, int whereCol, char* whereVal) {
	int dbSuccess = 0;
	
	// File pointer for the client's table
	FILE* fDB;
	
	switch (cmdType) {
	case 1: // SELECT
		fDB = fopen(tbl, "r"); // Open in read-only mode
		
		fclose(fDB);
		break;
	case 2: // INSERT
		printf("%s is the filename\n", tbl);
		fDB = fopen(tbl, "a"); // Open in append mode
		printf("%p is the file descriptor's pointer\n", fDB);
		//fwrite(setVal, sizeof(char), BUFF_SIZE, fDB);
		fputs(setVal, fDB);
		fclose(fDB);
		break;
	case 3: // UPDATE
		fDB = fopen(tbl, "r+"); // Open in read-write mode (find existing record and update)
		fclose(fDB);
		break;
	case 4: // DELETE
		fDB = fopen(tbl, "r+"); // Open in read-write mode (find existing record and delete)
		fclose(fDB);
		break;
	default:
		dbSuccess = -1;
	}
	
	return dbSuccess;
}

int registerClient(char* registerData, int isCustomer) {
	int registerSuccess = 0;
	printf("Is customer? %d\n", isCustomer);
	// Insert into sellerInformation or customerInformation
	if (isCustomer) {
		// Add the user to the database as a customer (TODO: Check for conflicts?)
		printf("Yes\n");
		registerSuccess = database(INSERT, -1, "clientInformation.txt", registerData, -1, NULL);
	}
	else {
		// Add the user to the database as a seller (TODO: Check for conflicts?)
		registerSuccess = database(INSERT, -1, "sellerInformation.txt", registerData, -1, NULL);
	}
	printf("Here\n");
	return registerSuccess;
}

int updateClient(char* registerData) {
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

int purchaseReturnProduct(char* productID, char* customerID, char* orderID, int change) {
	int purchaseReturnSuccess = 0;
	
	if (change > 0) {
		// If able to read, get the current quantity available
		// TODO: Get value from database, see if purchase can be made
		purchaseReturnSuccess = database(SELECT, QUANTITY_A, "productInformation.txt", NULL, PRODUCT_ID, productID);
	}
	
	// Update the quantity of a product in productInformation; reflect changes in billingInformation and customerOrder
	// TODO: Check if client has permission to access productInformation, billingInformation, and customerOrder
	//purchaseReturnSuccess = database(UPDATE, QUANTITY_A, "productInformation.txt", PRODUCT_ID, productID);
	
	return purchaseReturnSuccess;
}

int main() {
	char dummyData[] = "Test|testdata|Data|Text\n";
	printf("%s is the dummy data\n", dummyData);
	registerClient(dummyData, 1);
	return 0;
}