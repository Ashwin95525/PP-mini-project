
#include <iostream>
#include <vector>
#include <string>

using namespace std;

enum Genre {
    FICTION,
    ADVENTURE,
    NON_FICTION,
    MYSTERY,
    SCIENCE_FICTION,
    FANTASY,
    BIOGRAPHY,
    MAGAZINE
};

string genreToString(Genre genre) {
    switch (genre) {
        case FICTION: return "Fiction";
        case ADVENTURE: return "Adventure";
        case NON_FICTION: return "Non-Fiction";
        case MYSTERY: return "Mystery";
        case SCIENCE_FICTION: return "Science Fiction";
        case FANTASY: return "Fantasy";
        case BIOGRAPHY: return "Biography";
        case MAGAZINE: return "Magazine";
        default: return "Unknown";
    }
}

class Item {
protected:
    string title;
    string author;

public:
    Item(string t, string a) : title(t), author(a) {}
    virtual void display() const = 0; // Pure virtual function
    virtual string getType() const = 0; // Pure virtual function
    string getTitle() const { return title; }
    virtual ~Item() {}
};

class Book : public Item {
private:
    string isbn;
    Genre genre;
    bool issued;

public:
    Book(string t, string a, string i, Genre g)
        : Item(t, a), isbn(i), genre(g), issued(false) {}

    void display() const override {
        cout << "Book - Title: " << title << ", Author: " << author
             << ", ISBN: " << isbn << ", Genre: " << genreToString(genre)
             << ", Status: " << (issued ? "Issued" : "Available") << endl;
    }

    string getType() const override {
        return "Book";
    }

    bool isIssued() const { return issued; }
    void issue() { issued = true; }
    void returnItem() { issued = false; }
};

class Magazine : public Item {
private:
    string issueNumber;
    bool issued;

public:
    Magazine(string t, string a, string issue)
        : Item(t, a), issueNumber(issue), issued(false) {}

    void display() const override {
        cout << "Magazine - Title: " << title << ", Author: " << author
             << ", Issue Number: " << issueNumber
             << ", Status: " << (issued ? "Issued" : "Available") << endl;
    }

    string getType() const override {
        return "Magazine";
    }

    bool isIssued() const { return issued; }
    void issue() { issued = true; }
    void returnItem() { issued = false; }
};

class DVD : public Item {
private:
    string duration; // Duration in minutes
    bool issued;

public:
    DVD(string t, string a, string d)
        : Item(t, a), duration(d), issued(false) {}

    void display() const override {
        cout << "DVD - Title: " << title << ", Author: " << author
             << ", Duration: " << duration
             << ", Status: " << (issued ? "Issued" : "Available") << endl;
    }

    string getType() const override {
        return "DVD";
    }

    bool isIssued() const { return issued; }
    void issue() { issued = true; }
    void returnItem() { issued = false; }
};

class User {
private:
    string name;
    string userId;
    vector<Item*> borrowedItems;

public:
    User(string n, string id) : name(n), userId(id) {}

    void borrowItem(Item* item) {
        borrowedItems.push_back(item);
    }

    void returnItem(const string& title) {
        for (auto it = borrowedItems.begin(); it != borrowedItems.end(); ++it) {
            if ((*it)->getTitle() == title) {
                borrowedItems.erase(it);
                cout << title << " returned successfully!" << endl;
                return;
            }
        }
        cout << "You haven't borrowed this item." << endl;
    }

    const vector<Item*>& getBorrowedItems() const {
        return borrowedItems;
    }

    string getUserId() const { return userId; }
    string getName() const { return name; }
};

class Library {
private:
    vector<Item*> items;
    vector<User*> users;

public:
    ~Library() {
        for (Item* item : items) {
            delete item;
        }
        for (User* user : users) {
            delete user;
        }
    }

    void addItem(Item* item) {
        items.push_back(item);
    }

    void addUser(User* user) {
        users.push_back(user);
    }

    User* getUser(const string& userId) {
        for (User* user : users) {
            if (user->getUserId() == userId) {
                return user;
            }
        }
        return nullptr; // Return nullptr if user not found
    }

    void displayItems() const {
        cout << "Library Items:" << endl;
        for (const Item* item : items) {
            item->display();
        }
    }

    void searchItem(const string& title) const {
        bool found = false;
        for (const Item* item : items) {
            if (title == item->getTitle()) {
                item->display();
                found = true;
            }
        }
        if (!found) {
            cout << "Item not found!" << endl;
        }
    }

    void issueItem(const string& title, User& user) {
        for (Item* item : items) {
            if (item->getTitle() == title) {
                if (Book* book = dynamic_cast<Book*>(item)) {
                    if (!book->isIssued()) {
                        book->issue();
                        user.borrowItem(item);
                        cout << "Book issued successfully to " << user.getUserId() << "!" << endl;
                    } else {
                        cout << "Book is already issued!" << endl;
                    }
                    return;
                } else if (Magazine* magazine = dynamic_cast<Magazine*>(item)) {
                    if (!magazine->isIssued()) {
                        magazine->issue();
                        user.borrowItem(item);
                        cout << "Magazine issued successfully to " << user.getUserId() << "!" << endl;
                    } else {
                        cout << "Magazine is already issued!" << endl;
                    }
                    return;
                } else if (DVD* dvd = dynamic_cast<DVD*>(item)) {
                    if (!dvd->isIssued()) {
                        dvd->issue();
                        user.borrowItem(item);
                        cout << "DVD issued successfully to " << user.getUserId() << "!" << endl;
                    } else {
                        cout << "DVD is already issued!" << endl;
                    }
                    return;
                }
            }
        }
        cout << "Item not found!" << endl; // Item not found message
    }

