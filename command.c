#include "command.h"

void register_user(int sockfd) {
    char username[LINELEN];
    char password[LINELEN];
    bool valid = true;

    // read the username from stdin
    printf("username=");
    fgets(username, LINELEN, stdin);
    username[strlen(username) - 1] = '\0';

    // read the password from stdin
    printf("password=");
    fgets(password, LINELEN, stdin);
    password[strlen(password) - 1] = '\0';

    if (strstr(username, " ") != NULL) {
        printf("ERROR: Username cannot contain character whitespace (\" \").\n");
        return;
    }

    valid = validateInput(username);
    if (!valid) {
        return;
    }

    if (strstr(password, " ") != NULL) {
        printf("ERROR: Password cannot contain character whitespace (\" \").\n");
        return;
    }

    valid = validateInput(password);
    if (!valid) {
        return;
    }

    // JSON object
    JSON_Value *value = json_value_init_object();
    JSON_Object *object = json_value_get_object(value);

    json_object_set_string(object, "username", username);
    json_object_set_string(object, "password", password);

    char *json_object = json_serialize_to_string_pretty(value);

    // POST request
    char *post_request = compute_post_request(SERVER_IP, ACCESS_ROUTE_REGISTER, PAYLOAD, json_object, NULL, NULL);

    // send to server
    send_to_server(sockfd, post_request);

    // POST request response
    char *post_request_answer = receive_from_server(sockfd);

    // if the answer contains "error", it can be either from too many requests or
    // from the fact that the username has been already registered before
    if (strstr(post_request_answer, "error") != NULL) {
        if (strstr(post_request_answer, "taken") != NULL) {
            printf("ERROR: Username already exists.\n");
        }
    } else if (strstr(post_request_answer, "ok")) {
        printf("SUCCESS: Client has been successfully registered.\n");
    }
}

char *login_user(int sockfd) {
    // this is the cookie which will be returned
    char *cookie = NULL;
    bool valid = true;

    char username[LINELEN];
    char password[LINELEN];

    // read the username from stdin
    printf("username=");
    fgets(username, LINELEN, stdin);
    username[strlen(username) - 1] = '\0';

    // read the password from stdin
    printf("password=");
    fgets(password, LINELEN, stdin);
    password[strlen(password) - 1] = '\0';

    if (strstr(username, " ") != NULL) {
        printf("ERROR: No username contains whitespace (\" \").\n");
        return NULL;
    }

    valid = validateInput(username);
    if (!valid) {
        return NULL;
    }

    if (strstr(password, " ") != NULL) {
        printf("ERROR: No password contains whitespace (\" \").\n");
        return NULL;
    }

    valid = validateInput(password);
    if (!valid) {
        return NULL;
    }

    // JSON object
    JSON_Value *value = json_value_init_object();
    JSON_Object *object = json_value_get_object(value);

    json_object_set_string(object, "username", username);
    json_object_set_string(object, "password", password);

    char *json_object = json_serialize_to_string_pretty(value);

    // POST request
    char *post_request = compute_post_request(SERVER_IP, ACCESS_ROUTE_LOGIN, PAYLOAD, json_object, NULL, NULL);

    // send to server
    send_to_server(sockfd, post_request);

    // receive response
    char *post_request_answer = receive_from_server(sockfd);

    // if the answer contains "error", it can be either from too many requests or
    // from the fact that the username has been already registered before
    if (strstr(post_request_answer, "error") != NULL) {
        if (strstr(post_request_answer, "Credentials") != NULL) {
            printf("ERROR: Credentials do not match.\n");
        } else if (strstr(post_request_answer, "No account")) {
            printf("ERROR: Username does not exist.\n");
        }
    } else {
        printf("SUCCESS: Logged in successfully.\n");

        // extract the Set-Cookie field
        cookie = strstr(post_request_answer, "Set-Cookie");
        
        // extract the cookie itself without the header "Set-cookie:"
        cookie = strstr(cookie, " ");

        cookie = strtok(cookie, " ");

        cookie[strlen(cookie) - 1] = '\0';
    }

    return cookie;
}

void logout_user(int sockfd, char *cookie) {
    // GET request
    char *get_request = compute_get_request(SERVER_IP, ACCESS_ROUTE_LOGOUT, NULL, cookie, NULL);

    // send to server
    send_to_server(sockfd, get_request);

    // receive response
    char *get_request_answer = receive_from_server(sockfd);

    if (strstr(get_request_answer, "ok") != NULL) {
        printf("SUCCESS: Logged out successfully.\n");
    } else {
        printf("ERROR\n");
    }
}

char *enter_library(int sockfd, char *cookie) {
    // GET request
    char *get_request = compute_get_request(SERVER_IP, ACCESS_ROUTE_LIBRARY, NULL, cookie, NULL);

    // send to server
    send_to_server(sockfd, get_request);

    // receive response
    char *get_request_answer = receive_from_server(sockfd);

    if (strstr(get_request_answer, "error") == NULL) {
        printf("SUCCESS: Entered the library.\n");
    } else {
        printf("ERROR\n");
        return NULL;
    }

    // get the token
    char *token = strstr(get_request_answer, "token");
    token = strstr(token, ":");
    token = token + 2;
    token = strtok(token, "\"");

    return token;
}

