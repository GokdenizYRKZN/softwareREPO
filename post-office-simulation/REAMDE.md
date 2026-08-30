# Post Office Logistics & Parcel Tracking Simulation

A comprehensive Object-Oriented Programming (OOP) project built in C++ that simulates real-world postal service operations, package tracking, and courier logistics workflows. 

### Architecture & OOP Principles
* **Separation of Concerns:** Strict modular architecture separating interface (`.h` headers) from implementation (`.cpp` sources).
* **Data Encapsulation:** Core entities (`Customer`, `Parcel`, `Postman`) utilize private/protected access modifiers to secure state data, exposing operations strictly through controlled public interfaces.
* **Class Orchestration:** The `PostOffice` acts as the central manager, handling object instantiation, lifecycle management, and dynamic associations between couriers and delivery packages.
* **Memory Management:** Efficient handling of object references and pointers across the logistics lifecycle to prevent memory leaks during package transfers.

### Tech Stack
* **Language:** C++ (Bare-metal implementation)
* **Libraries:** Standard Template Library (STL)
* **Environment:** Cross-platform (Windows/Linux via g++)

### How to Run
Navigate to the project root directory and compile the source files using g++:

```bash
g++ src/*.cpp -I include -o post_office.exe
./post_office.exe