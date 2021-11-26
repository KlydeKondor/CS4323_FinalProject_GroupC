// Kyle Kentner
// 11/24/2021
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
#define BUYER_ID_PK 0 // Customer ID (customerInformation)
#define BUYER_ID_FK 1 // Customer ID (billingInformation)
#define QUANTITY_P 2 // Quantity purchased (customerOrder)

#define SELLER_ID_PK 0 // Seller ID (sellerInformation)
#define SELLER_ID_FK 2 // Seller ID (productInformation)

#define PRODUCT_ID_PK 0 // Product ID (productInformation)
#define PRODUCT_ID_FK 1 // Product ID (customerOrder)
#define QUANTITY_A 3 // Quantity available (productInformation)

#define ORDER_ID_PK 0 // Order ID (billingInformation)
#define ORDER_ID_FK 0 // Order ID (customerOrder)

typedef struct Records {
	struct Records* prev;
	struct Records* next;
	void* current;
} Record;

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
	char* deliveryAddress;
	float totalOrderPrice;
};

// Print out all results found
void printRecords(Record* records, int cols) {
	printf("\n");
	
	// Buffer for the current DB row
	char getVal[BUFF_SIZE];
	char* dbVal = (char*) malloc(BUFF_SIZE * sizeof(char)); // Char pointer for the current DB column
	const char separator[2] = "|"; // Delimiter to be used in the strtok function
	
	// Print while data is available
	while (records != NULL && records->current != NULL) {
		// Get the leftmost column
		strcpy(getVal, records->current);
		strcpy(dbVal, strtok(getVal, separator));
		
		int i = 0;
		while (i < cols) {
			int j = strlen(dbVal);
			
			// Print the column
			if (strcmp("\n", dbVal)) {
				printf("%s", dbVal);
			}
			
			// Try to get the next column
			if (i < cols - 1) {
				// Print delimiting spaces
				while (j < 25) {
					printf(" ");
					j++;
				}
				
				strcpy(dbVal, strtok(NULL, separator)); // Passing NULL so each column is removed from the buffer
			}
			i++;
		}

		// Free the data after printing
		if (records->next == NULL) {
			free(records->current);
			free(records);
			records = NULL;
		}
		else {
			// Advance
			records = records->next;
			
			// Free the previous node
			free(records->prev->current);
			free(records->prev);
			printf("\n");
		}
	}
	
	// Free the last record as needed
	if (records != NULL) {
		free(records);
	}
	
	// Free dbVal
	free(dbVal);
}

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
				// Quit searching
				goto found;
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
Record* select(const char* tbl, int whereCol, char* whereVal) {
	int dbSuccess = 0;
	
	Record* resList = (Record*) malloc(sizeof(Record));
	Record* head = resList;
	resList->prev = NULL;
	
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
		resList->current = retVal;
		
		// If the value was found, rename the dbTemp file to overwrite the existing DB
		if (retVal != NULL) {
			// malloc a new node and make resList point to it
			resList->next = (Record*) malloc(sizeof(Record));
			resList->next->prev = resList;
			resList = resList->next;
		}
	} while (retVal != NULL && retVal[0] != '\0');
	
	fclose(fDB);
	
	return head;
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
		registerSuccess = insert("customerInformation.txt", registerData);
	}
	else {
		// Add the user to the database as a seller (TODO: Check for conflicts?)
		registerSuccess = insert("sellerInformation.txt", registerData);
	}
	
	return registerSuccess;
}

// TESTING PURPOSES ONLY
Record* selectClient(int isCustomer) {
	Record* rowsSelected = NULL;
	
	// Select from sellerInformation or customerInformation
	if (isCustomer) {
		// TODO: Pass unique identifier to selectClient (currently hardcoded with CustomerID of 5)
		// Select a customer's information
		rowsSelected = select("customerInformation.txt", 0, "5");
	}
	else {
		// Select a seller's information
		rowsSelected = select("sellerInformation.txt", 0, NULL);
	}
	
	return rowsSelected;
}

