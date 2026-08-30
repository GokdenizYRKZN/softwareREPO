#include "Parcel.h"
#include <iostream>
#include <cstring>
#include <utility> // Required for std::swap

// --- Constructor ---
Parcel::Parcel(int id, double w, double vol, bool express, PaymentMode mode, const char* dest) {
    trackingID = id;
    weight = w;
    volume = vol;
    isExpress = express;
    payMode = mode;
    pendingFee = 0.0; // Default is 0. It is updated later if RECEIVER_PAYS.
    next = nullptr;
    
    // Dynamic memory allocation for the destination string
    destination = new char[strlen(dest) + 1];
    strcpy(destination, dest);
}

// --- Destructor ---
Parcel::~Parcel() {
    delete[] destination; // Prevents memory leaks by freeing the heap
}

// --- Copy Constructor ---
Parcel::Parcel(const Parcel& other) {
    trackingID = other.trackingID;
    weight = other.weight;
    volume = other.volume;
    isExpress = other.isExpress;
    payMode = other.payMode;
    pendingFee = other.pendingFee;
    
    // We reset the next pointer for the new copy to avoid dangling pointers
    next = nullptr; 
    
    // Deep copy for the dynamically allocated string
    destination = new char[strlen(other.destination) + 1];
    strcpy(destination, other.destination);
}

// --- Assignment Operator (Copy-and-Swap Idiom) ---
Parcel& Parcel::operator=(Parcel other) {
    // Swapping local variables with the temporary 'other' object
    std::swap(trackingID, other.trackingID);
    std::swap(weight, other.weight);
    std::swap(volume, other.volume);
    std::swap(isExpress, other.isExpress);
    std::swap(payMode, other.payMode);
    std::swap(pendingFee, other.pendingFee);
    std::swap(destination, other.destination);
    std::swap(next, other.next);
    
    return *this;
}

// --- Getters ---
int Parcel::getTrackingID() const { return trackingID; }
double Parcel::getWeight() const { return weight; }
double Parcel::getVolume() const { return volume; }
bool Parcel::getPriority() const { return isExpress; }
PaymentMode Parcel::getPaymentMode() const { return payMode; }
double Parcel::getPendingFee() const { return pendingFee; }
Parcel* Parcel::getNext() const { return next; }

// --- Setters and Modifiers ---
void Parcel::setNext(Parcel* p) { 
    next = p; 
}

void Parcel::setPendingFee(double fee) {
    // Ensuring the fee is only applied to valid payment modes
    if (payMode == RECEIVER_PAYS) {
        pendingFee = fee;
    }
}

void Parcel::clearPendingFee() { 
    pendingFee = 0.0; 
}