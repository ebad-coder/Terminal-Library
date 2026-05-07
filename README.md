# Library Management System



A robust, terminal-based Library Management System built with modern C++. This project utilizes the FTXUI library to deliver an interactive Text User Interface (TUI) and relies heavily on Object-Oriented Programming (OOP) principles to handle complex business logic and entity relationships. It serves as an excellent 2nd semester OOP project, demonstrating practical applications of memory management, file handling, and scalable software architecture.



## Object-Oriented Architecture



This project heavily utilizes Object-Oriented Programming (OOP) to create a modular, scalable, and maintainable codebase. Below are the core OOP principles demonstrated in this system:



### 1. Encapsulation

The system protects its internal state by hiding data members and exposing functionality through secure public getters and setters. Classes like Member, Resource, and Transaction declare sensitive variables (e.g., balance, inbox, reservation\_queue) as private or protected. Access to these variables is strictly managed through getter and setter methods (e.g., get\_username(), withdraw(), deposit()), ensuring that the state cannot be modified unexpectedly by external components.



### 2. Inheritance

* Account Hierarchy: Both Member and Admin inherit from Account base class. This allows them to share common attributes like username, password, and inbox, while implementing role-specific logic.
* Resource Hierarchy: Book, Article, and Journal inherit from the Resource base class. They inherit common data (like title and resource\_id) but extend it with their own unique attributes (like isbn for Books or issue\_number for Articles etc).



### 3. Polymorphism

The Account and Resource base classes define pure virtual functions. This allows the FileHandler or ResourceManager to iterate through collections of generic Account\* or Resource\* pointers. When virtual functions are called, the system dynamically invokes the correct overridden method for a Book, Journal, Member, or Admin at runtime.



### 4. Abstraction

Account and Resource contains pure virtual functions which make them abstract base classes and they cannot be instantiated directly. They serve as structural blueprints for their child classes.

The AccountManager and ResourceManager classes abstract away the complex underlying backend logic on resources and accounts and file interactions, providing a clean, high-level API for the main LibrarySystem application logic to consume.



## Technical Highlights



To ensure high performance, memory-safe, modular and maintainable system, it uses modern C++ standards and paradigms:



### 1. Memory Safety (Smart Pointers)

The system heavily relies on unique\_ptr for managing the lifetimes of polymorphic objects (like tracking different Account or Resource types). This guarantees deterministic memory management, ensures ownership semantics, and completely eliminates memory leaks without the dangers of raw pointers.



### 2. Optimized Lookups (Hashmaps)

The core database layers use unordered\_map to store user accounts and library resources. This provides O(1) average time complexity, ensuring that user authentication and resource retrieval remain lightning-fast even as the system scales.



### 3. Persistent File I/O

State is preserved across sessions using robust file stream operations (ifstream / ofstream). The system parses delimited text files to construct complex object upon initialization and serializes them safely upon exit.



### 4. C++ Features

Makes use of modern language features such as make\_unique, auto type deduction, and move semantics (move) to avoid expensive, unnecessary object copies when transferring ownership of objects into the database.



### 5. Interactive UI

This system moves far beyond basic cout/cin console application, it integrates the FTXUI library. It employs a declarative, component-based architecture and an interactive event loop to render a highly responsive and dynamic Text User Interface (TUI).



### 6. Reservation Queues

The system effectively manages demand for limited resources using queue data structure. It implements FIFO logic to manage reservation waitlists, autonomously evaluating and reallocating resources to the next eligible member the moment a resource is returned.



### 7. Notification System

To enhance user experience and maintain system integrity, the application features a robust, custom-built internal notification engine. This system acts as the central communication nervous system for the application, alerting users to state changes and administrative actions.



## Project Features Showcase



### User Registration

A new user can register themselves as members of the library. They are shown a form where they can enter their details and open a new account.
<img src="./screenshots/member_registration.png" />



### User Login

A user can login to their existing account by providing their username and password.
<img src="./screenshots/account_login.png" />



### Member Dashboard

A logged-in member can perform any operation from their dashboard. They are shown a welcome message and any unread notifications.
<img src="./screenshots/member_dashboard.png" />