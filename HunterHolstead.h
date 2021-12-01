#include "socketConnection.h"

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

void setSellerInformation();
void setCustomerInformation();
void setProductInformation();
void setBillingInformation();
void RunClient(struct socket_t* serverSocket);
void runBuyerClient(int ID);
void runSellerClient(int ID);
void registerClient();