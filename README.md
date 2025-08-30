## Phân tích và Định hướng Chi tiết Hệ thống Quản Lý Thư Viện Điện Tử

### I. Cấu trúc Thư mục Dự án (Gợi ý)

```
ElectronicLibrary/
├── data/                   # Contains data files (e.g., books.csv, members.csv, loans.csv)
├── include/                # Contains header files (.h, .hpp)
│   ├── core/               # Core classes (Book, Member, Loan, Author, Category)
│   ├── patterns/           # Interfaces and classes related to Design Patterns
│   │   ├── observer/
│   │   ├── strategy/
│   │   └── decorator/
│   ├── services/           # Business logic management classes (LibraryManager, AuthManager, NotificationService)
│   └── utils/              # Utility classes (DateUtils, Validator)
├── src/                    # Contains source files (.cpp)
│   ├── core/
│   ├── patterns/
│   │   ├── observer/
│   │   ├── strategy/
│   │   └── decorator/
│   ├── services/
│   └── utils/
├── .gitignore              # Hide build files during compilation
├── build.bat               # Program initialization file
├── CMakeLists.txt          # Or Makefile, used to build the project
├── main.cpp                # Program entry point, handles console UI
└── README.md               # Project tracking notes
```

### II. Phân tích và Thiết kế Lớp (Class Design)

#### A. Các Lớp Cốt Lõi (`core/`)

1.  **`User` (Lớp cơ sở hoặc trừu tượng)**
    - Thuộc tính: `userID`, `username`, `password` (nên mã hóa), `role` (Member, Librarian)
    - Phương thức: `login()`, `register()`, `viewProfile()`
2.  **`Member : public User`**
    - Thuộc tính: `membershipDate`, `borrowedBooks` (danh sách các `Loan*` hoặc `Book*`)
    - Phương thức: `borrowBook()`, `returnBook()`, `viewBorrowedBooks()`
3.  **`Librarian : public User`**
    - Phương thức: `addBook()`, `removeBook()`, `updateBookInfo()`, `manageMember()`, `viewAllLoans()`
4.  **`Book` (Lớp cơ sở cho Decorator)**
    - Thuộc tính: `isbn` (string, unique), `title` (string), `authorIDs` (vector<string>), `categoryID` (string), `publicationYear` (int), `publisher` (string), `totalCopies` (int), `availableCopies` (int), `status` (enum: AVAILABLE, BORROWED, RESERVED, UNDER_MAINTENANCE)
    - Phương thức: `virtual string getFullDescription() const`, `displayBasicInfo() const`, `isAvailable() const`
5.  **`Author`**
    - Thuộc tính: `authorID` (string, unique), `name` (string), `biography` (string)
6.  **`Category`**
    - Thuộc tính: `categoryID` (string, unique), `name` (string), `description` (string)
7.  **`Loan`**
    - Thuộc tính: `loanID` (string, unique), `bookISBN` (string), `memberID` (string), `borrowDate` (Date), `dueDate` (Date), `returnDate` (Date, có thể null), `status` (enum: ACTIVE, RETURNED, OVERDUE)
    - Phương thức: `isOverdue() const`, `calculateFine()` (có thể là một phần của Strategy xử lý vi phạm)

#### B. Design Patterns (`patterns/`)

1.  **Observer Pattern (`patterns/observer/`)**

    - **`ISubject` (Interface)**
      - Phương thức: `virtual void attach(IObserver* observer) = 0;`
      - `virtual void detach(IObserver* observer) = 0;`
      - `virtual void notify() = 0;`
    - **`IObserver` (Interface)**
      - Phương thức: `virtual void update(const string& message, const Book* bookContext = nullptr, Loan* loanContext = nullptr) = 0;`
    - **`BookSubject : public Book, public ISubject`** (Hoặc `Book` có một thành viên `ISubject*`)
      - Quản lý danh sách `vector<IObserver*> observers`.
      - `notify()` khi trạng thái sách thay đổi, sách được đặt trước.
    - **`LoanSubject : public Loan, public ISubject`** (Hoặc `Loan` có một thành viên `ISubject*`)
      - Quản lý danh sách `vector<IObserver*> observers`.
      - `notify()` khi sách sắp đến hạn trả.
    - **`MemberObserver : public IObserver`** (Thành viên nhận thông báo)
      - `update()`: Xử lý thông báo, ví dụ hiển thị ra console cho member đó.
    - **`LibrarianObserver : public IObserver`** (Thủ thư nhận thông báo)
      - `update()`: Xử lý thông báo, ví dụ hiển thị cảnh báo cho thủ thư.
    - **`NotificationService` (`services/`)**
      - Có thể đóng vai trò trung gian, quản lý việc đăng ký observers vào subjects cụ thể.
      - Hoặc các `Subject` tự quản lý. `NotificationService` sẽ chứa các `BookSubject`, `LoanSubject`.
      - Phương thức: `sendDueSoonNotifications()`, `sendReservationAvailableNotifications()`, `checkAndNotifyDueLoans()`.

