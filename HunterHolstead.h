//HunterHolstead.c

#include <stdio.h>

//Future plans consist of creating structs for sellerInformation, buyerInformation, productInformation, billingInformation which have
// the parameters as saved variables for easier access with getter methods

//All of these setter methods can be modified to have parameters passed into them without user input

/*1. sellerInformation database to store sellers’ information (Seller ID, Seller Name, Contact Number, 
Seller’s Contact Address) 
• Seller ID: ID number of a seller,  
• Seller Name: first and last name of the seller,  
• Contact Number: 10-digits contact number of the seller in the form XXX-XXX-XXXX,  
• Seller’s Contact Address: contact address of the seller */
void setSellerInformation()
{
	int sellerID;
	char sellerName[100];
	char contactNumber[100];
	char contactAddress[100];

   printf("%s\n","Please enter your Seller ID:");
   scanf("%d", &sellerID);
   printf("\nsellerID entered: %d ", sellerID);
   
   printf("%s\n","\nPlease enter your name:");
   scanf("%s", sellerName);
   printf("\nsellerName entered: %s ", sellerName);
   
   printf("%s\n","\nPlease enter your contact number:");
   scanf("%s", contactNumber);
   printf("\ncontactNumber entered: %s ", contactNumber);
   
   printf("%s\n","\nPlease enter your contact address:");
   scanf("%s", contactAddress);
   printf("\ncontactAddress entered: %s ", contactAddress);
   
   printf("%s\n", "\nSeller Information has been sent to the database."); 
   //sendToSIDatabase(sellerID, sellerName, contactNumber, contactAddress);
	
}

/*2. customerInformation database to store customers’ information (Customer ID, Customers Name, Contact Number, Customer’s Contact Address) 
• Customer ID: ID number of a customer,  
• Customers Name: first and last name of the customer,  
• Contact Number: 10-digits contact number of the seller in the form XXX-XXX-XXXX,  
• Customer’s Contact Address: contact address of the customer */
void setCustomerInformation() 
{
	int customerID;
	char customerName[100];
	char contactNumber[100];
	char contactAddress[100];

   printf("%s\n","Please enter your Customer ID:");
   scanf("%d", &customerID);
   printf("\nCustomerID entered: %d ", customerID);
   
   printf("%s\n","\nPlease enter your name:");
   scanf("%s", customerName);
   printf("\nCustomerName entered: %s ", customerName);
   
   printf("%s\n","\nPlease enter your contact number:");
   scanf("%s", contactNumber);
   printf("\ncontactNumber entered: %s ", contactNumber);
   
   printf("%s\n","\nPlease enter your contact address:");
   scanf("%s", contactAddress);
   printf("\ncontactAddress entered: %s ", contactAddress);
   
   printf("%s\n", "\nCustomer Information has been sent to the database."); 
   //sendToCIDatabase(customerID, customerName, contactNumber, contactAddress);
	
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
void setBillingInformation() 
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
   scanf("%f", totalOrderPrice);
   printf( "\ntotalOrderPrice entered: %f ", totalOrderPrice);
   
   printf("%s\n", "Billing Information has been sent to the database."); 
   //sendToBIDatabase(orderID,customerID,customerContactAdress,totalOrderPrice);
	
}

/*5. customerOrder  database  contains  information  for  the  billing  purpose  (Order  ID,  Product  ID, 
Quantity Purchased, Delivery Address, Total Price) 
• Order ID: ID number of the order,  
• Product ID: ID number of the product,  
• Quantity Purchased: number of items (belonging to that Product ID) bought,  
• Delivery Address: Same as customer’s  contact  address  from  customerInformation 
database  
• Total  Price:  total  price  of  the  product  i.e.  the  product  of  each  product  price  times  the 
quantity purchased */
void setCustomerOrder() 
{
	int orderID;
	int productID;
	int productQuantity;
	char customerContactAddress[100];
	float totalOrderPrice;

   printf("%s\n","Please enter your order ID:");
   scanf("%d", &orderID);
   printf( "\norderID entered: %d ", orderID);
   
   printf("%s\n","\nPlease enter your Product ID:");
   scanf("%d", &productID);
   printf( "\nproductID entered: %d ", productID);
   
   printf("%s\n","\nPlease enter the quantity of product available:");
   scanf("%d", &productQuantity);
   printf( "\nproductQuantity entered: %d ", productQuantity);
   
   printf("%s\n","\nPlease enter your contact address:");
   scanf("%s", customerContactAddress);
   printf( "\ncustomerContactAddress entered: %s ", customerContactAddress);
   
   
   printf("%s\n","\nPlease enter your Total Order Price:");
   scanf("%f", totalOrderPrice);
   printf( "\ntotalOrderPrice entered: %f ", totalOrderPrice);
   
   printf("%s\n", "Customer Order has been sent to the database.");  
   //sendToCODatabase(orderID,productID,productQuantity,customerContactAdress,totalOrderPrice);
	
}

//• allow the clients to register in the system, 
void RegisterClient()
{
	int clientType;
	
	printf("%s\n", "Enter 1 if you are a seller or 0 if you are a buyer:"); //Just a test way of distinguishing buyer and seller. Will be changed later
	scanf("%d", &clientType);
	
	if(clientType == 1)
	{
		setSellerInformation();
	}
	if(clientType == 0)
	{
		setCustomerInformation();
	}
	else
	{
		printf("%s\n", "You did not enter a correct response");
	}
	
	runClient(&clientType);
}

void runClient(int *clientType)
{
	int input;
	int n = 1;
	
	if(*clientType==1)
	{
		//seller
		while(1)
		{
			printf("%s\n", "Hello seller, enter 1 to set Product Information or 2 to exit"); //This is a test for Product Information
			scanf("%d", &input);
			
			if(input == 1)
			{
				setProductInformation();
			}
			else
			{
				n = 0;
				break;
			}
		}
	}
	if(*clientType==0)
	{
		//buyer
		while(1)
		{
			printf("%s\n", "Hello Buyer, enter 1 to set Billing Information, 2 for setting Customer Order, and 3 to exit."); //This is a test for Product Information
			scanf("%d", &input);
			if(input == 1)
			{
				setBillingInformation();
			}
			if(input == 2)
			{
				setCustomerOrder();
			}
			else
			{
				n = 0;
				break;
			}
		}
	}
	else
	{
		if(n==1)
		{
			printf("%s\n", "Something went wrong and you were not identified");
		}
	}
		
}

/*int main()
{
	RegisterClient();
	
	return 0;
}*/

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