// TESTING PURPOSES ONLY
int removeClient(int isCustomer) {
	int rowsAffected = 0;
	
	// Update sellerInformation or customerInformation
	if (isCustomer) {
		// TODO: Pass unique identifier to removeClient (currently hardcoded with CustomerID of 5)
		// Update the customer's information
		rowsAffected = drop("customerInformation.txt", 0, "5");
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
		rowsAffected = update("customerInformation.txt", registerData, 0, "5");
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

Record* purchaseReturnProduct(char* productID, char* customerID, char* orderID, int change) {
	Record* purchaseReturns = NULL;
	
	if (change > 0) {
		// If able to read, get the current quantity available
		// TODO: Get value from database, see if purchase can be made
		purchaseReturns = select("productInformation.txt", PRODUCT_ID_PK, productID);
	}
	
	// Update the quantity of a product in productInformation; reflect changes in billingInformation and customerOrder
	// TODO: Check if client has permission to access productInformation, billingInformation, and customerOrder
	//purchaseReturnSuccess = database(UPDATE, QUANTITY_A, "productInformation.txt", PRODUCT_ID, productID);
	
	return purchaseReturns;
}

struct productInfo* getProduct() {
	return (struct productInfo*) malloc(sizeof(struct productInfo));
}

void viewProductsSeller(char* sellerID) {
	// Select and display all products where this client is the seller
	Record* products = select("productInformation.txt", SELLER_ID_FK, sellerID);
	
	// Get column headers
	char hdrText[] = "Product ID|Product Description|Seller ID|Quantity Available|Unit Price|";
	Record* headers = (Record*) malloc(sizeof(Record));
	headers->current = (char*) malloc(sizeof(char) * (strlen(hdrText) + 1));
	headers->next = NULL;
	strcpy((char*) (headers->current), hdrText);
	
	// Display headers (headers freed in printRecords)
	printRecords(headers, 5);
	
	// Display records (records freed in printRecords)
	printRecords(products, 5);
	printf("\n");
}

void viewProductsBuyer(char* productID) {
	// Select and display information for the buyer's desired product
	Record* products = select("productInformation.txt", PRODUCT_ID_PK, productID);
	
	// Get column headers
	char hdrText[] = "Product ID|Product Description|Seller ID|Quantity Available|Unit Price|";
	Record* headers = (Record*) malloc(sizeof(Record));
	headers->current = (char*) malloc(sizeof(char) * (strlen(hdrText) + 1));
	headers->next = NULL;
	strcpy((char*) (headers->current), hdrText);
	
	// Display headers (headers freed in printRecords)
	printRecords(headers, 5);
	
	// Display records (records freed in printRecords)
	printRecords(products, 5);
	printf("\n");
}

void extractColumn(Record* records, int extCol, Record* outRecords, int cols) {
	// Buffer for the current DB row
	char getVal[BUFF_SIZE];
	char* dbVal = (char*) malloc(BUFF_SIZE * sizeof(char)); // Char pointer for the current DB column
	const char separator[2] = "|"; // Delimiter to be used in the strtok function
	
	while (records != NULL && records->current != NULL) { // && ((char*) (records->current))[0] != '\0') {
		// Get the leftmost column
		strcpy(getVal, records->current);
		strcpy(dbVal, strtok(getVal, separator));
		
		int i = 0;
		while (i < cols) {
			// Get the column which must be extracted
			if (i == extCol) {
				outRecords->current = (char*) malloc(sizeof(char) * strlen(dbVal));
				strcpy(((char*) outRecords->current), dbVal);
			}
			
			// Try to get the next column
			if (i < cols - 1) {
				strcpy(dbVal, strtok(NULL, separator)); // Passing NULL so each column is removed from the buffer
			}
			
			i++;
		}
		
		// Free the row's data
		if (records->next == NULL) {
			free(records->current);
			free(records);
			records = NULL;
			outRecords->next = NULL;
		}
		else {
			// Advance
			records = records->next;
			
			// Free the previous node
			free(records->prev->current);
			free(records->prev);
			
			// Advance outRecords
			outRecords->next = (Record*) malloc(sizeof(Record));
			outRecords->next->prev = outRecords;
			outRecords = outRecords->next;
			outRecords->current = NULL;
		}
	}
	
	if (records != NULL) {
		free(records);
	}
	
	free(dbVal);
}

Record* selectLoop(Record* searchRecords, const char* tbl, int searchCol) {
	Record* outRecords = NULL;
	
	while (searchRecords != NULL && searchRecords->current != NULL) {
		Record* curRecords = select(tbl, searchCol, (char*) (searchRecords->current));
		
		// Free the row's data
		if (searchRecords->next == NULL || searchRecords->next->current == NULL) {
			free(searchRecords->current);
			free(searchRecords);
			searchRecords = NULL;
		}
		else {
			// Advance
			searchRecords = searchRecords->next;
			
			// Free the previous node
			free(searchRecords->prev->current);
			free(searchRecords->prev);
		}
		
		// Accumulate records in outRecords
		if (outRecords == NULL && curRecords != NULL) {
			outRecords = (Record*) curRecords;
		}
		else if (curRecords != NULL) {
			outRecords->next = curRecords;
			outRecords->next->prev = outRecords;
			outRecords = outRecords->next;
		}
	}
	
	if (searchRecords != NULL) {
		free(searchRecords);
	}
	
	return outRecords;
}

void viewOrdersSeller(char* sellerID) {
	// Select all products where this client is the seller
	Record* products = select("productInformation.txt", SELLER_ID_FK, sellerID);
	
	// Extract each Product ID from products
	Record* productIDs = (Record*) malloc(sizeof(Record));
	Record* headID = productIDs;
	extractColumn(products, PRODUCT_ID_PK, productIDs, 5);
	
	// Select and display all orders for each of these products
	productIDs = headID;
	Record* orders = selectLoop(productIDs, "customerOrder.txt", PRODUCT_ID_FK);
	
	// Get column headers
	char hdrText[] = "Order ID|Product ID|Quantity Ordered|Delivery Address|Total Order Price|";
	Record* headers = (Record*) malloc(sizeof(Record));
	headers->current = (char*) malloc(sizeof(char) * (strlen(hdrText) + 1));
	headers->next = NULL;
	strcpy((char*) (headers->current), hdrText);
	
	// Display headers (headers freed in printRecords)
	printRecords(headers, 5);
	
	// Display records (records freed in printRecords)
	printRecords(orders, 5);
	printf("\n");
}

void viewOrdersBuyer(char* buyerID) {
	// Select all billings where this client is the buyer
	Record* billings = select("billingInformation.txt", BUYER_ID_FK, buyerID);
	
	// Extract each Order ID from billings
	Record* orderIDs = (Record*) malloc(sizeof(Record));
	Record* headID = orderIDs;
	extractColumn(billings, ORDER_ID_PK, orderIDs, 4);
	
	// Select and display all of the client's orders listed in customerOrder
	orderIDs = headID;
	Record* orders = selectLoop(orderIDs, "customerOrder.txt", ORDER_ID_FK);
	
	// Get column headers
	char hdrText[] = "Order ID|Product ID|Quantity Ordered|Delivery Address|Total Order Price|";
	Record* headers = (Record*) malloc(sizeof(Record));
	headers->current = (char*) malloc(sizeof(char) * (strlen(hdrText) + 1));
	headers->next = NULL;
	strcpy((char*) (headers->current), hdrText);
	
	// Display headers (headers freed in printRecords)
	printRecords(headers, 5);
	
	// Display records (records freed in printRecords)
	printRecords(orders, 5);
	printf("\n");
}

void viewBillingInfo(char* buyerID) {
	// Select and display all orders where this client is the buyer
	Record* orders = select("billingInformation.txt", BUYER_ID_PK, buyerID);
	
	// Get column headers
	char hdrText[] = "Order ID|Customer ID|Customer Address|Total Order Price|";
	Record* headers = (Record*) malloc(sizeof(Record));
	headers->current = (char*) malloc(sizeof(char) * (strlen(hdrText) + 1));
	headers->next = NULL;
	strcpy((char*) (headers->current), hdrText);
	
	// Display headers (headers freed in printRecords)
	printRecords(headers, 4);
	
	// Display records (records freed in printRecords)
	printRecords(orders, 4);
	printf("\n");
}