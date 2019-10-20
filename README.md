# Back-end Architecture

This is a peer to peer book lending service.

It has the following functions:

Book owners are able to register a list of shared books

The service (system) allows users to request any available books, and they must return them within 30 days.

Users are able to keep track of who has each book, and return deadlines and past sharing history.

It incoporates a search service that allows users to filter available books by name author, and year.

The system has a book ranking service that returns the top N books based on their history. Each book is calculated using their interactions with the user.

Users are evaluated based on the following interactions with the book: 
-Registering a book: +1 
-Lending a book: +5 
-Returning a book in good condition: +2 
-If the book is returned in bad condition: -10 
-Daily penalty for missing the deadline: -2