2.  **Strategy Pattern (`patterns/strategy/`)**

    - **Tìm kiếm sách:**
      - **`ISearchStrategy` (Interface)**
        - Phương thức: `virtual vector<Book*> search(const vector<Book*>& allBooks, const string& query) const = 0;`
      - **`TitleSearchStrategy : public ISearchStrategy`**
      - **`AuthorSearchStrategy : public ISearchStrategy`** (cần join với thông tin Author)
      - **`CategorySearchStrategy : public ISearchStrategy`** (cần join với thông tin Category)
      - Lớp Context (ví dụ trong `LibraryManager`):
        - `ISearchStrategy* currentSearchStrategy;`
        - `setSearchStrategy(ISearchStrategy* strategy);`
        - `vector<Book*> performSearch(const string& query);`
    - **Xử lý vi phạm:**
      - **`IPenaltyStrategy` (Interface)**
        - Phương thức: `virtual void applyPenalty(Member* member, Loan* loan, double overdueDays) = 0;`
      - **`FinePenaltyStrategy : public IPenaltyStrategy`** (Phạt tiền)
      - **`SuspendAccessPenaltyStrategy : public IPenaltyStrategy`** (Tạm ngừng quyền mượn)
      - **`WarningPenaltyStrategy : public IPenaltyStrategy`** (Cảnh cáo)
      - Lớp Context (ví dụ trong `LibraryManager` hoặc `LoanService`):
        - `IPenaltyStrategy* currentPenaltyStrategy;`
        - `setPenaltyStrategy(IPenaltyStrategy* strategy);`
        - `void handleOverdueLoan(Member* member, Loan* loan);` // Sẽ gọi `currentPenaltyStrategy->applyPenalty()`

3.  **Decorator Pattern (`patterns/decorator/`)**
    - **`BookDecorator : public Book` (Lớp trừu tượng)**
      - Thuộc tính: `Book* decoratedBook;` (con trỏ tới đối tượng `Book` hoặc `BookDecorator` khác)
      - Constructor: `BookDecorator(Book* book)`
      - Phương thức: `string getFullDescription() const override { return decoratedBook->getFullDescription(); }` (ví dụ, các phương thức khác cũng ủy quyền)
    - **`DifficultyLabelDecorator : public BookDecorator`**
      - Thuộc tính: `string difficultyLabel;` (VD: "Beginner", "Intermediate", "Advanced")
      - `getFullDescription() const override { return decoratedBook->getFullDescription() + "\nDifficulty: " + difficultyLabel; }`
    - **`SpecialTagDecorator : public BookDecorator`**
      - Thuộc tính: `vector<string> tags;` (VD: "Bestseller", "New Arrival", "Award Winning")
      - `getFullDescription() const override { /* ... thêm tags ... */ }`
    - **`AdditionalInfoDecorator : public BookDecorator`**
      - Thuộc tính: `string detailedInfo;`
      - `getFullDescription() const override { /* ... thêm detailedInfo ... */ }`
    - **Sử dụng:**
      ```cpp
      Book* coreBook = new Book(...);
      Book* bookWithDifficulty = new DifficultyLabelDecorator(coreBook, "Advanced");
      Book* bookWithDifficultyAndTags = new SpecialTagDecorator(bookWithDifficulty, {"Bestseller"});
      cout << bookWithDifficultyAndTags->getFullDescription();
      // Quan trọng: quản lý bộ nhớ cho các decorator và coreBook (dùng smart pointers)
      ```

#### C. Lớp Dịch vụ (`services/`)

