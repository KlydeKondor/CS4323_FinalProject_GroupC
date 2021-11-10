// Kyle Kentner
// 11/3/2021
// kyle.kentner@okstate.edu
// Defining functions to handle database operations

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

int findUpdateRow(FILE* fDB, const char* whereVal, int whereCol) {
	printf("whereVal in findUpdateRow: %s\n", whereVal);
	
	char getVal[BUFF_SIZE]; // Buffer for the current DB row
	char* fullRow = (char*) malloc(BUFF_SIZE * sizeof(char));
	char* dbVal = (char*) malloc(BUFF_SIZE * sizeof(char)); // Char pointer for the current DB column
	const char separator[2] = "|"; // Delimiter to be used in the strtok function
	int curRow = 0, curCol = 0;
	int breakCounter = 0;
	
	// Initialize getVal with the first row
	fgets(getVal, BUFF_SIZE, fDB);
	printf("%s\n", getVal);
	
	// Check each row
	while (getVal[0] != '\0') {
		// Store the current row in fullRow
		strcpy(fullRow, getVal);
		
		// Initialize dbVal with the first column in getVal
		dbVal = strtok(getVal, separator);
		
		
		// Check each column
		while (dbVal != NULL && dbVal[0] != '\0') {
			// Remove newline character if it is present at the token's end
			if (dbVal[strlen(dbVal) - 1] == '\n') {
				dbVal[strlen(dbVal) - 1] = '\0';
				printf("Removed a newline from %s\n", dbVal);
			}
			else {
				printf("Didn't remove the character %c\n", dbVal[strlen(dbVal) - 1]);
			}
			
			// Check dbVal vs whereVal
			if (curCol == whereCol && strcmp(dbVal, whereVal) == 0) {
				// Skip finding the next row; this is the final row
				printf("Found it!\n");
				goto found;
			}
			else if (dbVal != NULL) {
				printf("Column %d: DB - %s ### Search - %s\n", curCol, dbVal, whereVal);
			}
			
			// Get the current leftmost column using strtok
			dbVal = strtok(NULL, separator); // Passing NULL so each column is removed from the buffer
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
	return -1;
	
found:
	printf("Hi\n");
	int foundLength = (strlen(fullRow) + 1) * sizeof(char);
	printf("Successful return %d\n", foundLength);
	
	free(fullRow);
	printf("HEER\n");
	if (dbVal != NULL) {
		printf("Y tho %s\n", dbVal);
		free(dbVal);
	}
	else {
		printf("Successful return\n");
	}
	
	return foundLength;
	//return curRow;
}

int performUpdate(FILE* fDB, int updateRow, const char* newRow) {
	int curRow = 0;
	char newData[BUFF_SIZE];
	char curData[BUFF_SIZE];
	
	int i = 0;
	while (i < BUFF_SIZE && newRow[i] != '\0') {
		newData[i] = newRow[i];
		i++;
	}
	
	printf("New data: %s\n", newData);
	
	fgets(curData, BUFF_SIZE, fDB);
	printf("Reset row: %s\n", curData);
	
	// The cursor is now at the line at which the update should take place; write newRow to the file
	printf("Doin it\n");
	//fwrite(newRow, sizeof(char), strlen(newRow), fDB);
	fputs(newData, fDB);
	printf("Did it?\n");
	
	return 0;
}

int database(int cmdType, int col, const char* tbl, const char* setVal, int whereCol, const char* whereVal) {
	int dbSuccess = 0;
	printf("setVal: %s whereVal: %s\n", setVal, whereVal);
	printf("setVal: %p whereVal: %p\n", setVal, whereVal);
	
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
		fputs(setVal, fDB);
		fclose(fDB);
		break;
	case 3: // UPDATE
		fDB = fopen(tbl, "r+"); // Open in read-write mode (find existing record and update)
		int byteReset = findUpdateRow(fDB, whereVal, whereCol);
		printf("HEER: %d\n", byteReset);
		
		// If the value was found, update the associated row with the new info
		if (byteReset != -1) {
			// Use fseek to move the cursor back to the start of the current row and overwrite
			printf("Moving back %d characters\n", byteReset);
			fseek(fDB, -byteReset, SEEK_CUR);
			
			if (fDB == NULL) {
				printf("Dang\n");
			}
			else {
				printf("DUH\n");
			}
			
			printf("H-here? setVal: %s\n", setVal);
			
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

int updateClient(char* registerData, int isCustomer) {
	int rowsAffected = 0;
	printf("Is customer? %d\n", isCustomer);
	// Update sellerInformation or customerInformation
	if (isCustomer) {
		// Update the customer's information
		printf("New customer data: %s\n", registerData);
		rowsAffected = database(UPDATE, -1, "clientInformation.txt", registerData, 5, "Basketball");
	}
	else {
		// Update the seller's information
		rowsAffected = database(UPDATE, 0, "sellerInformation.txt", registerData, 0, NULL);
	}
	printf("He're\n");
	
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
	
	char dummyUpdate[] = "Teh|tesTD|Dadt|Tet\n";
	printf("%s is the dummy data\n", dummyUpdate);
	updateClient(dummyUpdate, 1);
	return 0;
}