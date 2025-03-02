#include "Queue.h"
#include <iostream>

// Constructor to initialize an empty queue
Queue::Queue() {
    front = -1;
    rear = -1;
}

// Adds a province to the end of the queue
void Queue::enqueue(int province) {
  	if(province < 0 || province >= MAX_QUEUE_SIZE){
        std::cout << "Invalid province" << std::endl;
        return;
    }
    if((rear+1)%MAX_QUEUE_SIZE == front){
        return;
    }
    if(front == -1){
        front = 0;
    }
    rear =(rear+1)%MAX_QUEUE_SIZE;
    data[rear] = province;
}

// Removes and returns the front province from the queue
int Queue::dequeue() {
    if(front != -1){
    int temp = data[front];
    data[front] = 0;
    if(front == rear){
        front = -1;
        rear = -1;
    }
    else {
        front = (front+1)%MAX_QUEUE_SIZE;
    }
    return temp;
    }
    return -1;
}

// Returns the front province without removing it
int Queue::peek() const {
    if(front != -1){
        return data[front];
    }
    return -1;
}

// Checks if the queue is empty
bool Queue::isEmpty() const {
    if(front == -1){
        return true;
    }
    return false;
}

// Add a priority neighboring province in a way that will be dequeued and explored before other non-priority neighbors
void Queue::enqueuePriority(int province) {
    if(province < 0 || province >= MAX_QUEUE_SIZE){
        return;
    }
  	if((rear+1)%MAX_QUEUE_SIZE == front){
        return;
    }
  	if(front == -1){
        front = 0;
        rear = 0;
    }
    else if(front == 0){
        front = MAX_QUEUE_SIZE-1;
    }
    else{
        front = front-1;
    }
    data[front] = province;
}

