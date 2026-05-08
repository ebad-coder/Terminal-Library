# Library Management System



A terminal-based Library Management System built with modern C++. This project utilizes the FTXUI library to deliver an interactive Text User Interface (TUI) and relies heavily on Object-Oriented Programming (OOP) principles to handle business logic and entity relationships. It serves as an excellent 2nd semester OOP project, demonstrating practical applications of memory management, file handling, and scalable software architecture.



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
<p align="center">
  <img src="./screenshots/member_registration.png" width="60%"/>
</p>

### User Login
A user can login to their existing account by providing their username and password.
<p align="center">
  <img src="./screenshots/account_login.png" width="60%" />
</p>

### Member Dashboard
A logged-in member can perform any operation from their dashboard. They are shown a welcome message and any unread notifications.
<p align="center">
  <img src="./screenshots/member_dashboard.png" width="60%" />
</p>

### Search a Resource
Allow member to search any resource against different criterias (title/category/creator). They are shown the matched resources. Member can select any resource to view its details.
<p align="center">
  <img src="./screenshots/search_resource_1.png" width="48%">
  <img src="./screenshots/search_resource_2.png" width="48%">
</p>

### View Available Resources
Allow member to view all the available resources in the catalog. They can select any resource to view its details.
<p align="center">
  <img src="./screenshots/search_resource_1.png" width="48%">
  <img src="./screenshots/view_available_resources_2.png" width="48%">
</p>

### Borrow a Resource
Allow member to borrow any available resource. First they search for the resource, then they select the resource they want to borrow, and finally confirm the borrowing.
<p align="center">
  <img src="./screenshots/borrow_resource_1.png" width="32%">
  <img src="./screenshots/borrow_resource_2.png" width="32%">
  <img src="./screenshots/borrow_resource_3.png" width="32%">
</p>

### Return a Resource
Allow member to return a borrowed resource. Confirm to return the selected resource.
<p align="center">
  <img src="./screenshots/return_resource_1.png" width="48%">
  <img src="./screenshots/return_resouce_2.png" width="48%">
</p>

### Reserve a Resource
Allow member to reserve an unavailable resource. First they search for the resource to reserve, then they select the resource they want to reserve, and confirm the reservation.
<p align="center">
  <img src="./screenshots/borrow_resource_1.png" width="32%">
  <img src="./screenshots/borrow_resource_2.png" width="32%">
  <img src="./screenshots/reserve_resource.png" width="32%">
</p>

### View Inbox
Allow member to view notifications in their inbox. They can select any notification to view its details.
<p align="center">
  <img src="./screenshots/view_inbox_1.png" width="48%">
  <img src="./screenshots/view_inbox_2.png" width="48%">
</p>

### View Borrowing History
Allow member to view his/her borrowing history. Member can select any past transaction to view its details.
<p align="center">
  <img src="./screenshots/view_borrowing_history_1.png" width="48%">
  <img src="./screenshots/view_borrowing_history_2.png" width="48%">
</p>

### View Balance
Allow member to view their account balance. Member can also deposit money to their account.
<p align="center">
  <img src="./screenshots/view_balance_1.png" width="48%">
  <img src="./screenshots/view_balance_2.png" width="48%">
</p>

### View Membership Status
Allow member to view their membership status and the benefits of each membership tier. A member can upgrade their membership status by donating 3 resources or paying a one-time fee of Rs.2000.
<p align="center">
  <img src="./screenshots/view_membership_status.png" width="60%">
</p>

### View Profile
Allow member to view their profile details. They can also edit their profile details or permanently close their account.
<p align="center">
  <img src="./screenshots/veiw_profile_1.png" width="48%">
  <img src="./screenshots/view_profile_2.png" width="48%">
</p>

### Donate a Resource
Allow member to donate a resource to the library. The Admin first accepts this donated resource, then it is added to the library catalog. First they select which resource type to donate, then enter the details of the donated resource.
<p align="center">
  <img src="./screenshots/donate_resource_1.png" width="48%">
  <img src="./screenshots/donate_resource_2.png" width="48%">
</p>

### Admin Dashboard
A logged-in admin can perform any operation from their dashboard. They are shown a welcome message and any unread donation requests.
<p align="center">
  <img src="./screenshots/admin_dashboard.png" width="60%">
</p>

### Add a Resource
Allow admin to add a new resource to the library catalog. First they select the resource type, then enter the details of the resource to add through a form.
<p align="center">
  <img src="./screenshots/donate_resource_1.png" width="48%">
  <img src="./screenshots/donate_resource_2.png" width="48%">
</p>

### Update a Resource
Allow admin to update an existing resource. First they search for the resource to update, then update the details of the selected resource through a form.
<p align="center">
  <img src="./screenshots/borrow_resource_1.png" width="48%">
  <img src="./screenshots/update_resource.png" width="48%">
</p>

### Remove a Resource
Allow admin to remove a resource from the library catalog. First they search for the resource to remove, then confirm the operation.
<p align="center">
  <img src="./screenshots/borrow_resource_1.png" width="48%">
  <img src="./screenshots/remove_resource.png" width="48%">
</p>

### View All Members
Allow admin to view all registered members in the library. They can select any member to view their details. They can also view their complete borrowing history.
<p align="center">
  <img src="./screenshots/view_all_members_1.png" width="48%">
  <img src="./screenshots/view_all_members_2.png" width="48%">
</p>

### Send a Notification
Allow admin to send a notification to a specific member or to all members of the library. They can write the message through a form.
<p align="center">
  <img src="./screenshots/send_notification_1.png" width="48%">
  <img src="./screenshots/send_notification_2.png" width="48%">
</p>

### View Donation Requests
Allow admin to view resource donation requests submitted by members. They can review, accept, or reject the request.
<p align="center">
  <img src="./screenshots/view_donation_requests_1.png" width="48%">
  <img src="./screenshots/view_donation_requests_2.png" width="48%">
</p>

### Generate a Report
Allow admin to generate a report about all the issued and overdue resources. They can see the details of who has borrowed what, and also the number of days since the resource is due.
<p align="center">
  <img src="./screenshots/generate_report.png" width="60%">
</p>



## Project Structure

* src/: Contains all C++ implementation files, handling the core logic, UI rendering, and data management.  

* include/: Contains the header files defining the architecture of the system's classes (e.g., Accounts, Resources, Transactions, Notification).  

* data/: Utilizes local text files (accounts.txt, resources.txt, transactions.txt, notifications.txt, and pending_resources.txt) to persistently store database records.  

* CMakeLists.txt: The CMake build configuration file responsible for fetching dependencies and compiling the project.



## Build Instructions

The project uses CMake for building and will automatically fetch and link the required FTXUI core modules. So make sure you have a C++ compiler, CMake and Git installed in your computer.

* Create a build directory and navigate into it:
    mkdir build && cd build

* Generate the build files:
    cmake ..

* Compile the executable:
    cmake --build . 
    if above command failed to build the project then run: cmake -G "MinGW Makefiles" ..

* Run the application:
    ./LibrarySystem



## Contributors

Designed and developed by a group of 2nd semester computer systems engineering students at NED University
* Muhammad Ebad Adam
* Uzair Mehmood
* Muhammad Ahmed Shaikh
* Muhammad Sarim Sarwar