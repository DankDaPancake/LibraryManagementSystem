# Kịch Bản Demo Video - Library Management System
**Thời lượng**: 15-18 phút  
**Ngôn ngữ mô tả**: Tiếng Việt  
**Script lời nói**: Tiếng Anh

---

## Chuẩn Bị Trước Khi Quay
### Yêu cầu kỹ thuật:
- Phần mềm screen recording (OBS Studio, Camtasia)
- Microphone chất lượng tốt
- Môi trường yên tĩnh
- Build project thành công, chạy được ứng dụng
- Chuẩn bị dữ liệu mẫu (books.csv, members.csv, etc.)

### Chuẩn bị nội dung:
- File report.md đã hoàn thành
- UML diagrams rõ ràng
- Code examples được highlight
- Screenshots của UI
- Flowcharts và architectural diagrams

---

## Phân Đoạn Demo Chi Tiết

### PHẦN 1: GIỚI THIỆU NHÓM VÀ DỰ ÁN (2-3 phút)

#### Nội dung trình chiếu:
- Slide tiêu đề với tên nhóm và dự án
- Thông tin thành viên nhóm
- Mục tiêu dự án và requirements

#### Script lời nói (Tiếng Anh):
```
"Good morning/afternoon everyone. Welcome to our Library Management System presentation. 

I am [Your Name], representing our development team. Today, we will demonstrate our object-oriented programming project - a comprehensive Library Management System built with C++.

Our team consists of [number] members: [briefly introduce each member and their roles]. Each member contributed significantly to different aspects of the project, from core development to UI design and testing.

The main objective of this project was to apply the four pillars of Object-Oriented Programming - Encapsulation, Inheritance, Polymorphism, and Abstraction - while implementing at least two design patterns to solve real-world library management problems.

Our system addresses the key challenges faced by modern libraries: efficient book catalog management, user authentication, loan processing, and automated notification systems."
```

#### Hành động của người trình bày:
- Đứng tự nhiên, nhìn thẳng vào camera
- Sử dụng pointer để chỉ các thông tin quan trọng trên slide
- Giọng điệu tự tin, rõ ràng

---

### PHẦN 2: KIẾN TRÚC HỆ THỐNG VÀ OOP PRINCIPLES (4-5 phút)

#### Nội dung trình chiếu:
- System architecture diagram
- UML class diagrams cho core classes
- Code examples demonstrating OOP principles

#### Script lời nói (Tiếng Anh):
```
"Let me start by explaining our system architecture and how we implemented the four OOP principles.

First, let's look at our class hierarchy. [Show UML diagram]

ENCAPSULATION: Each class has private data members with controlled access through public methods. For example, our Book class encapsulates ISBN, title, author information, and availability status. Users can only modify these through validated setter methods, ensuring data integrity.

INHERITANCE: We implemented a clear inheritance hierarchy with the User class as our abstract base class. Both Member and Librarian classes inherit from User, sharing common attributes like userID, name, and email, while implementing their specific behaviors.

POLYMORPHISM: Our system demonstrates polymorphism through virtual functions and strategy patterns. For instance, the getUserType() method is overridden in derived classes, and our search strategies provide different implementations of the same interface.

ABSTRACTION: We created several abstract interfaces like IObserver, ISearchStrategy, and IPenaltyStrategy. These hide complex implementation details behind simple, well-defined contracts."
```

#### Hành động của người trình bày:
- Zoom vào từng phần của UML diagram khi giải thích
- Highlight code examples tương ứng
- Sử dụng animation để show inheritance relationships

---

### PHẦN 3: DESIGN PATTERNS IMPLEMENTATION (4-5 phút)

#### Nội dung trình chiếu:
- Observer Pattern UML và code
- Strategy Pattern implementation
- Decorator Pattern examples