void get_books(int sockfd, char *cookie, char *token) {
    // GET request
    char *get_request = compute_get_request(SERVER_IP, ACCESS_ROUTE_BOOKS, NULL, cookie, token);

    // send to server
    send_to_server(sockfd, get_request);

    // receive response
    char *get_request_answer = receive_from_server(sockfd);

    printf("%s\n", get_request_answer);

    // extract the books
    if (strstr(get_request_answer, "[{") != NULL) {
        get_request_answer = strstr(get_request_answer, "[{");
    } else {
        printf("[]\n");
    }
}

void add_book(int sockfd, char *cookie, char *token) {
    bool valid = false;

    // read the data for the book   
    char title[LINELEN]; 
    printf("title= ");
    fgets(title, LINELEN, stdin);
    title[strlen(title) - 1] = '\0';

    char author[LINELEN];
    printf("author= ");
    fgets(author, LINELEN, stdin);
    author[strlen(author) - 1] = '\0';

    char genre[LINELEN];
    printf("genre= ");
    fgets(genre, LINELEN, stdin);
    genre[strlen(genre) - 1] = '\0';

    char publisher[LINELEN];
    printf("publisher= ");
    fgets(publisher, LINELEN, stdin);
    publisher[strlen(publisher) - 1] = '\0';

    char page_count[LINELEN];
    printf("page_count= ");
    fgets(page_count, LINELEN, stdin);
    page_count[strlen(page_count) - 1] = '\0';

    // validates the inputs
    valid = validateInput(title);
    if (!valid) {
        return;
    }

    valid = validateInput(author);
    if (!valid) {
        return;
    }

    valid = validateInput(genre);
    if (!valid) {
        return;
    }

    valid = validateInput(publisher);
    if (!valid) {
        return;
    }

    valid = validateInput(page_count);
    if (!valid) {
        return;
    }

    // checks if the number starts with 0
    if (page_count[0] == '0') {
        printf("ERROR: Please enter a valid number.\n");
        return;
    }

    // checks if all characters are numbers
    for (int i = 0; i < strlen(page_count); i++) {
        if (page_count[i] > '9' || page_count[i] < '0') {
            printf("ERROR: Incorrect data type for page_count.\n");
            return;
        }
    }

    // convert the string to an integer
    int page_count_number = atoi(page_count);

    // JSON object
    JSON_Value *value = json_value_init_object();
    JSON_Object *object = json_value_get_object(value);

    json_object_set_string(object, "title", title);
    json_object_set_string(object, "author", author);
    json_object_set_string(object, "publisher", publisher);
    json_object_set_string(object, "genre", genre);
    json_object_set_number(object, "page_count", page_count_number);

    char *json_object = json_serialize_to_string_pretty(value);

    // POST request
    char *post_request = compute_post_request(SERVER_IP, ACCESS_ROUTE_BOOKS, PAYLOAD, json_object, cookie, token);

    // send to server
    send_to_server(sockfd, post_request);

    // POST request response
    char *post_request_answer = receive_from_server(sockfd);

    if (strstr(post_request_answer, "ok") != NULL) {
        printf("SUCCESS: The book has been added to the library.\n");
    } else {
        printf("ERROR\n");
    }
}

void get_book(int sockfd, char *cookie, char *token) {
    char route[50], id[BUFLEN];
    
    // reads the id from stdin
    printf("id=");
    fgets(id, BUFLEN, stdin);
    id[strlen(id) - 1] = '\0';

    for (int i = 0; i < strlen(id); i++) {
        if (id[i] > '9' || id[i] < '0') {
            printf("ERROR: The book id must be a number.\n");
            return;
        }
    }

    // form the route for the book
    strcpy(route, ACCESS_ROUTE_BOOK);
    strcat(route, id);

    // GET request
    char *get_request = compute_get_request(SERVER_IP, route, NULL, cookie, token);

    // send to server
    send_to_server(sockfd, get_request);

    // receive response
    char *get_request_answer = receive_from_server(sockfd);

    if (strstr(get_request_answer, "title") != NULL) {
        get_request_answer = strstr(get_request_answer, "{");
    } else {
        printf("ERROR: Book with id %s was not found.\n", id);
        return;
    }

    printf("%s\n", get_request_answer);
}

void delete_book(int sockfd, char *cookie, char *token) {
    char route[50], id[BUFLEN];
    
    // reads the id from stdin
    printf("id=");
    fgets(id, BUFLEN, stdin);
    id[strlen(id) - 1] = '\0';

    for (int i = 0; i < strlen(id); i++) {
        if (id[i] > '9' || id[i] < '0') {
            printf("ERROR: The book id must be a number.\n");
            return;
        }
    }

    // form the route for the book
    strcpy(route, ACCESS_ROUTE_BOOK);
    strcat(route, id);

    // GET request
    char *delete_request = compute_delete_request(SERVER_IP, route, NULL, cookie, token);

    // send to server
    send_to_server(sockfd, delete_request);

    // receive response
    char *delete_request_answer = receive_from_server(sockfd);

    if (strstr(delete_request_answer, "error") == NULL) {
        printf("SUCCESS: Book with id %s was deleted.\n", id);
    } else {
        printf("ERROR: Book with id %s was not found.\n", id);
        return;
    }
}