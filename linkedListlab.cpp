#include <iostream>
#include <fstream>
//Implement Stack and Queue using provided Linked List methods only.
using namespace std;

struct customer { // Struct for individual customer's call information.
    string name;
    string state; // Contains 3 types of data for reading from file : Name, state, and service time.
    int serviceTime;
};

class Node {
public:
    int data;
    Node* next;
    Node(int value) {
        data = value;
        next = nullptr;
    }
};
class LinkedList {
protected:
    Node* head;
public:
    LinkedList() {
        head = nullptr;
    }
    ~LinkedList() {
        Node* current = head;
        while (current != nullptr) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }
    bool isEmpty() { return head == nullptr; }

    // Iterate to a given position (0-based index)
    Node* iterate(int position) {
        if (position < 0) return nullptr;
        Node* current = head;
        int index = 0;
        while (current != nullptr && index < position) {
            current = current->next;
            index++;
        }
        return current; // nullptr if not found
    }
    // Insert at position
    void insertAt(int position, int value) {
        Node* newNode = new Node(value);
        if (position <= 0 || head == nullptr) { // Insert at beginning
            newNode->next = head;
            head = newNode;
            return;
        }
        Node* prev = iterate(position - 1);
        if (prev == nullptr) { // Position too big, append at end
            append(value);
            delete newNode; // already appended, avoid memory leak
            return;
        }
        newNode->next = prev->next;
        prev->next = newNode;
    }
    // Prepend (beginning)
    void prepend(int value) {
        insertAt(0, value);
    }
    // Append (end)
    void append(int value) {
        if (head == nullptr) {
            head = new Node(value);
            return;
        }
        Node* last = iterate(size() - 1);
        last->next = new Node(value);
    }
    // Find node by value
    Node* find(int value) {
        Node* temp = head;
        while (temp != nullptr) {
            if (temp->data == value)
                return temp;
            temp = temp->next;
        }
        return nullptr;
    }
    // Contains (true/false)
    bool contains(int value) {
        return find(value) != nullptr;
    }
    // Remove by value
    bool remove(int value) {
        if (head == nullptr) return false;
        // Special case: head
        if (head->data == value) {
            Node* temp = head;
            head = head->next;
            delete temp;
            return true;
        }
        // Find previous node of the target
        int index = 0;
        Node* current = head;
        while (current != nullptr && current->next != nullptr) {
            if (current->next->data == value) {
                Node* target = current->next;
                current->next = target->next;
                delete target;
                return true;
            }
            current = current->next;
            index++;
        }
        return false; // Not found
    }
    // Get size
    int size() {
        int count = 0;
        Node* temp = head;
        while (temp != nullptr) {
            count++;
            temp = temp->next;
        }
        return count;
    }
    // Display
    void display() {
        Node* temp = head;
        while (temp != nullptr) {
            cout << temp->data << " -> ";
            temp = temp->next;
        }
        cout << "NULL" << endl;
    }
};

class Queue : public LinkedList{ // Queue for callers in order (waiting to be serviced).
public:

    void enqueue(int value) {
        append(value); // Enqueues by appending node by value.
    }

    int dequeue() { 
        if (head == nullptr) {  // Case for empty list.
            cout << "Queue is empty.\n"; 
            return -1; 
        } 

        int value = head->data; // Temporary variable to contain removed data.
        remove(value); // Uses remove function to erase node with given nodes value.
        return value; // Returns removed value.
    }
};

class Stack : public LinkedList{ // Stack for returning missed calls.
public:

    void push(int value) {
        prepend(value); // Adds value to the top of stack, at the head.
    }

    int pop() {
        if (head == nullptr) { // Case for empty list.
            cout << "Stack is empty.\n";
                return -1;
        }
        int value = head->data; // Temporary variable to contain popped data.
        remove(value); // Same with removing with queue before.
        return value; // Returns popped value.
    }
};

int main() {
    Queue waiting; // Queue for waiting callers.
    Stack missed; // Stack for missed callers.

    ifstream file("calls.txt");
    if (!file) {
        cout << "File not found.\n";
        return 1;
    }
    customer c; // Customer struct to store each customer.

    while (file >> c.name >> c.state >> c.serviceTime) { // Extracting data from file into struct.
        if (c.state == "waiting") { waiting.enqueue(c.serviceTime); } // Separating between missing and waiting calls through tags in the txt file.
        else if (c.state == "missed")
            missed.push(c.serviceTime);
    }
    file.close();

    cout << "Initial waiting queue (service times): ";
    waiting.display();
    cout << "Initial missed call stack (service times): ";
    missed.display();

    // After serving 3 calls...
    for (int i = 0; i < 3 && !waiting.isEmpty(); i++) {

        int time = waiting.dequeue(); // Temp variable to hold time for current displayed caller.
        cout << "Served customer with service time: " << time << endl;

    }
    
    if (!missed.isEmpty()) { // No loop needed cause its only one call.
        int time = missed.pop(); // Same temp variable for dispaly.
        cout << "Called back missed caller with service time: " << time << endl;
    }
    cout << "\n Remaining in waiting queue: ";
    waiting.display();
    cout << " Remaining missed callers: ";
    missed.display();
}
