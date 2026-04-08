# Electronic Library Management System

A desktop Library Management System built with **C++17**, featuring a graphical user interface powered by **Dear ImGui** (DirectX 9 backend on Windows). The application demonstrates core Object-Oriented Programming principles and implements three well-known software design patterns: **Observer**, **Strategy**, and **Decorator**.

---

## Table of Contents

- [Features](#features)
- [Project Structure](#project-structure)
- [Class Design](#class-design)
  - [Core Classes](#a-core-classes-core)
  - [Design Patterns](#b-design-patterns-patterns)
  - [Service Classes](#c-service-classes-services)
  - [Utility Classes](#d-utility-classes-utils)
- [Application Flow](#application-flow)
- [Building & Running](#building--running)
- [Important Notes](#important-notes)

---

## Features

- **User Authentication** — Register and log in as a Member or Librarian with CSV-backed credential storage.
- **Book Management** — Add, remove, update, and search books in the library catalog.
- **Borrowing & Returning** — Members can borrow available books and return them; the system tracks copies and availability.
- **Loan Tracking** — View all active, returned, and overdue loans with automatic overdue detection.
- **Flexible Search** — Search books by title, author, or category using interchangeable search strategies (Strategy Pattern).
- **Overdue Penalties** — Automatically apply tiered penalties (warning, fine, account suspension) based on days overdue (Strategy Pattern).
- **Real-time Notifications** — Observers are notified of book status changes and approaching/overdue loan due dates (Observer Pattern).
- **Extended Book Descriptions** — Dynamically layer additional information (difficulty labels, special tags, extra details) onto books without modifying the base class (Decorator Pattern).
- **Background Loan Monitoring** — A dedicated timer thread periodically scans loans for overdue items and applies penalties automatically.
- **Notification Logging** — Observer notifications are logged to the `librarylog/` directory for audit purposes.
- **GUI** — Full graphical interface with screens for login, registration, main menu, book search, borrowing, member management, loan listing, book addition, and notifications.

---

## Project Structure

```
LibraryManagementSystem/
├── data/                       # CSV data files (books, authors, categories, members, librarians, loans)
├── include/                    # Header files (.hpp)
│   ├── core/                   # Core domain classes
│   │   ├── Author.hpp
│   │   ├── Book.hpp
│   │   ├── Category.hpp
│   │   ├── Librarian.hpp
│   │   ├── Loan.hpp
│   │   ├── Member.hpp
│   │   └── User.hpp
│   ├── patterns/               # Design pattern interfaces and implementations
│   │   ├── decorator/          # Decorator Pattern
│   │   │   ├── BookDecorator.hpp
│   │   │   ├── DifficultyLabelDecorator.hpp
│   │   │   ├── SpecialTagDecorator.hpp
│   │   │   └── AdditionalInfoDecorator.hpp
│   │   ├── observer/           # Observer Pattern
│   │   │   ├── IObserver.hpp
│   │   │   ├── ISubject.hpp
│   │   │   ├── BookSubject.hpp
│   │   │   ├── LoanSubject.hpp
│   │   │   ├── MemberObserver.hpp
│   │   │   ├── LibrarianObserver.hpp
│   │   │   └── NotificationService.hpp
│   │   └── strategy/           # Strategy Pattern
│   │       ├── ISearchStrategy.hpp
│   │       ├── TitleSearchStrategy.hpp
│   │       ├── AuthorSearchStrategy.hpp
│   │       ├── CategorySearchStrategy.hpp
│   │       ├── IPenaltyStrategy.hpp
│   │       ├── FinePenaltyStrategy.hpp
│   │       ├── SuspendPenaltyStrategy.hpp
│   │       └── WarningPenaltyStrategy.hpp
│   ├── services/               # Business logic / service layer
│   │   ├── AuthenticateManager.hpp
│   │   └── LibraryManager.hpp
│   └── utils/                  # Utility / helper classes
│       ├── CSVHandler.hpp
│       └── StringHandler.hpp
├── src/                        # Source files (.cpp) — mirrors include/ structure
│   ├── core/
│   ├── patterns/
│   │   ├── decorator/
│   │   ├── observer/
│   │   └── strategy/
│   ├── services/
│   └── utils/
├── MainUI/                     # ImGui screen implementations
│   ├── AppState.hpp            # Application state enum
│   ├── Context.hpp             # Shared UI context
│   ├── Router.hpp              # Screen routing logic
│   ├── Screens.hpp             # Screen function declarations
│   ├── LoginUI.cpp
│   ├── RegisterUI.cpp
│   ├── MainMenuUI.cpp
│   ├── SearchBookUI.cpp
│   ├── BorrowBook.cpp
│   ├── AddBookUI.cpp
│   ├── ListMemberUI.cpp
│   ├── ListLoanUI.cpp
│   ├── MemberManageUI.cpp
│   └── NotificationUI.cpp
├── ui/                         # Dear ImGui library source & backends (DirectX 9, Win32)
├── UML/                        # UML diagrams (placeholder)
├── docs/                       # Documentation, report, demo script
├── librarylog/                 # Observer notification log files
├── main.cpp                    # Application entry point
├── CMakeLists.txt              # CMake build configuration
├── build.bat                   # Windows build script (MinGW)
├── requirements.md             # Course project requirements
├── imgui.ini                   # ImGui layout persistence
├── .gitignore
└── README.md                   # This file
```

---

## Class Design

### A. Core Classes (`core/`)

#### 1. `User` (Base class)

| Attribute   | Type     | Description                          |
|-------------|----------|--------------------------------------|
| `userID`    | `string` | Unique user identifier               |
| `userName`  | `string` | Display name                         |
| `password`  | `string` | User password                        |
| `role`      | `Role`   | Enum — `LIBRARIAN` or `MEMBER`       |

| Method          | Return   | Description                                  |
|-----------------|----------|----------------------------------------------|
| `getUserID()`   | `string` | Returns the user's ID                        |
| `getUserName()` | `string` | Returns the user's display name              |
| `getRole()`     | `Role`   | Returns the user's role                      |
| `viewProfile()` | `void`   | Prints the user's profile information        |

#### 2. `Member : public User`

| Attribute          | Type              | Description                               |
|--------------------|-------------------|-------------------------------------------|
| `membershipDate`   | `Date`            | Date the member joined                    |
| `borrowedBooks`    | `vector<Book*>`   | List of currently borrowed books          |
| `warningCount`     | `int`             | Number of penalty warnings received       |
| `suspended`        | `bool`            | Whether the account is currently suspended|
| `suspensionEndDate`| `time_t`          | When the suspension expires               |
| `totalFines`       | `double`          | Accumulated unpaid fines                  |

| Method                | Return          | Description                                        |
|-----------------------|-----------------|----------------------------------------------------|
| `borrowBook(ISBN)`    | `bool`          | Attempts to borrow a book; returns success status  |
| `returnBook(ISBN)`    | `bool`          | Returns a borrowed book; returns success status    |
| `getBorrowedBooks()`  | `vector<Book*>` | Lists all books currently borrowed by this member  |
| `addWarning()`        | `void`          | Increments the warning counter                     |
| `suspendAccount(days)`| `void`          | Suspends borrowing privileges for N days           |
| `isSuspended()`       | `bool`          | Checks whether the member is currently suspended   |
| `addFine(amount)`     | `void`          | Adds a monetary fine to the member's account       |
| `payFine(amount)`     | `void`          | Pays off part or all of the outstanding fine       |

#### 3. `Librarian : public User`

| Method                     | Return          | Description                                          |
|----------------------------|-----------------|------------------------------------------------------|
| `addBook(book)`            | `void`          | Adds a new book to the library catalog               |
| `removeBook(ISBN)`         | `void`          | Removes a book by ISBN                               |
| `updateBookInfo(book)`     | `void`          | Updates an existing book's information               |
| `manageMemberInfo(member)` | `void`          | Manages (view/edit) a member's information           |
| `viewAllLoans()`           | `vector<Loan*>` | Retrieves a list of all loan records                 |

#### 4. `Book` (Base class for Decorator)

| Attribute        | Type         | Description                                          |
|------------------|--------------|------------------------------------------------------|
| `ISBN`           | `string`     | Unique book identifier                               |
| `title`          | `string`     | Book title                                           |
| `author`         | `Author`     | Author object (ID, name, biography)                  |
| `category`       | `Category`   | Category object (ID, name, description)              |
| `status`         | `BookStatus` | Enum — `AVAILABLE` or `UNAVAILABLE`                  |
| `totalCopies`    | `int`        | Total number of copies owned                         |
| `availableCopies`| `int`        | Number of copies currently available for borrowing   |

| Method                 | Return   | Description                                             |
|------------------------|----------|---------------------------------------------------------|
| `getFullDescription()` | `string` | Returns a CSV-formatted full description (virtual, overridden by decorators) |
| `displayBasicInfo()`   | `void`   | Prints basic book info to the terminal                  |
| `isAvailable()`        | `bool`   | Returns `true` if `availableCopies > 0`                 |

#### 5. `Author`

| Attribute   | Type     | Description                |
|-------------|----------|----------------------------|
| `authorID`  | `string` | Unique author identifier   |
| `authorName`| `string` | Author's full name         |
| `biography` | `string` | Short biography            |

#### 6. `Category`

| Attribute    | Type     | Description                  |
|--------------|----------|------------------------------|
| `categoryID` | `string` | Unique category identifier   |
| `name`       | `string` | Category name                |
| `description`| `string` | Category description         |

#### 7. `Loan`

| Attribute    | Type         | Description                                     |
|--------------|--------------|-------------------------------------------------|
| `loanID`     | `string`     | Unique loan identifier                          |
| `bookISBN`   | `string`     | ISBN of the borrowed book                       |
| `memberID`   | `string`     | ID of the borrowing member                      |
| `borrowDate` | `Date`       | Date the book was borrowed                      |
| `dueDate`    | `Date`       | Date the book is due for return                 |
| `returnDate` | `Date`       | Actual return date (empty if not yet returned)  |
| `status`     | `LoanStatus` | Enum — `ACTIVE`, `RETURNED`, or `OVERDUE`       |

| Method             | Return   | Description                                                  |
|--------------------|----------|--------------------------------------------------------------|
| `isOverdue()`      | `bool`   | Checks whether the loan has passed its due date              |
| `getDaysOverdue()` | `int`    | Returns the number of days the loan is overdue               |
| `calculateFine()`  | `int`    | Calculates the fine based on overdue duration                |
| `loanCSVFormat()`  | `string` | Serializes the loan to CSV format for persistence            |
| `dateToString()`   | `string` | Static — converts a `Date` to a string                       |
| `stringToDate()`   | `Date`   | Static — parses a string into a `Date`                       |

---

### B. Design Patterns (`patterns/`)

#### 1. Observer Pattern (`patterns/observer/`)

Used to notify members and librarians about book status changes and loan due-date events.

**`IObserver` (Interface)**

| Method                                    | Description                                                       |
|-------------------------------------------|-------------------------------------------------------------------|
| `update(message, book, loan)`             | Pure virtual — called when an observed subject's state changes    |
| `logNotification(message, book, loan)`    | Pure virtual — logs the notification to a persistent log file     |

**`ISubject` (Interface)**

| Method              | Description                                              |
|---------------------|----------------------------------------------------------|
| `attach(observer)`  | Registers an observer to this subject                    |
| `detach(observer)`  | Removes an observer from this subject                    |
| `notify()`          | Notifies all attached observers of a state change        |

**`BookSubject : public Book, public ISubject`**
- Wraps a `Book` and manages a list of observers.
- Calls `notify()` when the book is borrowed, returned, reserved, or its status changes.

**`LoanSubject : public Loan, public ISubject`**
- Wraps a `Loan` and manages a list of observers.
- Calls `notify()` when the loan is approaching its due date or becomes overdue.

**`MemberObserver : public IObserver`**
- Receives notifications relevant to a member (e.g., due-date reminders, book availability).
- Logs notifications to `librarylog/member_notifications.log`.

**`LibrarianObserver : public IObserver`**
- Receives notifications relevant to a librarian (e.g., overdue alerts, system-level events).
- Logs notifications to `librarylog/librarian_notifications.log`.

**`NotificationService`**
- Orchestrates notification delivery across all book and loan subjects.
- Methods: `sendDueSoonNotifications()`, `sendReservationAvailableNotifications()`, `checkAndNotifyDueLoans()`.

---

#### 2. Strategy Pattern (`patterns/strategy/`)

Used for **book searching** and **overdue penalty handling**, allowing the algorithm to be swapped at runtime.

##### Book Search Strategies

**`ISearchStrategy` (Interface)**

| Method                        | Description                                                          |
|-------------------------------|----------------------------------------------------------------------|
| `search(books, query)`        | Pure virtual — searches a book collection and returns matching books |

| Concrete Strategy             | Description                                      |
|-------------------------------|--------------------------------------------------|
| `TitleSearchStrategy`         | Searches books by title (fuzzy matching)         |
| `AuthorSearchStrategy`        | Searches books by author name                    |
| `CategorySearchStrategy`      | Searches books by category name                  |

##### Overdue Penalty Strategies

**`IPenaltyStrategy` (Interface)**

| Method                                 | Description                                                    |
|----------------------------------------|----------------------------------------------------------------|
| `applyPenalty(member, loan, daysOverdue)` | Pure virtual — applies a penalty to the member for an overdue loan |

| Concrete Strategy             | Description                                                            |
|-------------------------------|------------------------------------------------------------------------|
| `WarningPenaltyStrategy`      | Issues a warning to the member                                         |
| `FinePenaltyStrategy`         | Charges a monetary fine proportional to overdue days                    |
| `SuspendPenaltyStrategy`      | Temporarily suspends the member's borrowing privileges                 |

The `LibraryManager` automatically selects the appropriate penalty strategy based on the number of days overdue.

---

#### 3. Decorator Pattern (`patterns/decorator/`)

Used to dynamically extend a `Book`'s description without modifying the `Book` class itself.

**`BookDecorator : public Book`** (Abstract base decorator)
- Holds a `shared_ptr<Book>` to the decorated book.
- Delegates `getFullDescription()` to the wrapped book.

| Concrete Decorator             | Description                                                      |
|--------------------------------|------------------------------------------------------------------|
| `DifficultyLabelDecorator`     | Appends a difficulty label (e.g., "Beginner", "Advanced")        |
| `SpecialTagDecorator`          | Appends special tags (e.g., "Bestseller", "New Arrival")         |
| `AdditionalInfoDecorator`      | Appends arbitrary extra information from the librarian           |

**Usage example:**
```cpp
auto coreBook = make_shared<Book>(...);
auto withDifficulty = make_shared<DifficultyLabelDecorator>(coreBook, "Advanced");
auto withTags = make_shared<SpecialTagDecorator>(withDifficulty, {"Bestseller"});
cout << withTags->getFullDescription();
```

---

### C. Service Classes (`services/`)

#### 1. `LibraryManager` (Singleton)

The central orchestrator of the application. Manages all books, members, loans, observers, and strategies.

| Key Method                    | Description                                                             |
|-------------------------------|-------------------------------------------------------------------------|
| `getInstance()`               | Returns the singleton instance                                          |
| `addBook(ISBN, title, ...)`   | Adds a new book to the system                                           |
| `addBookToSystem(book)`       | Registers a `Book*` in the internal catalog                             |
| `addMemberToSystem(member)`   | Registers a `Member*` in the system                                     |
| `findBook(ISBN)`              | Looks up a book by ISBN                                                 |
| `findMember(memberID)`        | Looks up a member by ID                                                 |
| `borrowBook(memberID, ISBN)`  | Processes a borrow request — creates a loan and updates availability    |
| `returnBook(memberID, ISBN)`  | Processes a return — closes the loan and restores availability          |
| `setSearchStrategy(strategy)` | Sets the active search algorithm                                        |
| `searchBooks(query)`          | Searches the catalog using the current strategy                         |
| `setObserver(observer)`       | Sets the observer to attach to subjects                                 |
| `addObserverToAllBooks()`     | Attaches the current observer to every `BookSubject`                    |
| `addObserverToAllLoans()`     | Attaches the current observer to every `LoanSubject`                    |
| `startLoanCheckTimer()`       | Starts a background thread that periodically checks for overdue loans   |
| `stopLoanCheckTimer()`        | Stops the background loan-check thread                                  |
| `loadBooksIntoLibrary()`      | Loads books, authors, and categories from CSV files                     |
| `loadMembersFromCSV(path)`    | Loads member data from a CSV file                                       |
| `loadLoansFromCSV(path)`      | Loads loan records from a CSV file                                      |
| `saveBooksNewInfo()`          | Persists updated book data back to CSV                                  |
| `systemLogout()`              | Cleans up observers and stops the timer on shutdown                     |

#### 2. `AuthenticateManager`

Handles user registration and authentication.

| Method                              | Return  | Description                                              |
|-------------------------------------|---------|----------------------------------------------------------|
| `registerUser(userName, pwd, role)` | `bool`  | Registers a new user; returns success status             |
| `loginUser(userName, pwd)`          | `User*` | Authenticates and returns the `User` object, or `nullptr`|
| `logoutUser()`                      | `void`  | Logs out the current user                                |

---

### D. Utility Classes (`utils/`)

#### 1. `CSVHandler`

Static helper for CSV file operations.

| Method                            | Description                                             |
|-----------------------------------|---------------------------------------------------------|
| `parseCSVLine(line)`              | Splits a CSV line into a vector of field strings        |
| `userExists(userID, fileName)`    | Checks if a user ID exists in the given CSV file        |
| `validateCredentials(user, pwd, file)` | Validates login credentials against the CSV        |
| `addUser(id, name, pwd, file)`    | Appends a new user record to the CSV file               |
| `generateUserID(prefix)`         | Generates a unique user ID with the given prefix        |

#### 2. `StringHandler`

String processing utilities for fuzzy search.

| Method                                    | Description                                                      |
|-------------------------------------------|------------------------------------------------------------------|
| `getLowercase(s)`                         | Converts a string to lowercase                                   |
| `findTopMatches(patterns, query, topN)`   | Returns the top-N best-matching books using Levenshtein distance |

---

## Application Flow

### GUI Screens

The application uses a simple **state-machine router** (`AppState` enum) to navigate between screens:

| Screen             | Description                                        | Access         |
|--------------------|----------------------------------------------------|----------------|
| **Login**          | Username/password login form                       | All users      |
| **Register**       | New user registration (Member or Librarian)        | All users      |
| **Main Menu**      | Dashboard with navigation to all features          | Authenticated  |
| **Search Book**    | Search by title, author, or category               | All roles      |
| **Borrow Book**    | Browse and borrow available books                  | Members        |
| **Add Book**       | Add a new book to the catalog                      | Librarians     |
| **List Members**   | View all registered members                        | Librarians     |
| **List Loans**     | View all loan records                              | Librarians     |
| **Member Manage**  | Edit member details                                | Librarians     |
| **Notifications**  | View system notifications                          | Authenticated  |

### Key Workflows

1. **Registration / Login** — `main.cpp` → `AuthenticateManager` → CSV credential store.
2. **Adding a Book** — Librarian navigates to Add Book screen → `LibraryManager::addBook()` → CSV persistence.
3. **Searching Books** — User selects a search strategy (title/author/category) → `LibraryManager::setSearchStrategy()` → `searchBooks(query)`.
4. **Borrowing a Book** — Member selects a book → `LibraryManager::borrowBook()` → creates a `Loan`, decrements `availableCopies`, notifies observers.
5. **Overdue Detection & Penalties** — Background timer thread → `checkLoansAndApplyPenalties()` → selects penalty strategy based on overdue days → applies warning / fine / suspension.
6. **Notifications** — `BookSubject` / `LoanSubject` call `notify()` → attached `MemberObserver` / `LibrarianObserver` receive `update()` → log to file.

---

## Building & Running

### Prerequisites

- **C++17** compatible compiler (MinGW-w64 recommended on Windows)
- **CMake** ≥ 3.10
- **DirectX 9 SDK** (included with Windows SDK)

### Build Steps

**Option 1: Using the build script**
```bash
build.bat
```
This will clean, configure (MinGW Makefiles), and build the project. The executable is output to `build/LMS.exe`.

**Option 2: Manual CMake**
```bash
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
cmake --build . --config Release
```

### Running

```bash
cd build
LMS.exe
```

> **Note:** The application expects the `data/` directory to be located at `../data/` relative to the executable (i.e., one level up from `build/`). Run the executable from inside the `build/` directory.

---

## Important Notes

- **Memory Management:** The Decorator pattern uses `std::shared_ptr` to manage decorated book lifetimes and prevent memory leaks.
- **Thread Safety:** The background loan-check timer uses `std::mutex` to protect shared loan data from race conditions.
- **Error Handling:** CSV parsing includes basic validation; invalid records are skipped gracefully.
- **Data Persistence:** All data is stored in CSV files under `data/`. Changes to books and loans are saved back to CSV on relevant operations.
- **Enumerations:** `BookStatus`, `LoanStatus`, and `Role` are implemented as `enum class` for type safety and readability.
- **Platform:** Currently Windows-only due to the DirectX 9 / Win32 ImGui backend.
