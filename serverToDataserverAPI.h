#include <stdbool.h>
#include "socketConnection.h"

#ifndef CS4323_FINALPROJECT_GROUPC_SERVERTODATASERVERAPI_H
#define CS4323_FINALPROJECT_GROUPC_SERVERTODATASERVERAPI_H

#define COMMAND_DELIMITER "$|$"
#define COMMAND_SUCCESS "|SUCCESS|"
#define COMMAND_FAIL "|FAILURE|"

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

bool registerNewClient(struct socket_t* dataServerSocket, int clientID, char* clientName, char* clientNumber, char* clientAddress);
bool updateClientInfo(struct socket_t* dataServerSocket, int clientID, char* clientName, char* clientNumber, char* clientAddress);
bool addProduct(struct socket_t* dataServerSocket, int productID, char* productDescription, char* sellerID, int quantity, float price);
bool updateProduct(struct socket_t* dataServerSocket, int productID, char* productDescription, char* sellerID, int quantity, float price);
bool deleteProduct(struct socket_t* dataServerSocket, int productID);
bool updateProductQuantity(struct socket_t* dataServerSocket, int productID, int quantity);
bool updateProductPrice(struct socket_t* dataServerSocket, int productID, float price);
bool updateBillingInfo(struct socket_t* dataServerSocket, int orderID, int clientID, char* address, float totalPrice);
bool addCustomerOrder(struct socket_t* dataServerSocket, int orderID, int productID, int quantity, char* address, float totalPrice);
char* getAllSellerProducts(struct socket_t* dataServerSocket, int clientID);
char* getProductInfo(struct socket_t* dataServerSocket, int productID);
char* getAllSellerOrders(struct socket_t* dataServerSocket, int clientID);
char* getAllBuyOrders(struct socket_t* dataServerSocket, int clientID);
char* getBillingInfo(struct socket_t* dataServerSocket, int clientID);

#endif //CS4323_FINALPROJECT_GROUPC_SERVERTODATASERVERAPI_H
