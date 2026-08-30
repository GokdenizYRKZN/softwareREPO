#include "PostOffice.h"
#include "Postman.h"
#include <iostream>

using namespace std;

// --- Constructor ---
PostOffice::PostOffice() {
    expressHead = nullptr;
    standardHead = nullptr;
    totalRevenue = 0.0;
}

// --- Destructor ---
PostOffice::~PostOffice() {
    clear(); // Safely deletes all remaining parcels in the post office
}

// --- Private Helper: Clear Memory ---
void PostOffice::clear() {
    // Clear express queue
    Parcel* current = expressHead;
    while (current != nullptr) {
        Parcel* nextNode = current->getNext();
        delete current;
        current = nextNode;
    }
    expressHead = nullptr;

    // Clear standard queue
    current = standardHead;
    while (current != nullptr) {
        Parcel* nextNode = current->getNext();
        delete current;
        current = nextNode;
    }
    standardHead = nullptr;
}

// --- Utility Functions ---
double PostOffice::calculateFee(double weight, double volume, bool isExpress) const {
    // A basic logistical formula: Base rate + weight rate + volume rate
    double baseFee = 5.0;
    double fee = baseFee + (weight * 2.0) + (volume * 10.0);
    
    // Express priority adds a 50% surcharge
    if (isExpress) {
        fee *= 1.5; 
    }
    return fee;
}

void PostOffice::addRevenue(double amount) {
    totalRevenue += amount;
}

// --- Core Behavior: Store Parcel ---
bool PostOffice::processAndStoreParcel(Parcel* p, double payment) {
    if (p == nullptr) return false;

    // Add upfront payment to revenue
    addRevenue(payment);

    // Determine which queue to use based on priority
    Parcel** targetHead = p->getPriority() ? &expressHead : &standardHead;

    // Insert at the end of the Linked List (FIFO Queue logic)
    if (*targetHead == nullptr) {
        *targetHead = p; // List is empty, new parcel becomes the head
    } else {
        Parcel* current = *targetHead;
        while (current->getNext() != nullptr) {
            current = current->getNext();
        }
        current->setNext(p); // Link at the tail
    }
    
    // Ensure the new tail points to null
    p->setNext(nullptr); 
    
    return true;
}

// --- Core Behavior: Load Postman (The Memory Transfer) ---
void PostOffice::loadPostman(Postman& postman) {
    // We must try to load Express parcels first, then Standard parcels.
    // We use an array of pointers to process both queues with the same logic.
    Parcel** queues[] = { &expressHead, &standardHead };
    
    for (int i = 0; i < 2; i++) {
        Parcel** currentHead = queues[i];
        Parcel* current = *currentHead;
        Parcel* prev = nullptr;

        while (current != nullptr) {
            // Attempt to give the parcel to the Postman
            if (postman.takeParcel(current)) {
                // Postman ACCEPTED! We must detach it from the PostOffice list.
                
                if (prev == nullptr) {
                    // Detaching from the Head (Step 2 from our Memory Map!)
                    *currentHead = current->getNext();
                } else {
                    // Detaching from the middle of the list
                    prev->setNext(current->getNext());
                }
                
                Parcel* transferredParcel = current;
                current = current->getNext(); // Move iterator forward safely
                
                // Clean the detached node's pointer so it doesn't drag the rest of the list with it
                transferredParcel->setNext(nullptr); 
                
            } else {
                // Postman REJECTED (Limits reached). Move to the next parcel.
                prev = current;
                current = current->getNext();
            }
        }
    }
}