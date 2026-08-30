#ifndef PARCEL_H
#define PARCEL_H

// Defines the payment modes used across the entire system
enum PaymentMode { SENDER_PAYS, RECEIVER_PAYS };

class Parcel {
private:
    int trackingID;
    double weight;
    double volume;
    bool isExpress;
    PaymentMode payMode;
    double pendingFee;
    char* destination; // Dynamically allocated string for the destination
    
    Parcel* next;      // Pointer for the linked list. PRIVATE FOR SAFETY!

public:
    // Constructor
    Parcel(int id, double w, double vol, bool express, PaymentMode mode, const char* dest);
    
    // Destructor
    ~Parcel();
    
    // Rule of Three (To prevent dynamic memory leaks)
    Parcel(const Parcel& other);            // Copy Constructor
    Parcel& operator=(Parcel other);        // Assignment Operator (Copy-and-Swap Idiom)

    // Getters (Read-only access)
    int getTrackingID() const;
    double getWeight() const;
    double getVolume() const;
    bool getPriority() const;
    PaymentMode getPaymentMode() const;
    double getPendingFee() const;
    Parcel* getNext() const;               // Safely read the next pointer
    
    // Setters and Modifiers (Controlled write access)
    void setNext(Parcel* p);               // Safely update the next pointer
    void setPendingFee(double fee);        // Sets the fee ONLY if RECEIVER_PAYS
    void clearPendingFee();                // Clears the fee after successful payment
};

#endif