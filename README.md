TEMA 4 - PROTOCOALE DE COMUNICATIE

!! AM FOLOSIT SCHELETUL LABORATORULUI 9 !!
Biblioteca JSON este cea din enuntul temei, din care am folosit doar functiile de creare a unui obiect JSON si cea de transformare
a unui obiect JSON intr-un string. In fisierul requests.c si requests.h am adaugat functia compute_delete_request care este folosita
pentru functia delete_book.


Intregul flow al programului se desfasoara intr-un ciclu while infinit, care se termina in momentul in care utilizatorul introduce 
de la tastatura comanda "exit". Pentru a minimiza timpul de conectare la server, la fiecare iteratie prin ciclul while am deschis
si inchis socket-ul pentru conexiunea la server (nu are sens sa stam conectati daca user-ul nu este activ).

FUNCTIA DE REGISTER (POST REQUEST):
Erori: - username-ul deja exista
       - username-ul / parola contin caracterul spatiu
       - username-ul / parola nu contin nimic (sunt "")
Se citesc de la tastatura username-ul si parola, si daca acestea nu contin spatiu, sunt introduse intr-un obiect de tip JSON, care
este trimis mai departe printr-un post-request catre server. Raspunsul de la server poate contine faptul ca user-ul deja exista,
afisandu-se eroarea afereta. Altfel, utilizatorul este creat cu succes.

FUNCTIA DE LOGIN (POST REQUEST):
Erori: - user-ul este deja logged in
       - username-ul / parola nu se potrivesc
       - username-ul nu exista
       - username-ul / parola contin spatiu
       - username-ul / parola nu contin nimic (sunt "")
Pentru aceasta functie, am creat variabila globala logged_user care devine true in momentul in care un user reuseste sa dea log in.
Astfel, se permite accesul si la alte functionalitati pentru clienti. In functia de login_user, se citesc credentialele user-ului.
Mai intai se verifica daca acestea contin caracterul spatiu. Daca nu, acestea sunt introduse intr-o variabila in format JSON si
sunt trimise catre server. In urma raspunsului de la server, se afiseaza mesajele de eroare corespunzatoare enumerate mai sus. Daca
nu apare nicio eroare, atunci se afiseaza mesajul de confirmare a login-ului, dupa care se extrage cookie-ul din mesajul trimis de
server. Acest cookie trebuie pastrat pe toata durata sesiunii de login pentru a dovedi ca utilizatorul este autentificat.

FUNCTIA DE ENTER_LIBRARY (GET REQUEST):
Erori: - user-ul nu este autentificat
       - user-ul este deja intrat in biblioteca
Pentru aceasta functie, am creat variabila globala entered care devine true in momentul in care un user intra in biblioteca. Astfel,
acesta va avea acces si la functiile specifice bibliotecii. Dupa trimiterea GET-requestului, utilizatorul va primi un token care
asigura o masura in plus de securitate. Acesta este extras din mesajul primit de la server si folosit mai departe de catre utilizator
in sesiunea de autentificare.

FUNCTIA DE ADD_BOOK (POST REQUEST):
Erori: - user-ul nu este autentificat
       - user-ul nu este intrat in biblioteca
       - unul dintre campurile cartii sunt "" (nu contine nimic)
       - campul page_count nu contine caractere de tip numeric (sau incepe cu 0)
Mai intai se citesc de la tastatura toate campurile pentru carte, acestea putand sa contina spatiu (mai putin page_count unde se
verifica ca toate caracterele sunt numerice si prima cifra este diferita de 0). Daca toate campurile sunt valide, atunci acestea
sunt introduse intr-un obiect de tip JSON, care este trimis mai departe catre server printr-un POST request. Daca cartea a fost
adaugata cu succes (adica mesajul inapoiat de server contine "ok"), atunci se afiseaza mesjul aferent de confirmare a adaugarii
cartii in baza de date.

FUNCTIA DE GET_BOOKS (GET REQUEST):
Erori: - user-ul nu este autentificat
       - user-ul nu este intrat in biblioteca
Se trimite un GET request catre server, iar acesta inapoiaza un raspuns in care se gasesc cartile din baza de date in format
JSON, care sunt afisate la output.

FUNCTIA DE GET_BOOK (GET REQUEST):
Erori: - user-ul nu este autentificat
       - user-ul nu este intrat in biblioteca
       - id-ul furnizat de catre user contine caractere care nu sunt numerice
       - id-ul furnizat de catre user nu exista in lista acestuia de carti
Se citeste de la tastatura id-ul pentru cartea a caror detalii trebuie afisate. Daca caracterele sunt numerice, atunci se formeaza
ruta de acces catre carte, concatenandu-se la "/api/v1/tema/library/book/", id-ul citit de la tastatura. Ulterior, se trimite un
GET request, iar daca raspunsul contine string-ul "title" (adica macar un cuvant care indica faptul ca mesajul contine detaliile
cartii), atunci afisam detaliile cartii. Altfel, se afiseaza mesajul de eroare "Book with id <ID> was not found".

FUNCTIA DE DELETE_BOOK (DELETE REQUEST):
Erori: - user-ul nu este autentificat
       - user-ul nu este intrat in biblioteca
       - id-ul furnizat de catre user contine caractere care nu sunt numerice
       - id-ul furnizat de catre user nu exista in lista acestuia de carti
La fel ca la functia, get_book, se valideaza id-ul, se formeaza ruta si se trimite un DELETE request catre server. Daca mesajul
inapoiat de server contine string-ul "ok", atunci inseamna ca stergerea a fost efectuata cu succes (se afiseaza mesajul aferent).
Altfel, inseamna ca am intampinat eroarea "Book with id <ID> was not found".

FUNCTIA DE LOGOUT (GET REQUEST)
Erori: - user-ul nu este autentificat
Se trimite GET request catre server, astfel finalizandu-se sesiunea de autentificare. Drept urmare, resetam variabilele pentru
logged_user si entered, pentru ca la un viitor eventual login, procesul descris in functiile de login / enter_library sa fie
reluat.

In functia main, am folosit o copie pentru token si pentru cookie pentru a ma asigura ca acestea se mentin pe toata durata sesiunii
de autentificare. Inainte de apelul fiecarei functii, token_copy-ul este cooiat in token-ul original.
In plus, daca user-ul introduce alta comanda decat cele existente, se afiseaza eroare.