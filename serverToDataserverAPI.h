#include <stdbool.h>
#include "socketConnection.h"

#ifndef CS4323_FINALPROJECT_GROUPC_SERVERTODATASERVERAPI_H
#define CS4323_FINALPROJECT_GROUPC_SERVERTODATASERVERAPI_H

#define ROUTING_ID_INDEX 0
#define COMMAND_INDEX 1
#define DATA_INDEX 2

#define COMMAND_DELIMITER "$|$"
#define COMMAND_SUCCESS "|SUCCESS|"
#define COMMAND_FAILURE "|FAILURE|"
#define QUIT "|QUIT|"

#define REGISTER_CUSTOMER "|REGISTER_CUSTOMER|"
#define UPDATE_CUSTOMER "|UPDATE_CUSTOMER_INFO|"
#define REGISTER_SELLER "|REGISTER_SELLER|"
#define UPDATE_SELLER "|UPDATE_SELLER_INFO|"
#define ADD_PRODUCT "|ADD_PRODUCT|"
#define UPDATE_PRODUCT "|UPDATE_PRODUCT|"
#define DELETE_PRODUCT "|DELETE_PRODUCT|"
#define UPDATE_PRODUCT_QUANTITY "|UPDATE_PRODUCT_QUANTITY|"
#define UPDATE_PRODUCT_PRICE "|UPDATE_PRODUCT_PRICE|"
#define UPDATE_BILLING_INFO "|UPDATE_BILLING_INFO|"
#define ADD_ORDER "|ADD_ORDER|"
#define GET_SELLER_PRODUCTS "|GET_SELLER_PRODUCTS|"
#define GET_ALL_PRODUCTS "|GET_ALL_PRODUCTS|"
#define GET_PRODUCT_INFO "|GET_PRODUCT_INFO|"
#define GET_SELLER_ORDERS "|GET_SELLER_ORDERS|"
#define GET_BUY_ORDERS "|GET_BUY_ORDERS|"
#define GET_BILLING_INFO "|GET_BILLING_INFO|"

#endif //CS4323_FINALPROJECT_GROUPC_SERVERTODATASERVERAPI_H
