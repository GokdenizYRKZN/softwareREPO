#include "Postman.h"
#include "Customer.h"
#include "PostOffice.h"
#include <iostream>
#include <cstring>
#include <utility>

using namespace std;

// --- Constructor ---
Postman::Postman(const char* n, double weightLimit, double volLimit) {
    maxWeight = weightLimit;
    maxVolume = volLimit;
    currentWeight = 0.0;
    currentVolume = 0.0;
    collectedCash = 0.0;
    trunk = nullptr;
    
    name = new char[strlen(n) + 1];
    strcpy(name, n);
}

// --- Destructor ---
Postman::~Postman() {
    delete[] name;
    clearTrunk(); // Destroy any undelivered parcels at the end of the day
}

// --- Private Helper to clear trunk memory ---
void Postman::clearTrunk() {
    Parcel* current = trunk;
    while (current != nullptr) {
        Parcel* nextNode = current->getNext();
        delete current;
        current = nextNode;
    }
    trunk = nullptr;
}

// --- Copy Constructor (Deep Copy) ---
Postman::Postman(const Postman& other) {
    maxWeight = other.maxWeight;
    maxVolume = other.maxVolume;
    currentWeight = 0.0; // Reset for the copy, will be updated during deep copy
    currentVolume = 0.0;
    collectedCash = other.collectedCash;
    trunk = nullptr;
    
    name = new char[strlen(other.name) + 1];
    strcpy(name, other.name);
    
    // Deep copy of the linked list
    Parcel* currentOther = other.trunk;
    while (currentOther != nullptr) {
        Parcel* copyParcel = new Parcel(*currentOther); // Uses Parcel's copy constructor
        this->takeParcel(copyParcel);
        currentOther = currentOther->getNext();
    }
}

// --- Assignment Operator ---
Postman& Postman::operator=(Postman other) {
    std::swap(name, other.name);
    std::swap(maxWeight, other.maxWeight);
    std::swap(maxVolume, other.maxVolume);
    std::swap(currentWeight, other.currentWeight);
    std::swap(currentVolume, other.currentVolume);
    std::swap(collectedCash, other.collectedCash);
    std::swap(trunk, other.trunk);
    return *this;
}

// --- Core Behavior: Take Parcel (Constraint Checking) ---
bool Postman::takeParcel(Parcel* p) {
    if (p == nullptr) return false;

    // Check Dual-Capacity Limits
    if (currentWeight + p->getWeight() > maxWeight || 
        currentVolume + p->getVolume() > maxVolume) {
        return false; // Cannot take, limits exceeded
    }

    // Accept the parcel: Add to the front of the trunk (O(1) operation)
    p->setNext(trunk);
    trunk = p;

    // Update current load
    currentWeight += p->getWeight();
    currentVolume += p->getVolume();
    
    return true;
}

// --- Core Behavior: Deliver Parcel (Safe Pointer Detachment) ---
bool Postman::deliverParcelTo(Customer& receiver, int parcelID) {
    Parcel* prev = nullptr;
    Parcel* current = trunk;

    while (current != nullptr) {
        // Assume getTrackingID() exists in Parcel class
        if (current->getTrackingID() == parcelID) {
            
            // 1. TEMPORARY DETACHMENT: Remove from trunk securely
            if (prev == nullptr) {
                trunk = current->getNext();
            } else {
                prev->setNext(current->getNext());
            }
            current->setNext(nullptr); // Isolate the parcel

            // We MUST save weight and volume now, because if delivery is successful, 
            // the Customer object will 'delete' the parcel from memory!
            double pWeight = current->getWeight();
            double pVolume = current->getVolume();

            // 2. Attempt Delivery
            if (receiver.receiveParcel(current, *this)) {
                // Success: The parcel was accepted and destroyed. We update our load.
                currentWeight -= pWeight;
                currentVolume -= pVolume;
                return true;
            } else {
                // Failure: Receiver rejected it (e.g., insufficient funds). 
                // We must re-attach the parcel back to our trunk!
                current->setNext(trunk);
                trunk = current;
                return false;
            }
        }
        prev = current;
        current = current->getNext();
    }
    
    cout << "[Postman Error] Parcel ID " << parcelID << " not found in the trunk.\n";
    return false; // Parcel not found
}

// --- Financial Behaviors ---
void Postman::receiveCash(double amount) {
    collectedCash += amount;
}

void Postman::handOverCashToOffice(PostOffice& po) {
    if (collectedCash > 0) {
        po.addRevenue(collectedCash);
        cout << "[System] Postman " << name << " handed over $" << collectedCash << " to the Post Office.\n";
        collectedCash = 0.0;
    }
}