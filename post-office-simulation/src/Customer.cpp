#include "Customer.h"
#include "PostOffice.h" // Needed to interact with the Post Office
#include "Postman.h"    // Needed to interact with the Postman
#include <iostream>
#include <cstring>
#include <utility>

using namespace std;

// --- Constructor ---
Customer::Customer(const char* n, double initialMoney) {
    wallet = initialMoney;
    name = new char[strlen(n) + 1];
    strcpy(name, n);
}

// --- Destructor ---
Customer::~Customer() {
    delete[] name; // Prevents memory leaks
}

// --- Copy Constructor ---
Customer::Customer(const Customer& other) {
    wallet = other.wallet;
    name = new char[strlen(other.name) + 1];
    strcpy(name, other.name);
}

// --- Assignment Operator (Copy-and-Swap) ---
Customer& Customer::operator=(Customer other) {
    std::swap(name, other.name);
    std::swap(wallet, other.wallet);
    return *this;
}

// --- Getters ---
double Customer::getWallet() const { return wallet; }
const char* Customer::getName() const { return name; }

// --- Core Behavior: Sending a Parcel ---
bool Customer::sendParcel(PostOffice& po, int id, double w, double vol, bool express, PaymentMode mode, const char* dest) {
    // 1. Ask the Post Office for the shipping fee
    double fee = po.calculateFee(w, vol, express);
    
    // 2. Check financial constraints based on Payment Mode
    if (mode == SENDER_PAYS) {
        if (wallet < fee) {
            cout << "[Denied] " << name << " cannot afford the upfront fee of $" << fee << ".\n";
            return false; // Transaction aborted
        }
        wallet -= fee; // Deduct money from sender
    }
    
    // 3. Dynamic Memory Allocation: Creating the Parcel in Heap memory
    Parcel* newParcel = new Parcel(id, w, vol, express, mode, dest);
    
    // 4. If receiver pays, attach the fee to the parcel
    if (mode == RECEIVER_PAYS) {
        newParcel->setPendingFee(fee);
    }
    
    // 5. Transfer pointer ownership to the Post Office
    // Assuming SENDER_PAYS means the fee goes to PostOffice revenue immediately
    double upfrontPayment = (mode == SENDER_PAYS) ? fee : 0.0;
    
    if (po.processAndStoreParcel(newParcel, upfrontPayment)) {
        cout << "[Success] " << name << " dispatched Parcel ID: " << id << ".\n";
        return true;
    } else {
        // If PostOffice rejects it for any reason, we must free the memory to avoid leaks
        delete newParcel;
        return false;
    }
}

// --- Core Behavior: Receiving a Parcel ---
bool Customer::receiveParcel(Parcel* p, Postman& pm) {
    if (p == nullptr) return false;

    // 1. Check if the receiver needs to pay
    if (p->getPaymentMode() == RECEIVER_PAYS) {
        double requiredFee = p->getPendingFee();
        
        if (wallet < requiredFee) {
            cout << "[Delivery Failed] " << name << " has insufficient funds to receive Parcel ID: " 
                 << p->getTrackingID() << " (Needs $" << requiredFee << ").\n";
            return false; // Parcel stays with the Postman
        }
        
        // Deduct money and pay the postman
        wallet -= requiredFee;
        pm.receiveCash(requiredFee);
        p->clearPendingFee();
        cout << "[Paid] " << name << " paid $" << requiredFee << " to the Postman.\n";
    }
    
    // 2. Delivery successful. The parcel's lifecycle ends here.
    cout << "[Delivered] " << name << " received Parcel ID: " << p->getTrackingID() << ".\n";
    
    // CRITICAL: Memory deallocation. The object is destroyed.
    delete p; 
    
    return true;
}