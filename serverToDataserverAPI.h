#include <stdbool.h>

#ifndef CS4323_FINALPROJECT_GROUPC_SERVERTODATASERVERAPI_H
#define CS4323_FINALPROJECT_GROUPC_SERVERTODATASERVERAPI_H

#define REGISTER_CLIENT "|REGISTER|"
#define UPDATE_CLIENT "|UPDATE_INFO|"
#define ADD_PRODUCT "|ADD_PRODUCT|"
#define UPDATE_PRODUCT "|UPDATE_PRODUCT|"
#define DELETE_PRODUCT "|DELETE_PRODUCT|"
#define UPDATE_PRODUCT_QUANTITY "|UPDATE_PRODUCT_QUANTITY|"
#define UPDATE_PRODUCT_PRICE "|UPDATE_PRODUCT_PRICE|"
#define UPDATE_BILLING_INFO "|UPDATE_BILLING_INFO|"
#define ADD_ORDER "|ADD_ORDER|"
#define GET_SELLER_PRODUCTS "|GET_SELLER_PRODUCTS|"
#define GET_PRODUCT_INFO "|GET_PRODUCT_INFO|"
#define GET_SELLER_ORDERS "|GET_SELLER_ORDERS|"
#define GET_BUY_ORDERS "|GET_BUY_ORDERS|"
#define GET_BILLING_INFO "|GET_BILLING_INFO|"

bool registerClient(int clientID, char* clientName, char* clientNumber, char* clientAddress);
bool updateClientInfo(int clientID, char* clientName, char* clientNumber, char* clientAddress);
bool addProduct(int productID, char* productDescription, char* sellerID, int quantity, float price);
bool updateProduct(int productID, char* productDescription, char* sellerID, int quantity, float price);
bool deleteProduct(int productID);
bool updateProductQuantity(int productID, int quantity);
bool updateProductPrice(int productID, float price);
bool updateBillingInfo(int orderID, int clientID, char* address, float totalPrice);
bool addCustomerOrder(int orderID, int productID, int quantity, char* address, float totalPrice);
char* getAllSellerProducts(int clientID);
char* getProductInfo(int productID);
char* getAllSellerOrders(int clientID);
char* getAllBuyOrders(int clientID);
char* getBillingInfo(int clientID);

#endif //CS4323_FINALPROJECT_GROUPC_SERVERTODATASERVERAPI_H
