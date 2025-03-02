#include "Stack.h"
#include <iostream>

// Constructor to initialize an empty stack
Stack::Stack() {
    top = -1;
}

// Adds a province to the top of the stack
void Stack::push(int province) {
    if(top < MAX_SIZE-1){
        top++;
        data[top] = province;}
}

// Removes and returns the top province from the stack
int Stack::pop() {
    if(top !=-1){
        int temp = data[top];
        data[top] = 0;
        top--;
        return temp;
    }
    return -1;
}

// Returns the top province without removing it
int Stack::peek() const {
    if(top !=-1){
        return data[top];
    }
    return -1;
}

// Checks if the stack is empty
bool Stack::isEmpty() const {
    if(top ==-1){
        return true;
    }
    return false;
}

// Function to get the current size of the stack
int Stack::getSize() const {
    if(top !=-1){
        return top+1;
    }
    return 0;
}
