#ifndef _COMMAND_
#define _COMMAND_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "helpers.h"
#include "parson.h"
#include "requests.h"

#define SERVER_IP             "34.246.184.49"
#define SERVER_PORT           8080

#define ACCESS_ROUTE_REGISTER "/api/v1/tema/auth/register"
#define ACCESS_ROUTE_LOGIN    "/api/v1/tema/auth/login"
#define ACCESS_ROUTE_LIBRARY  "/api/v1/tema/library/access"
#define ACCESS_ROUTE_BOOKS    "/api/v1/tema/library/books"
#define ACCESS_ROUTE_BOOK     "/api/v1/tema/library/books/"
#define ACCESS_ROUTE_LOGOUT   "/api/v1/tema/auth/logout"

#define PAYLOAD               "application/json"

// registers a user
void register_user(int sockfd);

// logs in a user
char *login_user(int sockfd);

// logs out a user
void logout_user(int sockfd, char *cookie);

// user accesses the library
char *enter_library(int sockfd, char *cookie);

// user wants to see the books information
void get_books(int sockfd, char *cookie, char *token);

// user wants to add a book to the library
void add_book(int sockfd, char *cookie, char *token);

// gets the details of a book with a certain id
void get_book(int sockfd, char *cookie, char *token);

// deletes a book from the user's list of books
void delete_book(int sockfd, char *cookie, char *token);

#endif