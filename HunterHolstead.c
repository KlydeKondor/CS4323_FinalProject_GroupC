//Client.c
//Hunter Holstead

#include <stdio.h>
#include <stdlib.h>
#include "serverNetwork.h"
#include "socketConnection.h"
#include "serverToDataserverAPI.h"
#include "util.h"


//Structs for sellerInformation, customerInformation, productInformation, billingInformation
struct sellerInformation
{
	int sellerID;
	char sellerName[100];
	char contactNumber[100];
	char contactAddress[100];
};

struct customerInformation
{
	int customerID;
	char customerName[100];
	char contactNumber[100];
	char contactAddress[100];
};

struct productInformation
{
	int productID;
	char productDescription[100];
	int sellerID;
	int productQuantity;
	float productPrice;
};

struct billingInformation
{
	int orderID;
	int customerID;
	char customerContactAddress[100];
	float totalOrderPrice;
};



/*1. sellerInformation database to store sellers’ information (Seller ID, Seller Name, Contact Number, 
Seller’s Contact Address) 
• Seller ID: ID number of a seller,  
• Seller Name: first and last name of the seller,  
• Contact Number: 10-digits contact number of the seller in the form XXX-XXX-XXXX,  
• Seller’s Contact Address: contact address of the seller */
void setSellerInformation()
{ 

	struct sellerInformation SI; 

	//They don't set SellerID it must be sequential
   printf("%s\n","Please enter your Seller ID:");
   scanf("%d", &SI.sellerID);
   printf("\nsellerID entered: %d ", SI.sellerID);
   
   printf("%s\n","\nPlease enter your name:");
   scanf("%s", SI.sellerName);
   printf("\nsellerName entered: %s ", SI.sellerName);
   
   printf("%s\n","\nPlease enter your contact number:");
   scanf("%s", SI.contactNumber);
   printf("\ncontactNumber entered: %s ", SI.contactNumber);
   
   printf("%s\n","\nPlease enter your contact address:");
   scanf("%s", SI.contactAddress);
   printf("\ncontactAddress entered: %s ", SI.contactAddress);
   
   //sendSocket(sellerName, contactNumber, contactAddress);
   printf("%s\n", "\nSeller Information has been sent to the database."); 
	
   printf("\nYour seller ID is: %d ", SI.sellerID);
}

/*2. customerInformation database to store customers’ information (Customer ID, Customers Name, Contact Number, Customer’s Contact Address) 
• Customer ID: ID number of a customer,  
• Customers Name: first and last name of the customer,  
• Contact Number: 10-digits contact number of the seller in the form XXX-XXX-XXXX,  
• Customer’s Contact Address: contact address of the customer */
void setCustomerInformation() 
{

	struct customerInformation CI;

	//They don't set CustomerID it must be sequential
   printf("%s\n","Please enter your Customer ID:");
   scanf("%d", &CI.customerID);
   printf("\nCustomerID entered: %d ", CI.customerID);
	
   printf("%s\n","\nPlease enter your name:");
   scanf("%s", CI.customerName);
   printf("\nCustomerName entered: %s ", CI.customerName);
   
   printf("%s\n","\nPlease enter your contact number:");
   scanf("%s", CI.contactNumber);
   printf("\ncontactNumber entered: %s ", CI.contactNumber);
   
   printf("%s\n","\nPlease enter your contact address:");
   scanf("%s", CI.contactAddress);
   printf("\ncontactAddress entered: %s ", CI.contactAddress);
   
   //sendSocket(customerName, contactNumber, contactAddress);
   printf("%s\n", "\nCustomer Information has been sent to the database."); 
   
   printf("\nYour client ID is: %d ", CI.customerID);
	
}

