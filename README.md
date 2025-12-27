C++ OOP Final Project
# Parking Lot Simulation System

An advanced **Parking Lot Management System** developed in C++ using Object-Oriented Programming (OOP) principles.

## 📌 Features
* **OOP Concepts:** Abstraction, Encapsulation, Inheritance, Polymorphism.
* **Dynamic Memory:** Uses `std::vector` and pointers for efficient memory management.
* **Persistence:** Saves and loads vehicle data using File I/O (`parking_data.txt`).
* **Fee Calculation:** Calculates parking fees dynamically based on vehicle type (Car, Truck, Motorbike) and duration.

## 🛠️ Architecture
* **Vehicle (Abstract Base Class):** Defines the interface.
* **Car / Truck / Motorbike:** Derived classes with specific fee logic.
* **ParkingLot:** Manager class that handles logic and file operations.

## 🚀 How to Run
1.  Compile the code:
    ```bash
    g++ main.cpp -o parking_system
    ```
2.  Run the executable:
    ```bash
    ./parking_system
    ```

## 👨‍💻 Author
**Ali Bal** 