#### Script lời nói (Tiếng Anh):
```
"Now, let's examine the three design patterns we implemented to solve specific architectural challenges.

OBSERVER PATTERN: We use this pattern for our notification system. When a book's status changes or a loan becomes overdue, relevant observers are automatically notified. BookSubject and LoanSubject act as publishers, while MemberObserver and LibrarianObserver act as subscribers. This ensures loose coupling between notification sources and recipients.

[Show code example of observer implementation]

STRATEGY PATTERN: We implemented this pattern twice - for search algorithms and penalty systems. Users can search books by title, author, or category using different search strategies. Similarly, the system can apply different penalty strategies like fines, suspension, or warnings for overdue books. This design allows easy addition of new strategies without modifying existing code.

[Demonstrate strategy switching in code]

DECORATOR PATTERN: This pattern allows us to enhance book information display without modifying the core Book class. We can dynamically add difficulty labels, special tags, or additional information to books. Multiple decorators can be chained together for complex enhancements.

[Show decorator chain example]

These patterns significantly improve our code's flexibility, maintainability, and extensibility."
```

#### Hành động của người trình bày:
- Draw connections between pattern components on screen
- Show before/after code comparisons
- Demonstrate pattern benefits with real examples

---

### PHẦN 4: LIVE DEMO ỨNG DỤNG (5-6 phút)

#### Nội dung trình chiếu:
- Screen recording của ứng dụng chạy thực tế
- Demo các tính năng chính

#### Script lời nói (Tiếng Anh):
```
"Now, let's see our system in action. I'll demonstrate the key features and user interactions.

[Launch application]

AUTHENTICATION SYSTEM: Here's our login interface built with ImGui. The system supports both member and librarian accounts with role-based access control. Let me log in as a library member first.

[Login as member]

MEMBER INTERFACE: As you can see, members have access to book search, borrowing, returning, and profile management features. The interface is clean and intuitive with proper error handling and user guidance.

[Demonstrate search functionality]

SEARCH SYSTEM: I'm searching for books about 'Python programming'. Notice how our strategy pattern allows switching between different search criteria. The results show enhanced book information thanks to our decorator pattern.

[Show search results with decorated information]

BORROWING PROCESS: Let me borrow this Python book. The system checks availability, creates a loan record, and updates the book status. Notice the real-time notification system in action.

[Complete borrowing process]

OBSERVER PATTERN IN ACTION: The notification appears because our BookSubject notified all registered observers about the status change.

[Switch to librarian account]

LIBRARIAN FEATURES: Librarians have additional privileges including book management, member administration, and system configuration. The penalty system allows applying different strategies based on violation severity.

[Demonstrate admin features]

DATA PERSISTENCE: All changes are automatically saved to CSV files, ensuring data persistence between sessions."
```

#### Hành động của người trình bày:
- Thao tác mouse/keyboard một cách mượt mà
- Giải thích từng bước thực hiện
- Highlight các notifications và responses của system

---

### PHẦN 5: CODE WALKTHROUGH (3-4 phút)

#### Nội dung trình chiếu:
- IDE với source code
- Key classes và methods
- Critical implementation details

#### Script lời nói (Tiếng Anh):
```
"Let me show you some key implementation details in our source code.

[Open IDE/editor]

Here's our LibraryManager class - the heart of our system. Notice how it uses dependency injection for strategies and manages observer collections for the notification system.

[Navigate through code structure]

This is our Observer pattern implementation. The BookSubject class maintains a list of observers and notifies them when status changes occur. The implementation follows RAII principles for proper memory management.

[Show observer implementation]

Our Strategy pattern implementation demonstrates clean separation of concerns. Each search strategy implements the same interface but provides different algorithms for finding books.

[Display strategy classes]

The Decorator pattern implementation uses smart pointers to ensure proper memory management while allowing flexible composition of book enhancements.

[Show decorator chain]

Notice our attention to modern C++ practices: we use smart pointers, RAII principles, const-correctness, and proper exception handling throughout the codebase."
```

#### Hành động của người trình bày:
- Navigate code smoothly, zoom vào những phần quan trọng
- Highlight syntax và design decisions
- Explain complex logic với ví dụ cụ thể

