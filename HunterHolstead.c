//HunterHolstead.c
//Client
//Hunter Holstead

#include <stdio.h>

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
	
	while(1)
	{
		printf("%s\n", "Please enter your user ID to login, if you are not registered enter -1:");
		scanf("%d", &clientType);
		
		runSellerClient();
		runBuyerClient();
		registerClient();
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

void runBuyerClient()
{
	int input;
	int loop = 1;
	
	while(loop == 1)
	{
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
			//showOrders();
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

void runSellerClient()
{
	int input;
	int loop = 1;
	
	while(loop == 1)
	{
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
			//seeOrders();
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

int main()
{
	CheckID();
	
	return 0;
}
