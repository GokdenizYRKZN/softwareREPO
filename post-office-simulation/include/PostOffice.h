#ifndef POST_OFFICE_H
#define POST_OFFICE_H

#include "Parcel.h" // Needed for Parcel pointers and PaymentMode

// Forward declaration to avoid circular dependency
class Postman;

class PostOffice {
private:
    Parcel* expressHead;   // Pointer to the head of the express parcels queue
    Parcel* standardHead;  // Pointer to the head of the standard parcels queue
    double totalRevenue;   // Accumulates all shipping fees earned

    // Private helper function to prevent code duplication in destructor
    void clear();

public:
    // Constructor & Destructor
    PostOffice();
    ~PostOffice();

    // Utility
    double calculateFee(double weight, double volume, bool isExpress) const;
    void addRevenue(double amount);

    // Core Behaviors
    // Adds a parcel to the end of the correct queue (express or standard)
    bool processAndStoreParcel(Parcel* p, double payment);

    // Iterates through queues (Express first, then Standard) and transfers 
    // pointer ownership to the Postman until the Postman's limits are reached.
    void loadPostman(Postman& postman);
};

#endif