---

### PHẦN 6: CHALLENGES VÀ FUTURE WORK (1-2 phút)

#### Nội dung trình chiếu:
- Challenges faced slide
- Future enhancements roadmap
- Lessons learned

#### Script lời nói (Tiếng Anh):
```
"During development, we encountered several interesting challenges that enhanced our learning experience.

MEMORY MANAGEMENT: Managing memory for decorator chains and observer collections required careful implementation of RAII principles and smart pointers.

OBSERVER LIFECYCLE: Preventing dangling pointers when observers are destroyed was solved through proper detach mechanisms and validity checks.

UI STATE MANAGEMENT: We implemented a Router pattern to handle complex UI state transitions cleanly.

For future enhancements, we're considering:
- Database migration from CSV to SQLite for better performance
- Web interface for remote access  
- Mobile application development
- Integration with external book databases
- Advanced analytics and reporting features

This project significantly improved our understanding of object-oriented design principles, modern C++ practices, and software architecture patterns."
```

---

### PHẦN 7: KẾT LUẬN (1 phút)

#### Script lời nói (Tiếng Anh):
```
"In conclusion, our Library Management System successfully demonstrates:
- All four OOP principles with practical implementations
- Three design patterns solving real architectural problems  
- Modern C++ development practices
- User-friendly interface with robust error handling
- Extensible architecture ready for future enhancements

The system is fully functional and ready for deployment in educational or small library environments.

Thank you for your attention. We're happy to answer any questions about our implementation, design decisions, or technical challenges we overcame."
```

---

## Checklist Chuẩn Bị Demo

### Trước khi quay:
- [ ] Kiểm tra âm thanh và video quality
- [ ] Test run toàn bộ demo flow
- [ ] Chuẩn bị backup slides và code
- [ ] Đảm bảo ứng dụng build và chạy ổn định
- [ ] Chuẩn bị dữ liệu demo phong phú

### Trong khi quay:
- [ ] Nói rõ ràng, tốc độ vừa phải
- [ ] Highlight những điểm quan trọng
- [ ] Avoid technical jargon, explain clearly
- [ ] Show confidence và enthusiasm
- [ ] Keep eye contact với camera

### Sau khi quay:
- [ ] Review video quality và audio
- [ ] Check timing (15-18 phút)
- [ ] Add captions nếu cần thiết
- [ ] Export với quality phù hợp
- [ ] Backup video file

---

## Tips Presentation Hiệu Quả

### Về giọng nói:
- Tốc độ: 120-150 từ/phút
- Giọng điệu: Tự tin, nhiệt tình nhưng không quá excited
- Pause: Sử dụng pause để tạo emphasis
- Volume: Consistent và rõ ràng

### Về body language:
- Đứng thẳng, tự tin
- Hand gestures tự nhiên
- Nhìn vào camera, không đọc script
- Facial expressions phù hợp với nội dung

### Về technical presentation:
- Explain before you show code
- Zoom vào những phần quan trọng
- Highlight key concepts với colors/arrows
- Don't rush through complex topics
- Provide context for each demonstration

### Về time management:
- Dành 60% time cho demo thực tế
- 25% cho architecture và design patterns
- 15% cho introduction và conclusion
- Prepare backup content nếu có thời gian thừa
- Have natural stopping points nếu cần cut short

---

## Emergency Backup Plans

### Nếu app crash:
- Có sẵn recorded demo video backup
- Screenshots của key features
- Explain implementation từ code without running

### Nếu có technical issues:
- Static slides với code examples
- UML diagrams detailed explanation
- Architecture walkthrough without live demo

### Nếu thiếu thời gian:
- Priority order: Core demo > Design patterns > Code walkthrough
- Skip detailed code explanation, focus on results
- Summarize challenges quickly

### Nếu có thời gian thừa:
- Deep dive vào specific design pattern
- Show additional features
- Explain testing và validation process
- Discuss performance considerations