1.  **`LibraryManager` (Singleton hoặc đối tượng chính)**
    - Thuộc tính:
      - `vector<Book*> allBooks;` (Sau khi decorate, có thể là `vector<Book*>`, nơi `Book*` có thể trỏ tới `BookDecorator`)
      - `vector<Member*> allMembers;`
      - `vector<Librarian*> allLibrarians;`
      - `vector<Loan*> allLoans;`
      - `vector<Author*> allAuthors;`
      - `vector<Category*> allCategories;`
      - `ISearchStrategy* searchStrategy;`
      - `IPenaltyStrategy* penaltyStrategy;`
      - `NotificationService* notificationService;` (Nếu có)
    - Phương thức:
      - Quản lý sách: `addBook(Book* book)`, `removeBook(const string& isbn)`, `updateBook(Book* book)`, `findBookByISBN(const string& isbn)`
      - Quản lý thành viên: `addMember(Member* member)`, `findMemberByID(const string& memberID)`
      - Mượn/trả: `borrowBook(const string& memberID, const string& isbn)`, `returnBook(const string& memberID, const string& isbn)`
      - Tìm kiếm: `setSearchStrategy()`, `searchBooks(const string& query)`
      - Xử lý vi phạm: `setPenaltyStrategy()`, `processOverdueLoans()` (duyệt `allLoans`, kiểm tra quá hạn, áp dụng `penaltyStrategy`)
      - Thống kê: `generateBookStatistics()`, `generateMemberActivityReport()`
      - Tải/Lưu dữ liệu: `loadData()`, `saveData()`
2.  **`AuthManager` (Quản lý Đăng nhập/Đăng ký)**
    - Phương thức: `registerUser(const string& username, const string& password, Role role)`, `loginUser(const string& username, const string& password)` (trả về `User*` hoặc null), `logoutUser()`
    - Lưu trữ thông tin người dùng (có thể là một phần của `LibraryManager` hoặc file riêng).
3.  **`NotificationService` (Nếu tách riêng khỏi các Subject)**
    - Quản lý các `Subject` (Sách, Giao dịch mượn).
    - Đăng ký `Observer` (Thành viên, Thủ thư) vào các `Subject` tương ứng.
    - Kích hoạt `notify()` trên các `Subject` khi có sự kiện.
    - Ví dụ: `checkAndNotifyDueLoans()`: Duyệt qua các khoản vay, nếu sắp đến hạn, kích hoạt `notify()` trên `LoanSubject` tương ứng.

#### D. Lớp Tiện ích (`utils/`)

1.  **`DateUtils`**
    - Phương thức tĩnh: `getCurrentDate()`, `addDaysToDate(Date d, int days)`, `dateToString(Date d)`, `stringToDate(string s)`, `compareDates(Date d1, Date d2)`.
2.  **`Validator`**
    - Phương thức tĩnh: `isValidEmail(const string& email)`, `isValidISBN(const string& isbn)`, `isStrongPassword(const string& password)`.
3.  **`InputHelper`**
    - Phương thức tĩnh: `getIntInput(const string& prompt)`, `getStringInput(const string& prompt)`.

### III. Luồng Chức năng Chính và Sự Tương tác của Patterns

1.  **Đăng ký/Đăng nhập:** `main.cpp` -> `AuthManager`.
2.  **Thêm sách mới (có thể kèm decorator):**
    - Librarian (UI) -> `LibraryManager::addBook()`
    - Trong `addBook()`, có thể tạo `Book` cơ sở, sau đó bọc bằng các `Decorator` nếu thủ thư chọn.
    - Sách mới được thêm vào `allBooks`.
3.  **Tìm kiếm sách:**
    - User (UI) chọn chiến lược tìm kiếm -> `LibraryManager::setSearchStrategy(new ConcreteSearchStrategy())`.
    - User (UI) nhập query -> `LibraryManager::performSearch(query)` -> `currentSearchStrategy->search()`.
4.  **Mượn sách:**
    - Member (UI) -> `LibraryManager::borrowBook()`.
    - `LibraryManager` tạo `Loan`, cập nhật `availableCopies` của `Book`.
    - `Book` (nếu là `BookSubject`) có thể `notify()` về thay đổi trạng thái.
    - `Loan` (nếu là `LoanSubject`) sẽ được `NotificationService` hoặc `LibraryManager` theo dõi để gửi thông báo sắp đến hạn.
5.  **Thông báo sách sắp đến hạn:**
    - `LibraryManager` (hoặc `NotificationService` chạy định kỳ/khi đăng nhập) kiểm tra các `Loan`.
    - Nếu `Loan` sắp đến hạn, `LoanSubject::notify()` được gọi.
    - `MemberObserver` (đã đăng ký với `LoanSubject` đó) nhận `update()` và hiển thị thông báo cho member.
