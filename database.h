// Kyle Kentner
// 11/24/2021
// kyle.kentner@okstate.edu
// Defining functions to handle database operations

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "linkedlist.h"

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

void getTempName(char* dbTemp, const char* prefix, const char* tbl) {
	// Concatenate the prefix and the DB name, and store in dbTemp
	for (int i = 0, j = 0; i < strlen(prefix) + strlen(tbl) + 1; i++) {
		if (i < strlen(prefix)) {
			// Place the prefix
			dbTemp[i] = prefix[i];
		}
		else if (i < strlen(prefix) + strlen(tbl)) {
			// Place the existing filename
			dbTemp[i] = tbl[j];
			j++;
		}
		else {
			// Place the terminating null character
			dbTemp[i] = '\0';
		}
	}
}

// Handles the Select query; returns a char* value which points to the row that is found
char* findRow(FILE* fDB, int whereCol, const char* whereVal) {
	// Buffer variables
	char getVal[BUFF_SIZE]; // Buffer for the current DB row
	char* fullRow = (char*) malloc(BUFF_SIZE * sizeof(char)); // Char pointer for the current DB row
	char* dbVal = (char*) malloc(BUFF_SIZE * sizeof(char)); // Char pointer for the current DB column
	const char separator[2] = "|"; // Delimiter to be used in the strtok function
	
	// Counter variables
	int curRow = 0, curCol = 0;
	
	// Initialize getVal with the first row
	fgets(getVal, BUFF_SIZE, fDB);
	
	// Check each row while getVal is not null and the full row was read (i.e. quit at EOF)
	while (getVal[0] != '\0' && strlen(getVal) > 2 && getVal[strlen(getVal) - 2] == '|') {
		// Store the current row in fullRow
		strcpy(fullRow, getVal);
		
		// Initialize dbVal with the first column in getVal
		strcpy(dbVal, strtok(getVal, separator));
		
		// Check each column
		while (curCol <= whereCol && dbVal != NULL && dbVal[0] != '\n' && dbVal[0] != '|') {
			// Check dbVal vs whereVal
			if (curCol == whereCol && strcmp(dbVal, whereVal) == 0) {
				// Assign fullRow to retRow to pass back by reference
				printf("Column %d: DB - %s ### Search - %s\n", curCol, dbVal, whereVal);
				
				// Quit searching
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
	
	// If not found, return NULL
	free(fullRow);
	free(dbVal);
	return NULL;
	
found:
	// fullRow must be freed outside of this function
	free(dbVal);
	return fullRow;
}

// Handles both Update and Delete commands; returns an integer value indicating success (>= 0) or failure (< 0)
int findAndReplaceRow(FILE* fDB, const char* dbName, int whereCol, const char* whereVal, const char* newRow, int cmd) {
	// Pointer to a new file which will eventually overwrite the original DB
	FILE* fNewDB = fopen(dbName, "w");
	
	// Buffer variables
	int foundLength = -1; // Will record the length of the line in which whereVal is found
	char getVal[BUFF_SIZE]; // Buffer for the current DB row
	char* fullRow = (char*) malloc(BUFF_SIZE * sizeof(char)); // Char pointer for the current DB row
	char* dbVal = (char*) malloc(BUFF_SIZE * sizeof(char)); // Char pointer for the current DB column
	const char separator[2] = "|"; // Delimiter to be used in the strtok function
	
	// Counter variables
	int curRow = 0, curCol = 0;
	
	// Initialize getVal with the first row
	fgets(getVal, BUFF_SIZE, fDB);
	
	// Check each row
	while (getVal[0] != '\0') {
		// Store the current row in fullRow
		strcpy(fullRow, getVal);
		
		// Initialize dbVal with the first column in getVal (need strcpy?)
		strcpy(dbVal, strtok(getVal, separator));
		
		// Check each column
		while (curCol <= whereCol && dbVal != NULL && dbVal[0] != '\n' && dbVal[0] != '|') {
			// Check dbVal vs whereVal
			if (curCol == whereCol && strcmp(dbVal, whereVal) == 0) {
				// Place setVal instead of the existing row
				printf("Column %d: DB - %s ### Search - %s\n", curCol, dbVal, whereVal);
				
				// Record the length of the last row
				foundLength = (strlen(fullRow) + 1) * sizeof(char);
				
				// Check the command
				if (cmd == UPDATE) {
					// If Update, place the new row
					fputs(newRow, fNewDB);
					fputc('\n', fNewDB);
				}
				// If Delete, drop the record by omission
				
				// Skip placing the existing row
				goto found;
			}
			else if (dbVal != NULL) {
				printf("Column %d: DB - %s ### Search - %s\n", curCol, dbVal, whereVal);
			}
			
			// Get the current leftmost column using strtok
			strcpy(dbVal, strtok(NULL, separator)); // Passing NULL so each column is removed from the buffer
			curCol++;
		}
		
		// Write the current row in the temp file
		fputs(fullRow, fNewDB);
		
	found:	
		// Get the next row
		memset(getVal, 0, BUFF_SIZE); // Clear getVal
		fgets(getVal, BUFF_SIZE, fDB); // Use fgets for the next row
		curRow++; // Increment curRow
		curCol = 0; // Reset curCol
	}
	
	// If not found, return -1
	free(fullRow);
	free(dbVal);
	fclose(fNewDB);
	return foundLength;
}

// Selects the first row where some field (whereCol) matches the search criterion (whereVal)
// TODO: Accomodate multiple records
// 
int select(const char* tbl, int whereCol, char* whereVal) {
	int dbSuccess = 0;
	
	struct node_t* resList = (struct node_t*) malloc(sizeof(struct node_t));
	struct node_t* curRes = resList;
	
	// File pointer for the client's table
	FILE* fDB;
	
	// A const char prefix for the DB's filename, and a pointer to the full temp name
	char prefix[] = "temp_";
	char* dbTemp;
	
	 // Open in read-only mode
	fDB = fopen(tbl, "r");
	
	// Select the row
	char* retVal;
	
	do {		
		retVal = findRow(fDB, whereCol, whereVal);

		// Store the pointer to the DB record in the node's data (may be NULL)
		curRes->data = retVal;
		
		// If the value was found, rename the dbTemp file to overwrite the existing DB
		if (retVal != NULL) {
			printf("The row was found! %s\n", retVal);
			
			// TODO: Return linked list to calling function and convert to struct
			
			// malloc a new node and make curRes point to it
			curRes->next = (struct node_t*) malloc(sizeof(struct node_t));
			curRes = curRes->next;
			
			//free(retVal);
		}
	} while (retVal != NULL && retVal[0] != '\0');
	
	// For testing purposes; print out all results found
	curRes = resList;
	struct node_t* prevRes;
	
	// Print while data is available
	int i = 1;
	while (curRes->data != NULL) {
		printf("Selection %d: %s\n", i, (char*) curRes->data);
		i++;
		
		// Free the data after printing
		free(curRes->data);
		
		// Remember this node
		prevRes = curRes;
		
		// Advance
		curRes = curRes->next;
		
		// Free the previous node
		free(prevRes);
	}
	
	free(curRes);
	
	fclose(fDB);
	
	return dbSuccess;
}

// Inserts a row at the end of the file
// TODO: Ensure unique key, place in sequential order
//
int insert(const char* tbl, char* setVal) {
	int dbSuccess = -1; // Failure
	
	// File pointer for the client's table
	FILE* fDB;
	
	// A const char prefix for the DB's filename, and a pointer to the full temp name
	char prefix[] = "temp_";
	char* dbTemp;
	
	// Open in append mode
	fDB = fopen(tbl, "a");
	if (fDB != NULL) {
		fputs(setVal, fDB);
		fputc('\n', fDB); // Add the newline character separately
		fclose(fDB);
		dbSuccess = 0;
	}
	
	return dbSuccess;
}

// Updates one or more records where some field (whereCol) matches some criterion (whereVal)
//
int update(const char* tbl, char* setVal, int whereCol, char* whereVal) {
	int dbSuccess = 0, byteReset = -1;
	
	// File pointer for the client's table
	FILE* fDB;
	
	// A const char prefix for the DB's filename, and a pointer to the full temp name
	char prefix[] = "temp_";
	char* dbTemp;
	
	// Open in read-write mode (find existing record and update)
	fDB = fopen(tbl, "r+");
	
	// Create the temp file's name on the heap, adding an extra char for null
	dbTemp = (char*) malloc((strlen(prefix) + strlen(tbl) + 1) * sizeof(char));
	
	// Initialize dbTemp
	getTempName(dbTemp, prefix, tbl);
	
	// Perform the update
	byteReset = findAndReplaceRow(fDB, dbTemp, whereCol, whereVal, setVal, UPDATE);
	
	// If the value was found, rename the dbTemp file to overwrite the existing DB
	fclose(fDB);
	if (byteReset != -1) {
		// Set the temp file's name to the original file's name
		remove(tbl);
		if (rename(dbTemp, tbl)) {
			// Failure
			printf("Couldn't rename\n");
			dbSuccess = -2;
		}
	}
	else {
		// The whereVal was not found
		printf("Not found\n");
		
		// Remove the temp file
		remove(dbTemp);
		dbSuccess = 1;
	}
	
	free(dbTemp);
			
	return dbSuccess;
}

// Deletes one or more rows where some field (whereCol) matches some criterion (whereVal)
//
int drop(const char* tbl, int whereCol, char* whereVal) {
	int dbSuccess = 0, byteReset = -1;
	
	// File pointer for the client's table
	FILE* fDB;
	
	// A const char prefix for the DB's filename, and a pointer to the full temp name
	char prefix[] = "temp_";
	char* dbTemp;
	
	// Open in read-write mode (find existing record and delete)
	fDB = fopen(tbl, "r+");
	
	// Create the temp file's name on the heap, adding an extra char for null
	dbTemp = (char*) malloc((strlen(prefix) + strlen(tbl) + 1) * sizeof(char));
	
	// Initialize dbTemp
	getTempName(dbTemp, prefix, tbl);
	
	// Perform the deletion
	byteReset = findAndReplaceRow(fDB, dbTemp, whereCol, whereVal, NULL, DELETE);
	
	// If the value was found, rename the dbTemp file to overwrite the existing DB
	fclose(fDB);
	if (byteReset != -1) {
		// Set the temp file's name to the original file's name
		remove(tbl);
		if (rename(dbTemp, tbl)) {
			// Failure
			printf("Couldn't rename\n");
			dbSuccess = -2;
		}
	}
	else {
		// The whereVal was not found
		printf("Not found\n");
		
		// Remove the temp file
		remove(dbTemp);
		dbSuccess = 1;
	}
	
	free(dbTemp);
	
	return dbSuccess;
}

int database(int cmdType, int col, const char* tbl, const char* setVal, int whereCol, const char* whereVal);

// Facilitates the Insert command for a customer or seller
int registerClient(char* registerData, int isCustomer) {
	int registerSuccess = 0;
	
	// Insert into sellerInformation or customerInformation
	if (isCustomer) {
		// Add the user to the database as a customer (TODO: Check for conflicts?)
		registerSuccess = insert("clientInformation.txt", registerData);
	}
	else {
		// Add the user to the database as a seller (TODO: Check for conflicts?)
		registerSuccess = insert("sellerInformation.txt", registerData);
	}
	
	return registerSuccess;
}

// TESTING PURPOSES ONLY
int selectClient(int isCustomer) {
	int rowsAffected = 0;
	
	// Select from sellerInformation or customerInformation
	if (isCustomer) {
		// TODO: Pass unique identifier to selectClient (currently hardcoded with CustomerID of 5)
		// Select a customer's information
		rowsAffected = select("clientInformation.txt", 0, "5");
	}
	else {
		// Select a seller's information
		rowsAffected = select("sellerInformation.txt", 0, NULL);
	}
	
	return rowsAffected;
}

// TESTING PURPOSES ONLY
int removeClient(int isCustomer) {
	int rowsAffected = 0;
	
	// Update sellerInformation or customerInformation
	if (isCustomer) {
		// TODO: Pass unique identifier to removeClient (currently hardcoded with CustomerID of 5)
		// Update the customer's information
		rowsAffected = drop("clientInformation.txt", 0, "5");
	}
	else {
		// Update the seller's information
		rowsAffected = drop("sellerInformation.txt", 0, NULL);
	}
	
	return rowsAffected;
}

// Facilitates the Update command for a customer or seller
int updateClient(char* registerData, int isCustomer) {
	int rowsAffected = 0;
	
	// Update sellerInformation or customerInformation
	if (isCustomer) {
		// TODO: Pass unique identifier to updateClient (currently hardcoded with CustomerID of 5)
		// Update the customer's information
		rowsAffected = update("clientInformation.txt", registerData, 0, "5");
	}
	else {
		// Update the seller's information
		rowsAffected = update("sellerInformation.txt", registerData, 0, NULL);
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
		purchaseReturnSuccess = select("productInformation.txt", PRODUCT_ID, productID);
	}
	
	// Update the quantity of a product in productInformation; reflect changes in billingInformation and customerOrder
	// TODO: Check if client has permission to access productInformation, billingInformation, and customerOrder
	//purchaseReturnSuccess = database(UPDATE, QUANTITY_A, "productInformation.txt", PRODUCT_ID, productID);
	
	return purchaseReturnSuccess;
}