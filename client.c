#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include <stdbool.h>
#include "buffer.h"
#include "helpers.h"
#include "requests.h"
#include "command.h"

// USERNAME SI PAROLA CU WHITESPACE SEGFAULT LA REGISTER / LOGIN / ENTER LIBRARY IN CHECKER

bool logged_user;
bool entered;
bool ok_enter;
bool ok_login;

int main() {
    int sockfd;
    char *cookie = NULL;
    char *cookie_copy = NULL;
    char *token = NULL;
    char *token_copy = NULL;
    char *command = malloc(BUFLEN * sizeof(char));

    entered = false;
    logged_user = false;
    ok_enter = true;
    ok_login = true;

    while (1) {
        // reads the command from stdin
        fgets(command, BUFLEN, stdin);
        command[strlen(command) - 1] = '\0';

        // open_connection also handles the errors, besides setting up the conenction
        sockfd = open_connection(SERVER_IP, SERVER_PORT, AF_INET, SOCK_STREAM, 0);

        // treat the cases for each command found in the buffer
        if (strcmp(command, "register") == 0) {
            if (!logged_user) {
                // register the user
                register_user(sockfd);
            } else {
                printf("ERROR: Account cannot be created because you are already logged in.\n");
            }
            close(sockfd);
        } else if (strcmp(command, "login") == 0) {
            if (!logged_user) {
                cookie = login_user(sockfd);
                if (cookie != NULL) {
                    logged_user = true;
                }
                if (ok_login && cookie != NULL) {
                    cookie_copy = malloc(sizeof(char) * (strlen(cookie) + 1));
                    memcpy(cookie_copy, cookie, strlen(cookie));
                    ok_login = false;
                }
            } else {
                printf("ERROR: You are already logged in. You need to log out first.\n");
            }
            close(sockfd);
        } else if (strcmp(command, "enter_library") == 0) {
            if (!logged_user) {
                printf("ERROR: You have to log in first in order to enter the library.\n");
            } else if (entered) {
                printf("ERROR: You already entered the library.\n");
            } else {
                // returns the session token
                token = enter_library(sockfd, cookie);
                if (ok_enter && token != NULL) {
                    token_copy = malloc(sizeof(char) * (strlen(token) + 1));
                    memcpy(token_copy, token, strlen(token));
                    ok_enter = false;
                }
                if (token != NULL) {
                    entered = true;
                }
            }
            close(sockfd);
        } else if (strcmp(command, "get_books") == 0) {
            if (!logged_user) {
                printf("ERROR: You have to log in first in order to use the command get_books.\n");
            } else {
                if (token == NULL) {
                    printf("ERROR: You do not have access to the library in order to use this command.\n");
                } else {
                    strcpy(token, token_copy);
                    strcpy(cookie, cookie_copy);
                    get_books(sockfd, cookie, token);
                }
            }
            close(sockfd);
        } else if (strcmp(command, "get_book") == 0) {
            if (!logged_user) {
                printf("ERROR: You have to log in first in order to use the command get_book.\n");
            } else {
                if (token == NULL) {
                    printf("ERROR: You do not have access to the library in order to use this command.\n");
                } else {
                    strcpy(token, token_copy);
                    strcpy(cookie, cookie_copy);
                    get_book(sockfd, cookie, token);
                }
            }
        } else if (strcmp(command, "add_book") == 0) {
            if (!logged_user) { 
                printf("ERROR: You have to log in first in order to use the command add_book.\n");
            } else {
                if (token == NULL) {
                    printf("ERROR: You do not have access to the library in order to use this command.\n");
                } else {
                    strcpy(token, token_copy);
                    strcpy(cookie, cookie_copy);
                    add_book(sockfd, cookie, token);
                }
            }
            close(sockfd);
        } else if (strcmp(command, "delete_book") == 0) {
            if (!logged_user) { 
                printf("ERROR: You have to log in first in order to use the command delete_book.\n");
            } else {
                if (token == NULL) {
                    printf("ERROR: You do not have access to the library in order to use this command.\n");
                } else {
                    strcpy(token, token_copy);
                    strcpy(cookie, cookie_copy);
                    delete_book(sockfd, cookie, token);
                }
            }
            close(sockfd);
        } else if (strcmp(command, "logout") == 0) {
            if (!logged_user) {
                printf("ERROR: You have to log in first in order to log out.\n");
            } else {
                logout_user(sockfd, cookie);

                // set logged_user back to false for future a log in
                logged_user = false;

                // set entered back to false for future enter_library
                entered = false;

                ok_enter = true;
                ok_login = true;
            }
            close(sockfd);
        } else if (strcmp(command, "exit") == 0) {
            // close the socket and exit the program
            close(sockfd);
            free(command);
            return 0;
        } else {
            printf("ERROR: Invalid command. Please enter a valid command.\n");
        }

    }

    return 0;
}