6.  **Xử lý sách quá hạn:**
    - `LibraryManager` xác định sách quá hạn.
    - `LibraryManager` chọn/thiết lập `IPenaltyStrategy` (có thể dựa trên quy định hoặc cấu hình).
    - `LibraryManager::handleOverdueLoan()` -> `currentPenaltyStrategy->applyPenalty()`.
7.  **Xem thông tin chi tiết sách:**
    - User (UI) -> `LibraryManager::findBookByISBN()` -> lấy `Book*`.
    - Gọi `book->getFullDescription()`. Nếu sách đã được decorate, thông tin mở rộng sẽ được hiển thị.

### IV. Chức năng của từng phương thức trong từng UML

#### A. Các Lớp Cốt Lõi (`core/`)

1.  **`User`**
    - login(): bool : kiếm tra xem người dùng đã đăng nhập thành công không
    - register(): bool: kiểm tra xem người dùng đã đăng kí thành công không
    - viewProfile(): void: xem thông tin của người dùng
2.  **`Member : public User`**
    - borrowBook(isbn: string): bool: cuốn sách đó có được người dùng thuê không
    - returnBook(isbn : string): bool: cuốn sách đó đã được người dùng trả về chưa
    - viewBorrowedBooks(): vector<Book\*>: những cuốn sách người dùng đã mượn
3.  **`Librarian : public User`**
    - addBook(book: Book\*): void: thêm sách vào thư viện
    - removeBook(isbn: string): bool: kiểm tra xem cuốn sách đó đã được loại bỏ chưa
    - updateBookInfo(book: Book): void: cập nhật thông tin của sách
    - managerMemeber(member: Member\*): void: quản lý thành viên thuộc thư viện
    - viewAllLoans(): vector<Loans\*>: kiểm tra những khoản nợ sách của thành viên
4.  **`Book` (Lớp cơ sở cho Decorator)**
    - getFullDescription(): string: đưa mô tả cho cuốn sách
    - displayBasicInfo(): void: trình bày những thông tin cơ bản về cuốn sách
    - isAvaliable(): bool: kiểm tra xem sách còn trống không
5.  **`Author`**
6.  **`Category`**
7.  **`Loan`**
    - isOverdue(): bool: kiểm tra xem đã quá hạn chưa
    - calculateFine(): double: tính xem bao giờ hết hạn

#### B. Design Patterns (`patterns/`)

1.  **Observer Pattern (`patterns/observer/`)**

    - **`ISubject` (Interface)**
      - attach(observer: IObserver\*): thêm observer vào danh sách theo dõi
      - detach(observer: IObserver\*): void: gỡ observer khỏi danh sách
      - notify(): void: thông báo đến tất cả observer khi có thay đổi
    - **`IObserver` (Interface)**
      - update(message: string, book: Book*, loan: Loan*): void: nhận thông báo và xử lý tương ứng
    - **`BookSubject : public Book, public ISubject`**
      - notify(): để thông báo khi sách được đặt trước, mượn, hoặc thay đổi trạng thái
      - setStatus(status: BookStatus): cài đặt trạng thái của sách
    - **`LoanSubject : public Loan, public ISubject`**
      - notify(): thông báo khi khoản vay sắp đến hạn hoặc quá hạn
      - checkDueDate(): kiểm tra hạn của sách
    - **`MemberObserver : public IObserver`**
      - update(): nhận thông báo liên quan đến sách mượn, nhắc nhở hạn trả
    - **`LibrarianObserver : public IObserver`**
      - update(): nhận thông báo để xử lý quá hạn, thống kê
    - **`NotificationService` (`services/`)**
      - sendDueSoonNotifications(): gửi thông báo đến các thành viên có sách sắp đến hạn.
      - sendReservationAvailableNotifications(): thông báo sách đã sẵn sàng cho người đặt
      - checkAndNotifyDueLoans(): kiểm tra tất cả các khoản mượn và gọi notify() nếu cần

