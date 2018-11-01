/*
Create the backend architecture for a peer-to-peer book lending service.

LEVEL 1: Book owners must be able to register a list of shared books.

LEVEL 2: Help users to keep track of who has each book, return deadlines, and past sharing history.

LEVEL 3: The system will allow users to request any available book. They must return them within a 30-days period.

LEVEL 4: We need to provide a mechanism to calculate a user score. Users will be evaluated based on their actions:
  a. Registering a book: +1
  b. Lending a book: +5
  c. Returning a book in good condition: +2
  d. If the book is returned in bad condition: -10
  e. Daily penalty for missing the deadline: -2

LEVEL 5: Create a search service, allowing users to filter available books by name, author, or year.

LEVEL 6: Provide a book ranking service, that returns the top N books based on their history. Each book score is calculated using their interactions with users.
*/

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <ctime>
#include <cmath>

using namespace std;

// Class for registered user
class Reg_user{
    string name;
    int score;
    
public:
    Reg_user(string n, int s){
        name = n;
        score = s;
    }
    void setName(string s){
        name = s;
    }
    void addScore(int s){
        score += s;
    }
    int getScore(){
        return score;
    }
    string getName(){
        return name;
    }
};

// Class for book
class Books{
    string name;
    string writer;
    string year;
    string owner;
    string holder;
    bool IngoodCondition;
    time_t returnDeadline;
    vector <string> history; //  stores the history of past users 
    
public:
    Book(string n, string w, string o, string y){
        name = n;
        writer = w;
        owner = o;
        year = y;
        IngoodCondition = true;
        
    }
    void setReturnDeadline(){
        returnDeadline = time(0) + 2592000; // after 30 days 
    }
    time_t getReturnDeadline(){
        return returnDeadline;
    }
    void setHolder(string h){
        holder = h;
    }
    void setCondition(bool c){
        IngoodCondition = c;
    }
    void addToHistory(string p){
        history.push_back(p);
    }
    string getName(){
        return name;
    }
    string getWriter(){
        return writer;
    }
    string getOwner(){
        return owner;
    }
    string getHolder(){
        return holder;
    }
    string getYear(){
        return year;
    }
    vector <string> getHistory(){
        return history;
    }
    bool isGood(){
        return IngoodCondition;
    }
    bool isAvalaible(){
        return (holder == "");
    }
};


//checking if a person is registered 
int searchPerson(string name, vector<User> users){
    for (int i = 0; i < users.size(); i++){
        if (name == users[i].getName()) return i;
    }
    return -1;
}

// checking if book was registered
int searchBook(string name, string author, string year, vector<Book> books){
    for (int i = 0; i < books.size(); i++){
        if (name == books[i].getName() && author == books[i].getWriter() && year == books[i].getYear())
            return i;
    }
    return -1;
}

// registering a book
void registerBook(string registrant, string bookName, string author, string year, vector <User>& users, vector <Book>& books){
    Book tempBook(bookName, author, registrant, year);
    books.push_back(tempBook);
    
    int index = searchPerson(registrant, users);
    
    if (index != -1){// if person is already registered 
        users[index].addScore(1);
    }
    else{
        User temp(registrant, 1);
        users.push_back(temp);
    }
}

// requesting to  a book
void requestBook(string borrower, string bookName, string author, string year, vector<User>& users, vector <Book>& books){
    int i = searchBook(bookName, author, year, books);
    if (i == -1)
        cout << bookName << " by writer, " << author << " is not a registered\n" << endl;
    else if (!books[i].isAvalaible()){
        cout << bookName << " by " << author << ", published in the year " << year << "is currently with " << books[i].getHolder() << endl;
        time_t deadline = books[i].getReturnDeadline();
        tm *timePtr = localtime(&deadline);
        cout << "The book will be back in stock on date: " << (timePtr->tm_mon)+1 << "/" << (timePtr->tm_mday) <<
        "/" << (timePtr->tm_year)+1900 << "\n" << endl;
    }
    else{
        books[i].setHolder(borrower);
        string lender = books[i].getOwner();
        int j = searchPerson(lender,users);
        users[j].addScore(5);
        books[i].setReturnDeadline();
        time_t deadline = books[i].getReturnDeadline();
        tm *timePtr = localtime(&deadline);
        cout << "Return book by " << (timePtr->tm_mon)+1 << "/" << (timePtr->tm_mday) << "/" << (timePtr->tm_year)+1900 << endl;
        
        // sign-up borrower, if not registered
        int k = searchPerson(borrower,users);
        if (k == -1){
            User temp(borrower, 0);
            users.push_back(temp);
        }
    }
}

// returning a book
void returnBook(string returner, string bookName, string author, string year, bool goodCondition, vector<User>& users, vector <Book>& books){
    int i = searchBook(bookName, author, year, books);
    books[i].setHolder("");
    
    books[i].addToHistory(returner);
    
    int j = searchPerson(returner, users);
    
    // checking condition of book
    if(IngoodCondition){
        users[j].addScore(2);
    }
    else{
        users[j].addScore(-10);
    }    
    
    // losing points for overdue
    time_t now = time(0)/60/60/24;
    time_t deadline = books[i].getReturnDeadline()/60/60/24;
    if(now > deadline){
        users[j].addScore(-2 * (int)(now - deadline));
    }
}

// get book's previous users
void printHistory(string bookName, string author, string year, vector<Book> books){
    int i = searchBook(bookName, author, year, books);
    if (i == -1)
        cout << bookName << " by " << author << " published in " << year << " is not a registered book. " << endl;
    else{
        vector <string> history = books[i].getHistory();
        if (history.empty())
            cout << bookName << " by " << author << " published in " << year << " has no previous user" << endl;
        else {
            for (int i = 0; i < history.size(); i++){
                cout << history[i] << endl;
            }
        }
    }
}

// to get scores of registered users
void printScores(vector <User> users){
    for (int i = 0; i < users.size(); i++){
        cout << users[i].getName() << " ==> " << users[i].getScore() << endl;
    }

// searching for book using title
vector <Book> searchBookByName(string title, vector<Book> listofBooks){
    vector<Book> list;
    for (int i = 0; i < listofBooks.size(); i++){
        if (listofBooks[i].isAvalaible() && listofBooks[i].getName() == title){
            list.push_back(listofBooks[i]);
        }
    }
    return list;
}

// searching for book using writer
vector <Book> searchBookByWriter(string writer, vector<Book> listofBooks){
    vector<Book> list;
    for (int i = 0; i < listofBooks.size(); i++){
        if (listofBooks[i].isAvalaible() && listofBooks[i].getWriter() == writer){
            list.push_back(listofBooks[i]);
        }
    }
    return list;
}

// searching for book using year
vector <Book> searchBookByYear(string year, vector<Book> listofBooks){
    vector<Book> list;
    for (int i = 0; i < listofBooks.size(); i++){
        if (listofBooks[i].isAvalaible() && listofBooks[i].getYear() == year){
            list.push_back(listofBooks[i]);
        }
    }
    return list;
}