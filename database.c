// Kyle Kentner
// 11/10/2021
// kyle.kentner@okstate.edu
// Defining functions to handle database operations

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Buffer variable
#define BUFF_SIZE 8192

// Maximum row size allowed
#define MAX_ROW 512

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

int findUpdateRow(FILE* fDB, const char* whereVal, int whereCol) {
	int foundLength = -1;
	char getVal[BUFF_SIZE]; // Buffer for the current DB row
	char* fullRow = (char*) malloc(BUFF_SIZE * sizeof(char)); // Char pointer for the current DB row
	char* dbVal = (char*) malloc(BUFF_SIZE * sizeof(char)); // Char pointer for the current DB column
	
	const char separator[2] = "|"; // Delimiter to be used in the strtok function
	int curRow = 0, curCol = 0;
	int breakCounter = 0;
	
	// Initialize getVal with the first row
	fgets(getVal, BUFF_SIZE, fDB);
	
	// Check each row
	while (getVal[0] != '\0') {
		// Store the current row in fullRow
		strcpy(fullRow, getVal);
		
		// Initialize dbVal with the first column in getVal (need strcpy?)
		strcpy(dbVal, strtok(getVal, separator));
		
		// Check each column
		while (dbVal != NULL && dbVal[0] != '\n' && dbVal[0] != '|') {
			// Check dbVal vs whereVal
			if (curCol == whereCol && strcmp(dbVal, whereVal) == 0) {
				// Skip finding the next row; this is the final row
				goto found;
			}
			else if (dbVal != NULL) {
				printf("Column %d: DB - %s ### Search - %s\n", curCol, dbVal, whereVal);
			}
			
			// Get the current leftmost column using strtok
			strcpy(dbVal, strtok(NULL, separator)); // Passing NULL so each column is removed from the buffer
			curCol++;
		}
		
		// Get the next row
		memset(getVal, 0, BUFF_SIZE); // Clear getVal
		fgets(getVal, BUFF_SIZE, fDB); // Use fgets for the next row
		curRow++; // Increment curRow
		curCol = 0; // Reset curCol
	}
	
	// If not found, return -1
	free(fullRow);
	free(dbVal);
	return foundLength;
	
found:
	foundLength = (strlen(fullRow) + 1) * sizeof(char);
	printf("Success! Row length = %d chars\n", foundLength);
	free(fullRow);
	free(dbVal);
	
	return foundLength;
}

int performUpdate(FILE* fDB, int rowLength, const char* newRow) {
	int curChar = strlen(newRow);
	
	// Use fseek to move the cursor back to the start of the current row and overwrite
	fseek(fDB, -rowLength, SEEK_CUR);
	
	// fseek moved the cursor to the correct location; can now place the new data
	fputs(newRow, fDB);
	
	// Overwrite the remainder of the line with pipe characters if the replacement data is shorter than the original
	while (curChar < MAX_ROW && curChar < rowLength - 1) {
		fputc('\0', fDB);
		curChar++;
	}
	
	// Add a newline character
	fseek(fDB, -(int) sizeof(char), SEEK_CUR);
	fputc('\n', fDB);
	
	return 0;
}

int database(int cmdType, int col, const char* tbl, const char* setVal, int whereCol, const char* whereVal) {
	int dbSuccess = 0;
	
	// File pointer for the client's table
	FILE* fDB;
	
	switch (cmdType) {
	case 1: // SELECT
		fDB = fopen(tbl, "r"); // Open in read-only mode
		
		fclose(fDB);
		break;
	case 2: // INSERT
		fDB = fopen(tbl, "a"); // Open in append mode
		fputs(setVal, fDB);
		fputc('\n', fDB); // Add the newline character separately
		fclose(fDB);
		break;
	case 3: // UPDATE
		fDB = fopen(tbl, "r+"); // Open in read-write mode (find existing record and update)
		int byteReset = findUpdateRow(fDB, whereVal, whereCol);
		
		// If the value was found, update the associated row with the new info
		if (byteReset != -1) {
			// Overwrite the old row with the new row
			performUpdate(fDB, byteReset, setVal);
			fclose(fDB);
		}
		else {
			printf("Not found\n");
			fclose(fDB);
		}
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
	
	// Insert into sellerInformation or customerInformation
	if (isCustomer) {
		// Add the user to the database as a customer (TODO: Check for conflicts?)
		registerSuccess = database(INSERT, -1, "clientInformation.txt", registerData, -1, NULL);
	}
	else {
		// Add the user to the database as a seller (TODO: Check for conflicts?)
		registerSuccess = database(INSERT, -1, "sellerInformation.txt", registerData, -1, NULL);
	}
	
	return registerSuccess;
}

int updateClient(char* registerData, int isCustomer) {
	int rowsAffected = 0;
	
	// Update sellerInformation or customerInformation
	if (isCustomer) {
		// TODO: Pass unique identifier to updateClient (currently hardcoded with CustomerID of 5)
		// Update the customer's information
		rowsAffected = database(UPDATE, -1, "clientInformation.txt", registerData, 0, "5");
	}
	else {
		// Update the seller's information
		rowsAffected = database(UPDATE, 0, "sellerInformation.txt", registerData, 0, NULL);
	}
	
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
	// TODO: Uniqueness checking
	char dummyData[] = "5|Jane Doe|555-867-5309|123 Address Road|";
	registerClient(dummyData, 1);
	
	char dummyUpdate[] = "5|tesT|Dadt|Foo|";
	updateClient(dummyUpdate, 1);
	
	return 0;
}