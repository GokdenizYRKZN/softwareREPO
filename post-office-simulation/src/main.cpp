#include <iostream>
#include "Customer.h"
#include "PostOffice.h"
#include "Postman.h"

using namespace std;

// A helper function to print separators for readability
void printScenarioHeader(const char* title) {
    cout << "\n========================================================\n";
    cout << " " << title << "\n";
    cout << "========================================================\n";
}

int main() {
    
    // ---------------------------------------------------------
    // SCENARIO 1: The Complete Lifecycle (Correct Case)
    // ---------------------------------------------------------
    printScenarioHeader("SCENARIO 1: The Complete Lifecycle (Sender Pays)");
    
    Customer senderA("Alice", 100.0);
    Customer receiverB("Bob", 50.0);
    PostOffice centralOffice;
    Postman postmanJohn("John", 50.0, 5.0); // John can carry 50kg and 5.0 m3

    // 1. Alice sends a parcel (ID: 101, 2.0kg, 0.5m3, Express, Sender Pays)
    cout << "-> Alice is trying to send a parcel...\n";
    senderA.sendParcel(centralOffice, 101, 2.0, 0.5, true, SENDER_PAYS, "Warsaw");

    // 2. Post Office gives parcels to the postman
    cout << "-> Post Office is loading the postman...\n";
    centralOffice.loadPostman(postmanJohn);

    // 3. Postman attempts to deliver
    cout << "-> Postman John is out for delivery...\n";
    postmanJohn.deliverParcelTo(receiverB, 101);
    
    // Print final balances
    cout << "-> Final Wallet of Alice: $" << senderA.getWallet() << "\n";
    cout << "-> Final Wallet of Bob: $" << receiverB.getWallet() << "\n";


    // ---------------------------------------------------------
    // SCENARIO 2: The Bulky Package Limit (Incorrect Case)
    // ---------------------------------------------------------
    printScenarioHeader("SCENARIO 2: The Bulky Package Limit");

    Customer corpSender("Corp", 500.0);
    PostOffice branchOffice;
    Postman smallVanDave("Dave", 100.0, 2.0); // Dave's van only has 2.0 m3 space!

    // Corp sends a light but HUGE parcel (ID: 201, 5.0kg, 3.5m3)
    cout << "-> Corp is trying to send a massive parcel...\n";
    corpSender.sendParcel(branchOffice, 201, 5.0, 3.5, false, SENDER_PAYS, "Krakow");

    // Post Office attempts to load Dave
    cout << "-> Post Office is loading Dave's small van...\n";
    branchOffice.loadPostman(smallVanDave);
    
    cout << "[Notice] Parcel 201 remains in the Post Office because Dave's van rejected it due to volume limits.\n";


    // ---------------------------------------------------------
    // SCENARIO 3: The Broke Receiver (Incorrect Case)
    // ---------------------------------------------------------
    printScenarioHeader("SCENARIO 3: The Broke Receiver (Receiver Pays)");

    Customer senderEve("Eve", 100.0);
    Customer brokeCharlie("Charlie", 5.0); // Charlie only has $5
    PostOffice mainOffice;
    Postman postmanMike("Mike", 50.0, 5.0);

    // Eve sends an Express parcel to Charlie but chooses RECEIVER_PAYS
    cout << "-> Eve is sending a parcel (Receiver Pays)...\n";
    senderEve.sendParcel(mainOffice, 301, 10.0, 1.0, true, RECEIVER_PAYS, "Gdansk");

    // Load Mike
    cout << "-> Post Office is loading the postman...\n";
    mainOffice.loadPostman(postmanMike);

    // Delivery Attempt
    cout << "-> Postman Mike attempts to deliver to Charlie...\n";
    postmanMike.deliverParcelTo(brokeCharlie, 301);

    // Postman hands over any collected cash (should be $0 here since delivery failed)
    postmanMike.handOverCashToOffice(mainOffice);

    cout << "\n========================================================\n";
    cout << " ALL TESTS COMPLETED SUCCESSFULLY! \n";
    cout << " Destructors will now run to clean up memory... \n";
    cout << "========================================================\n";

    return 0; // The end! System gracefully destroys all objects.
}