2.  **Strategy Pattern (`patterns/strategy/`)**

    - **Tìm kiếm sách:**
      - **`ISearchStrategy` (Interface)**
        - search(books, query): vector<Book\*>: interface tìm kiếm sách theo các tiêu chí khác nhau
      - **`TitleSearchStrategy : public ISearchStrategy`**
        - search(books: vector<Book\*>, query: string) để thực hiện tìm kiếm theo tiêu đề / tác giả / thể loại
      - **`AuthorSearchStrategy : public ISearchStrategy`**
        - applyPenalty(member, loan, overdueDays): giao diện áp dụng xử phạt khi trễ hạn
    - **Xử lý vi phạm:**
      - **`IPenaltyStrategy` (Interface)**
        - applyPenalty(member, loan, overdueDays): giao diện áp dụng xử phạt khi trễ hạn
      - **`FinePenaltyStrategy : public IPenaltyStrategy`**
        - applyPenalty(): phạt tiền thành viên
      - **`SuspendAccessPenaltyStrategy : public IPenaltyStrategy`**
        - applyPenalty(): tạm khóa quyền mượn
      - **`WarningPenaltyStrategy : public IPenaltyStrategy`**
        - applyPenalty(): chỉ cảnh báo

3.  **Decorator Pattern (`patterns/decorator/`)**
    - **`BookDecorator : public Book` (Lớp trừu tượng)**
      - getFullDescription(): mở rộng thông tin của sách bằng cách bọc ngoài một Book khác
    - **`DifficultyLabelDecorator : public BookDecorator`**
      - thêm thông tin về độ khó
    - **`SpecialTagDecorator : public BookDecorator`**
      - thêm các tag như "Bestseller", "New"
    - **`AdditionalInfoDecorator : public BookDecorator`**
      - thêm mô tả chi tiết từ người quản lý

#### C. Lớp Dịch vụ (`services/`)

1.  **`LibraryManager` (Singleton hoặc đối tượng chính)**
    - Thuộc tính:
      - `vector<Book*> allBooks;`
      - `vector<Member*> allMembers;`
      - `vector<Librarian*> allLibrarians;`
      - `vector<Loan*> allLoans;`
      - `vector<Author*> allAuthors;`
      - `vector<Category*> allCategories;`
      - `ISearchStrategy* searchStrategy;`
      - `IPenaltyStrategy* penaltyStrategy;`
      - `NotificationService* notificationService;`
    - Phương thức:
      - addBook(book): thêm sách mới vào thư viện
      - removeBook(isbn): xóa sách khỏi thư viện nếu tồn tại
      - updateBook(book): cập nhật thông tin sách
      - findBookByISBN(isbn): Book\*: tìm sách theo mã ISBN
      - addMember(member): thêm thành viên mới
      - findMemberByID(memberID): Member\*: tìm thành viên theo ID
      - borrowBook(memberID, isbn): bool: xử lý yêu cầu mượn sách
      - returnBook(memberID, isbn): bool: xử lý yêu cầu trả sách
      - setSearchStrategy(strategy): thiết lập chiến lược tìm kiếm hiện tại
      - searchBooks(query): vector<Book\*>: thực hiện tìm kiếm sách
      - setPenaltyStrategy(strategy): thiết lập chiến lược xử phạt quá hạn
      - handleOverdueLoan(member, loan): xử lý mượn quá hạn (gọi applyPenalty())
      - getInstance(): LibraryManager\*: lấy singleton instance (nếu dùng singleton)
2.  **`AuthManager` (Quản lý Đăng nhập/Đăng ký)**
    - registerUser(username, password, role): bool: đăng ký người dùng mới
    - loginUser(username, password): User\*: xác thực và trả về đối tượng User
    - logoutUser(): void: đăng xuất người dùng hiện tại
3.  **`NotificationService` (Nếu tách riêng khỏi các Subject)**
    - sendDueSoonNotifications(): kiểm tra khoản mượn gần đến hạn và gửi thông báo
    - sendReservationAvailableNotifications(): thông báo sách đã có sẵn
    - checkAndNotifyDueLoans(): duyệt toàn bộ khoản vay, phát hiện sách đến hạn hoặc quá hạn và gửi thông báo

### V. Lưu ý Quan trọng

- **Quản lý bộ nhớ:** Sử dụng smart pointers (`std::unique_ptr`, `std::shared_ptr`) để tránh memory leak, đặc biệt với các đối tượng được tạo động và trong Decorator Pattern.
- **Xử lý lỗi:** Sử dụng exception handling (`try-catch`) cho các tình huống lỗi (ví dụ: file không tìm thấy, input không hợp lệ).
- **Tính nhất quán dữ liệu:** Đảm bảo khi một hành động xảy ra (ví dụ mượn sách), tất cả các đối tượng liên quan (Book, Member, Loan) được cập nhật.
- **Hằng số và Enum:** Sử dụng `const` cho các giá trị không đổi và `enum class` cho các trạng thái, vai trò để code dễ đọc và bảo trì.
