#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "Parcel.h" // We need PaymentMode and Parcel class details

// Forward declarations to prevent circular dependency errors
class PostOffice;
class Postman;

class Customer {
private:
    char* name;     // Dynamically allocated string for the customer's name
    double wallet;  // Customer's financial balance

public:
    // Constructor
    Customer(const char* n, double initialMoney);
    
    // Destructor
    ~Customer();
    
    // Rule of Three (Crucial for safe dynamic string management)
    Customer(const Customer& other);            // Copy Constructor
    Customer& operator=(Customer other);        // Assignment Operator
    
    // Core Behaviors
    // Attempts to send a parcel. Returns true if successful (sufficient funds, etc.)
    bool sendParcel(PostOffice& po, int id, double w, double vol, bool express, PaymentMode mode, const char* dest);
    
    // Attempts to receive a parcel. Deletes the parcel from memory if successful.
    bool receiveParcel(Parcel* p, Postman& pm);
    
    // Getters (Read-only access)
    double getWallet() const;
    const char* getName() const;
};

#endif