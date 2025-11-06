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
private:
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

class Queue{ // Queue for callers in order (waiting to be serviced).
private:
    LinkedList list; // Linked list is provided...
public:
    bool isEmpty() { return list.size() == 0; } // Checks if the size of the list is 0 (empty) using size function.

    void enqueue(int value) {
        list.append(value); // Enqueues by appending node by value.
    }

    int dequeue() { 

        if (list.size() == 0) {  // Case for empty list.
            cout << "Queue is empty.\n"; 
            return -1; 
        } 

        Node* front = list.iterate(0); // Temporary pointer to point to head.
        int value = front->data; // Stores head data into temporary variable.
        list.remove(value); // Uses remove function to erase node with given nodes value.
        return value; // Returns removed value.
    }

    void display() {
        list.display(); // Just uses display function from LinkedList class.
    }
    void peek() {
        Node* temp = list.iterate(0);
        cout << "Value at head of queue: " << temp->data << endl;
    }
};

class Stack{ // Stack for returning missed calls. Mostly the same logic.
private:
    LinkedList list; 
public:
    bool isEmpty() {
        return list.size() == 0;
    }

    void peek() { // Peeks top of list.
        Node* temp = list.iterate(0); // Takes a peek at the head of the list (which is the top of the stack)
        cout << "Value at top of stack: " << temp->data << endl; // and dislays it.
    }

    void push(int value) {
        list.prepend(value); // Adds value to the top of stack, at the head.
    }

    int pop() {
        if (list.size() == 0) { // Case for empty list.
            cout << "Stack is empty.\n";
                return -1;
        }
        Node* front = list.iterate(0); // Pointer to head node.
        int value = front->data; // Temporary variable to contain popped data from head.
        list.remove(value); // Same with removing with queue before.
        return value; // Returns popped value.
    }

    void display() {
        list.display();
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
        if (c.state == "waiting") { waiting.enqueue(c.serviceTime); } // Push to queue if contains tag "waiting".

        else if (c.state == "missed") // Push to stack if contains tag "missed".
            missed.push(c.serviceTime);
    }
    file.close();

    cout << "Initial waiting queue (service times): ";
    waiting.display();
    cout << "Initial missed call stack (service times): ";
    missed.display();

    cout << '\n';

    // We want a while loop that goes through both waiting and missed until both are empty (served). The order needed is to serve 3 waiting calls, then 1 missed.
    while (!waiting.isEmpty() || !missed.isEmpty()) { 
        // We serve 3 calls, and then 1 missed.
        for (int i = 0; i < 3; i++) { // Loop to serve 3 waiting callers.

            int time = waiting.dequeue(); // Temp variable to hold time for current displayed caller.

            if (time == -1) // Fix for weird bug that prints that it served customer with time -1. Basically checks if the queue is empty again
                break;      // and breaks the service loop if it is (so that it doesn't go into the negatives)

            cout << "Served customer with service time : " << time << endl;
        }
            if (!missed.isEmpty()) { // Serves one missed call.
                int time = missed.pop(); // Same temp variable for display.
                cout << "Called back missed caller with service time: " << time << endl;
            }

    }
    
    
}
