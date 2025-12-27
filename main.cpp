/*
 * Project Name: Parking Lot Simulation System
 * Course: Object-Oriented Programming
 * Description: Final Project
 * * Features:
 * - Inheritance (Vehicle -> Car, Truck, Motorbike)
 * - Polymorphism (calculateFee, displayInfo)
 * - File I/O (Persistence of data)
 * - STL Vector (Dynamic memory management)
 * * Author: Ali Bal
 * Date: December 2025
 */

#include <iostream>
#include <vector>
#include <string>
#include <fstream>  // Required for File I/O (Save/Load)
#include <ctime>    // Required for time tracking
#include <iomanip>  // Required for output formatting

using namespace std;

// ABSTRACT BASE CLASS: VEHICLE
// It cannot be instantiated directly because of pure virtual functions.
class Vehicle {
protected:
    string licensePlate;
    string type;
    time_t entryTime; // Stores the entry time as a Unix Timestamp

public:
    // Constructor: Initializes the vehicle with plate, type, and entry time.
    // If 'entry' is 0, it defaults to the current system time.
    Vehicle(string plate, string type, time_t entry = 0) : licensePlate(plate), type(type) {
        if (entry == 0) {
            entryTime = time(0); // Set to current time
        } else {
            entryTime = entry;   // Set to loaded time (from file)
        }
    }

    // This makes the Vehicle class "Abstract".
    virtual double calculateFee() = 0;

    // Virtual Function: Can be overridden, but has a default implementation.
    virtual void displayInfo() {

        string timeStr = ctime(&entryTime);
        
        // Remove the trailing newline character added by ctime
        if (!timeStr.empty() && timeStr[timeStr.length()-1] == '\n') {
            timeStr.erase(timeStr.length()-1);
        }

        // Print formatted output used "setw" for output formatting
        cout << left << setw(15) << type 
             << setw(15) << licensePlate 
             << "Entry: " << timeStr << endl;
    }

    // Getters
    string getLicensePlate() const { return licensePlate; }
    string getType() const { return type; }
    time_t getEntryTime() const { return entryTime; }

    // destructor
    virtual ~Vehicle() {}
};

// Inherits from Vehicle. Represents standard sized vehicles.
class Car : public Vehicle {
public:
    Car(string plate, time_t t = 0) : Vehicle(plate, "Car", t) {}

    // Override: Implements specific fee logic for Cars.
    double calculateFee() override {

        double hourlyRate = 20.0; // $20.00 per hour
        
        time_t now = time(0);
        double hours = difftime(now, entryTime) / 3600.0; // Convert seconds to hours
        
        // Simulation Rule: Minimum charge is for 1 hour.
        if (hours < 1.0) hours = 1.0;
        
        return hours * hourlyRate; 
    }
};

class Truck : public Vehicle {
public:
    Truck(string plate, time_t t = 0) : Vehicle(plate, "Truck", t) {}

    double calculateFee() override {

        double hourlyRate = 50.0; // large vehicles with higher fees.

        time_t now = time(0);
        double hours = difftime(now, entryTime) / 3600.0;
        
        if (hours < 1.0) hours = 1.0;
        
        return hours * hourlyRate;
    }
};

class Motorbike : public Vehicle {
public:
    Motorbike(string plate, time_t t = 0) : Vehicle(plate, "Motorbike", t) {}

    double calculateFee() override {
    
        double hourlyRate = 10.0; // lower fees

        time_t now = time(0);
        double hours = difftime(now, entryTime) / 3600.0;
        
        if (hours < 1.0) hours = 1.0;

        return hours * hourlyRate;
    }
};

// This class manages the parking operations using a collection of Vehicle objects.
class ParkingLot {
private:
    // Storage: Dynamic list of pointers to Vehicle objects
    // We use pointers (Vehicle*) to store derived objects (Car, Truck) in the same list.
    vector<Vehicle*> parkedVehicles; 
    
    const int capacity = 7; // Max limit for car park
    double totalRevenue;    // total revenue

public:
    // Loads previous data from file upon startup.
    ParkingLot() : totalRevenue(0.0) {
        loadData(); 
    }

    // Saves data and cleans up memory upon exit.
    ~ParkingLot() {
        saveData(); 
        
        // Memory Cleanup: Delete all dynamically allocated vehicle objects
        for (Vehicle* v : parkedVehicles) {
            delete v; 
        }
        parkedVehicles.clear();
    }

    // Method: Park a new vehicle
    // Accepts a base class pointer, allowing any derived vehicle type.

    void parkVehicle(Vehicle* newVehicle) {
        if (parkedVehicles.size() >= capacity) {
            cout << "Parking Lot is Full! " << newVehicle->getLicensePlate() << " cannot enter." << endl;
            delete newVehicle; // Important: Delete the object since we are not storing it.
        } else {
            parkedVehicles.push_back(newVehicle);
            cout << newVehicle->getType() << " (" << newVehicle->getLicensePlate() << ") parked successfully." << endl;
        }
    }