/*3. productInformation  database  contains  information  about  the  product  (Product  ID,  Product 
Description, Seller ID, Product Quantity Available, Product Price) 
• Product ID: ID number of a product,  
• Product Description: name of the product,  
• Seller ID: ID of the seller who is selling the product,  
• Product Quantity Available: number of quantity available,  
• Product Price: price per unit */
void setProductInformation() 
{
	int productID;
	char productDescription[100];
	int sellerID;
	int productQuantity;
	float productPrice;

   printf("%s\n","Please enter your Product ID:");
   scanf("%d", &productID);
   printf( "\nproductID entered: %d ", productID);
   
   printf("%s\n","\nPlease enter your Product Description:");
   scanf("%s", productDescription);
   printf( "\nproductDescription entered: %s ", productDescription);
   
   printf("%s\n","\nPlease enter your Seller ID:");
   scanf("%d", &sellerID);
   printf( "\nsellerID entered: %d ", sellerID);
   
   printf("%s\n","\nPlease enter the quantity of product available:");
   scanf("%d", &productQuantity);
   printf( "\nproductQuantity entered: %d ", productQuantity);
   
   /*printf("%s\n","\nPlease enter your Product Price:");
   scanf("%f", productPrice);
   printf( "\nproductPrice entered: %f ", productPrice);*/
   
   printf("%s\n", "\nProduct Information has been sent to the database."); 
   //sendToPIDatabase(productID,productDescription,sellerID,productQuantity,productPrice);
	
}

/*4. billingInformation database contains information for the billing purpose (Order ID, Customer ID, 
Customer’s Billing Address, Total Order Price) 
• Order ID: ID number of the order,  
• Customer ID: ID of the customer who made the order,  
• Customer’s  Billing Address:  Same  as  customer’s  contact  address  from 
customerInformation database  
• Total  Order  Price:  total  order  price  i.e.  summation  of  all  the  items  that  customer  has 
purchased on that particular order */
void seeBillingInformation() 
{
	int orderID;
	int customerID;
	char customerContactAddress[100];
	float totalOrderPrice;

   printf("%s\n","Please enter your order ID:");
   scanf("%d", &orderID);
   printf( "\norderID entered: %d ", orderID);
   
   printf("%s\n","\nPlease enter your Customer ID:");
   scanf("%d", &customerID);
   printf( "\nCustomerID entered: %d ", customerID);
   
   printf("%s\n","\nPlease enter your contact address:");
   scanf("%s", customerContactAddress);
   printf( "\ncustomerContactAddress entered: %s ", customerContactAddress);
   
   
   printf("%s\n","\nPlease enter your Total Order Price:");
   scanf("%f", &totalOrderPrice);
   printf( "\ntotalOrderPrice entered: %f ", totalOrderPrice);
   
   //sendSocket(orderID,customerID,customerContactAdress,totalOrderPrice);
   printf("%s\n", "Billing Information has been sent to the database.");
	
}

//• allow the clients to register in the system, 
void CheckID()
{
	int clientType;
	int seller;
	int buyer;
	int ID;
	
	while(1)
	{
		printf("%s\n", "Please enter 0 if you are a Seller or 1 if you are a Buyer. If you are not registered enter -1:");
		scanf("%d", &clientType);
		
		if(clientType == 0)
		{
			printf("%s\n", "Please enter your User ID:");
			scanf("%d", &ID);
			runSellerClient(ID);
		}
		else if(clientType == 1)
		{
			printf("%s\n", "Please enter your User ID:");
			scanf("%d", &ID);
			runBuyerClient(ID);
		}
		else
		{
			registerClient();
		}
		//rselect(clientType)
		/*if(rselect(clientType) == seller)
		{
			runSellerClient();
		}
		if(rselect(clientType) == buyer)
		{
			runBuyerClient();
		}
		else
		{
			registerClient();
		}*/
	}
}

void runBuyerClient(int ID)
{
	int input;
	int loop = 1;
	//int inputID = ID;
	char* delimiter = "$|$";
	
	while(loop == 1)
	{
		//printf("%s\n", buyerID);
		
		char buffer[MAX_TCP_BUFFER_SIZE];
		char* command = GET_PRODUCT_INFO;
		sprintf(buffer, "%s%s%d", command, delimiter, ID);
		//printf("%s\n", buffer);
		
		
		//PrintProductInformation()
		printf("%s\n", "Hello Buyer, enter 1 to update your information, 2 to purchase a product, 3 to return a product"); //This is a test for Product Information
		printf("%s\n", " 4 to see your Billing Information, 5 to look at your orders or 6 to exit");
		scanf("%d", &input);
		
		if(input == 1)
		{
			setCustomerInformation();
		}
		else if(input == 2)
		{
			//purchaseProduct()
		}
		else if(input == 3)
		{
			//returnProduct();
		}
		else if(input == 4)
		{
			//showBillingInformation();
		}
		else if(input == 5)
		{
			//ViewOrderBuyer();
		}
		else if(input == 6)
		{
			loop = 0;
		}
		else
		{
			printf("%s\n", "You did not enter a correct response");
		}
	
	}
}

