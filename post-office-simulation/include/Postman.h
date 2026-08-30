#ifndef POSTMAN_H
#define POSTMAN_H

#include "Parcel.h" // Needed for Parcel pointers

// Forward declarations
class Customer;
class PostOffice;

class Postman {
private:
    char* name;
    double maxWeight;
    double maxVolume;
    double currentWeight;
    double currentVolume;
    double collectedCash; // Money collected from RECEIVER_PAYS deliveries
    
    Parcel* trunk; // Head pointer for the Postman's linked list of parcels

    // Private helper for safe memory cleanup
    void clearTrunk();

public:
    // Constructor
    Postman(const char* n, double weightLimit, double volLimit);
    
    // Destructor
    ~Postman();
    
    // Rule of Three (To handle the dynamic name and trunk list safely)
    Postman(const Postman& other);
    Postman& operator=(Postman other);

    // Core Behaviors
    // Attempts to take a parcel. Returns true if within physical limits.
    bool takeParcel(Parcel* p);
    
    // Attempts to deliver a specific parcel to a Customer.
    bool deliverParcelTo(Customer& receiver, int parcelID);
    
    // Financial Interactions
    void receiveCash(double amount);
    void handOverCashToOffice(PostOffice& po);
};

#endif