    // Method: Remove a vehicle and calculate fee
    void unparkVehicle(string plate) {
        bool found = false;
        
        // Iterator is used to traverse the vector safely while erasing elements.
        for (auto it = parkedVehicles.begin(); it != parkedVehicles.end(); ++it) {
            if ((*it)->getLicensePlate() == plate) {
                
                // Polymorphism in action: correct calculateFee() is called based on object type.
                double fee = (*it)->calculateFee();
                totalRevenue += fee;

                // Receipt Output
                cout << "\n---------------------------------" << endl;
                cout << "[EXIT] " << (*it)->getLicensePlate() << " is leaving." << endl;
                cout << "Vehicle Type: " << (*it)->getType() << endl;
                cout << "Total Fee: $" << fee << endl;
                cout << "---------------------------------\n" << endl;

                delete *it; // Free the heap memory
                parkedVehicles.erase(it); // Remove the pointer from the vector
                found = true;
                break;
            }
        }

        if (!found) {
            cout << ">> ERROR: Vehicle with plate " << plate << " not found!" << endl;
        }
    }

    // Method: Display status of the parking lot
    void displayStatus() {
        cout << "\n=== PARKING LOT STATUS (" << parkedVehicles.size() << "/" << capacity << ") ===" << endl;
        cout << "Total Revenue: $" << totalRevenue << endl;
        cout << "--------------------------------------------------------" << endl;
        
        if (parkedVehicles.empty()) {
            cout << "Parking lot is currently empty." << endl;
        } else {
            // Ranged-based for loop
            for (Vehicle* v : parkedVehicles) {
                v->displayInfo(); // Polymorphism: Calls the correct display function
            }
        }
        cout << "--------------------------------------------------------\n" << endl;
    }

    // FILE I/O OPERATIONS
    
    // Saves current state to a text file
    void saveData() {
        ofstream outFile("parking_data.txt");
        if (!outFile.is_open()) {
            cout << "Error: Could not open file for saving." << endl;
            return;
        }

        // Format: TYPE LICENSE_PLATE ENTRY_TIMESTAMP
        for (Vehicle* v : parkedVehicles) {
            outFile << v->getType() << " " << v->getLicensePlate() << " " << v->getEntryTime() << endl;
        }
        outFile.close();
        cout << "Data saved successfully." << endl;
    }

    // Loads data from text file
    void loadData() {
        ifstream inFile("parking_data.txt");
        if (!inFile.is_open()) return;

        string type, plate;
        time_t timeEntry;

        // Read file line by line
        while (inFile >> type >> plate >> timeEntry) {
            // Factory Pattern Logic: Create correct object based on string type
            if (type == "Car") {
                parkedVehicles.push_back(new Car(plate, timeEntry));
            } else if (type == "Truck") {
                parkedVehicles.push_back(new Truck(plate, timeEntry));
            } else if (type == "Motorbike") {
                parkedVehicles.push_back(new Motorbike(plate, timeEntry));
            }
        }
        inFile.close();
        cout << "Previous data loaded." << endl;
    }
};

int main() {
    ParkingLot myParkingLot;
    int choice;
    string plate;

    cout << "===========================================" << endl;
    cout << "   Parking Lot Management System   " << endl;
    cout << "===========================================" << endl;

    while (true) {
        cout << "1. Park Car" << endl;
        cout << "2. Park Truck" << endl;
        cout << "3. Park Motorbike" << endl;
        cout << "4. Unpark Vehicle (Pay & Exit)" << endl;
        cout << "5. Display Status" << endl;
        cout << "0. Exit & Save" << endl;
        cout << "Select an option: ";
        
        // Input Validation: Check if user entered a number
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear(); // Clear error flags
            cin.ignore(10000, '\n'); // Discard invalid input
            continue;
        }

        if (choice == 0) break;

        switch (choice) {
            case 1:
                cout << "Enter License Plate: "; cin >> plate;
                myParkingLot.parkVehicle(new Car(plate));
                break;
            case 2:
                cout << "Enter License Plate: "; cin >> plate;
                myParkingLot.parkVehicle(new Truck(plate));
                break;
            case 3:
                cout << "Enter License Plate: "; cin >> plate;
                myParkingLot.parkVehicle(new Motorbike(plate));
                break;
            case 4:
                cout << "Enter License Plate to Unpark: "; cin >> plate;
                myParkingLot.unparkVehicle(plate);
                break;
            case 5:
                myParkingLot.displayStatus();
                break;
            default:
                cout << "Invalid selection! Please try again." << endl;
        }
    }

    cout << "System shutting down. Goodbye!" << endl;
    return 0;
}