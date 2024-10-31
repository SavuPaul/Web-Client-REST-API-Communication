# COMMUNICATION PROTOCOLS PROJECT

**Note:** The JSON library is the one mentioned in the task, from which I only used the functions to create a JSON object and to transform a JSON object into a string. In the files `requests.c` and `requests.h`, I added the `compute_delete_request` function, which is used for the `delete_book` function.

The entire flow of the program runs in an infinite `while` loop, which ends when the user inputs the command "exit" from the keyboard. To minimize connection time to the server, the socket for the connection to the server is opened and closed in each iteration of the `while` loop (there's no point in staying connected if the user is not active).

## FUNCTION DESCRIPTIONS

### REGISTER FUNCTION (POST REQUEST)

**Errors:**
- The username already exists.
- The username/password contains a space character.
- The username/password is empty (i.e., "").

The username and password are read from the keyboard, and if they do not contain spaces, they are added to a JSON object, which is then sent via a POST request to the server. The server's response may indicate that the user already exists, displaying the corresponding error. Otherwise, the user is created successfully.

### LOGIN FUNCTION (POST REQUEST)

**Errors:**
- The user is already logged in.
- The username/password do not match.
- The username does not exist.
- The username/password contains a space character.
- The username/password is empty (i.e., "").

For this function, I created a global variable `logged_user` which becomes `true` when a user successfully logs in. This allows access to other functionalities for clients. In the `login_user` function, the user's credentials are read. First, it checks if they contain a space character. If not, they are entered into a JSON variable and sent to the server. Upon receiving a response from the server, the corresponding error messages are displayed. If there are no errors, a confirmation message for login is displayed, and then the cookie from the message sent by the server is extracted. This cookie must be retained throughout the login session to prove that the user is authenticated.

### ENTER_LIBRARY FUNCTION (GET REQUEST)

**Errors:**
- The user is not authenticated.
- The user is already in the library.

For this function, I created a global variable `entered`, which becomes `true` when a user enters the library. This allows access to library-specific functions. After sending the GET request, the user receives a token that provides an additional layer of security. This token is extracted from the message received from the server and used by the user throughout the authentication session.

### ADD_BOOK FUNCTION (POST REQUEST)

**Errors:**
- The user is not authenticated.
- The user is not in the library.
- One of the book fields is empty (i.e., "").
- The `page_count` field does not contain numeric characters (or starts with 0).

First, all the fields for the book are read from the keyboard, which may contain spaces (except for `page_count`, where it is checked that all characters are numeric and the first digit is not 0). If all fields are valid, they are added to a JSON object, which is then sent to the server via a POST request. If the book was successfully added (i.e., the message returned from the server contains "ok"), a confirmation message for adding the book to the database is displayed.

### GET_BOOKS FUNCTION (GET REQUEST)

**Errors:**
- The user is not authenticated.
- The user is not in the library.

A GET request is sent to the server, which returns a response containing the books in the database in JSON format, displayed in the output.

### GET_BOOK FUNCTION (GET REQUEST)

**Errors:**
- The user is not authenticated.
- The user is not in the library.
- The ID provided by the user contains non-numeric characters.
- The ID provided by the user does not exist in their list of books.

The ID for the book whose details need to be displayed is read from the keyboard. If the characters are numeric, the access route to the book is formed by concatenating `"/api/v1/tema/library/book/"` with the ID read from the keyboard. A GET request is sent, and if the response contains the string "title" (indicating that the message contains the book details), the book details are displayed. Otherwise, the error message "Book with id <ID> was not found" is displayed.

### DELETE_BOOK FUNCTION (DELETE REQUEST)

**Errors:**
- The user is not authenticated.
- The user is not in the library.
- The ID provided by the user contains non-numeric characters.
- The ID provided by the user does not exist in their list of books.

Similar to the `get_book` function, the ID is validated, the route is formed, and a DELETE request is sent to the server. If the message returned from the server contains the string "ok", then the deletion was successful (the corresponding message is displayed). Otherwise, it indicates that we encountered the error "Book with id <ID> was not found".

### LOGOUT FUNCTION (GET REQUEST)

**Errors:**
- The user is not authenticated.

A GET request is sent to the server, thus finalizing the authentication session. Consequently, the variables for `logged_user` and `entered` are reset, so that in a future login, the processes described in the login/enter_library functions can be resumed.

In the `main` function, I used a copy for the token and for the cookie to ensure that they are maintained throughout the duration of the authentication session. Before calling each function, the `token_copy` is copied into the original token. Additionally, if the user enters a command that does not exist, an error is displayed.