    void returnItem(const string& title, User& user) {
        for (Item* item : items) {
            if (item->getTitle() == title) {
                if (Book* book = dynamic_cast<Book*>(item)) {
                    if (book->isIssued()) {
                        book->returnItem();
                        user.returnItem(title);
                    } else {
                        cout << "Book was not issued!" << endl;
                    }
                    return;
                } else if (Magazine* magazine = dynamic_cast<Magazine*>(item)) {
                    if (magazine->isIssued()) {
                        magazine->returnItem();
                        user.returnItem(title);
                    } else {
                        cout << "Magazine was not issued!" << endl;
                    }
                    return;
                } else if (DVD* dvd = dynamic_cast<DVD*>(item)) {
                    if (dvd->isIssued()) {
                        dvd->returnItem();
                        user.returnItem(title);
                    } else {
                        cout << "DVD was not issued!" << endl;
                    }
                    return;
                }
            }
        }
        cout << "Item not found!" << endl;
    }

    void displayUsers() const {
        cout << "Library Users:" << endl;
        for (const User* user : users) {
            cout << "Name: " << user->getName() << ", User ID: " << user->getUserId() << endl;
        }
    }
};

void addBook(Library& library) {
    string title, author, isbn;
    int genreChoice;

    cout << "Enter title: ";
    cin.ignore();
    getline(cin, title);
    cout << "Enter author: ";
    getline(cin, author);
    cout << "Enter ISBN: ";
    getline(cin, isbn);

    cout << "Select Genre:\n";
    cout << "0. Fiction\n1. Adventure\n2. Non-Fiction\n3. Mystery\n4. Science Fiction\n5. Fantasy\n6. Biography\n";
    cin >> genreChoice;

    Genre genre = static_cast<Genre>(genreChoice);
    library.addItem(new Book(title, author, isbn, genre));
    cout << "Book added successfully!" << endl;
}

void addMagazine(Library& library) {
    string title, author, issueNumber;

    cout << "Enter title: ";
    cin.ignore();
    getline(cin, title);
    cout << "Enter author: ";
    getline(cin, author);
    cout << "Enter issue number: ";
    getline(cin, issueNumber);

    library.addItem(new Magazine(title, author, issueNumber));
    cout << "Magazine added successfully!" << endl;
}

void addDVD(Library& library) {
    string title, author, duration;

    cout << "Enter title: ";
    cin.ignore();
    getline(cin, title);
    cout << "Enter author: ";
    getline(cin, author);
    cout << "Enter duration (in minutes): ";
    getline(cin, duration);

    library.addItem(new DVD(title, author, duration));
    cout << "DVD added successfully!" << endl;
}

void addUser(Library& library) {
    string name, userId;

    cout << "Enter user name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter user ID: ";
    getline(cin, userId);

    library.addUser(new User(name, userId));
    cout << "User added successfully!" << endl;
}

int main() {
    Library library;

    int choice;
    do {
        cout << "\nLibrary Management System\n";
        cout << "1. Add Book\n";
        cout << "2. Add Magazine\n";
        cout << "3. Add DVD\n";
        cout << "4. Add User\n";
        cout << "5. Display Items\n";
        cout << "6. Display Users\n";
        cout << "7. Issue Item\n";
        cout << "8. Return Item\n";
        cout << "9. Search Item\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addBook(library); break;
            case 2: addMagazine(library); break;
            case 3: addDVD(library); break;
            case 4: addUser(library); break;
            case 5: library.displayItems(); break;
            case 6: library.displayUsers(); break;
            case 7: {
                string title, userId;
                cout << "Enter item title: ";
                cin.ignore();
                getline(cin, title);
                cout << "Enter user ID: ";
                getline(cin, userId);
                User* user = library.getUser(userId);
                if (user) {
                    library.issueItem(title, *user);
                } else {
                    cout << "User not found!" << endl;
                }
                break;
            }
            case 8: {
                string title, userId;
                cout << "Enter item title: ";
                cin.ignore();
                getline(cin, title);
                cout << "Enter user ID: ";
                getline(cin, userId);
                User* user = library.getUser(userId);
                if (user) {
                    library.returnItem(title, *user);
                } else {
                    cout << "User not found!" << endl;
                }
                break;
            }
            case 9: {
                string title;
                cout << "Enter item title to search: ";
                cin.ignore();
                getline(cin, title);
                library.searchItem(title);
                break;
            }
            case 0: cout << "Exiting program..." << endl; break;
            default: cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 0);

    return 0;
}