void runSellerClient(int ID)
{
	int input;
	int loop = 1;
	int inputID = ID;
	char sellerID[50];
	
	sprintf(sellerID,"%d",inputID);
	
	while(loop == 1)
	{
		printf("%s\n", sellerID);
		//PrintProductInformation()
		printf("%s\n", "Hello Seller, enter 1 to update your information, 2 to add a product, 3 to remove a product"); //This is a test for Product Information
		printf("%s\n", "4 to set a product's quanitity, 5 to set a product's price, 6 to see orders on your product, or 7 to exit");
		scanf("%d", &input);
		
		if(input == 1)
		{
			setSellerInformation();
		}
		else if(input == 2)
		{
			//addProduct()
		}
		else if(input == 3)
		{
			//removeProduct();
		}
		else if(input == 4)
		{
			//setProductQuantity();
		}
		else if(input == 5)
		{
			//setProductPrice();
		}
		else if(input == 6)
		{
			//ViewOrderSeller();
		}
		else if(input == 7)
		{
			loop = 0;
		}
		else
		{
			printf("%s\n", "You did not enter a correct response");
		}
	
	}
}

void registerClient()
{
	int clientType;
	int loop = 1;
	
	while(loop == 1)
	{
	
		printf("%s\n", "Enter 1 if you are a seller or 0 if you are a buyer:"); //Just a test way of distinguishing buyer and seller. Will be changed later
		scanf("%d", &clientType);
		
		if(clientType == 1)
		{
			setSellerInformation();
			loop = 0;
		}
		else if(clientType == 0)
		{
			setCustomerInformation();
			loop = 0;
		}
		else
		{
			printf("%s\n", "You did not enter a correct response");
		}
	}

}

/*
int main(int argc, char **argv) {
    seedRand();
    // Parse server port
    if(argc < 1) {
        printf("Port argument missing\nUsage: ./client.out <port>\n");
        exit(1);
    }

    long port;
    char* other;
    if((port = strtol(argv[0], &other, 10)) == 0) {
        if(port > (1 << 16)) { //Max port: 65536
            printf("Port outside of valid range 0 < port < 65536\n");
            exit(1);
        }
    }

    // Connect to the server
    struct socket_t* dataServerSocket = mallocSocket(SERVER_ADDRESS, port);
    connectSocket(dataServerSocket, 5, 2);
	
	CheckID();
	
	return 0;
}*/


int main()
{
	CheckID()
	
	return 0;
}

/* Future Plans
//Buyer
ClientID = 0 //consumer (buys the products)
//• allow the clients to update the information in the system: o both buyers and sellers can change their personal information 
UpdateClientInfo() 
//o buyers can purchase the product or return the product 
PurchaseProduct()
ReturnProduct()
//o buyers can look at the quantity of their product available and its price. 
//o sellers can look at the quantity of their products available and its price. 
getQuantity()
getPrice()
//o buyers can look at their orders. 
getBoughtOrders()
//o buyers can look at their billing information
getBillingInformation()
//----------------------------------------------------------------------------
//Seller
ClientID = 1 //seller (lists the products)
//• allow the clients to register in the system, 
// Same as client
RegisterClient()
//• allow the clients to update the information in the system: o both buyers and sellers can change their personal information 
// Same as client
UpdateClientInfo() 
//o sellers can add a new product or remove a product 
AddProduct()
RemoveProduct()
//o sellers can add or subtract the quantity of the product already existing in the database, including the price of the product 
ChangeQuantity() 
ChangePrice()
//• allow the clients to see all the relevant information from the productInformation, billingInformation and customerOrder database. 
getProductInformation()
getBillingInformation()
getCustomerOrder()
//o buyers can look at the quantity of their product available and its price. 
//o sellers can look at the quantity of their products available and its price. 
getQuantity()
getPrice()
//o sellers can look at all the customers’ order belonging to his products. 
getSoldOrders()
*/
