/* 
  Generic queue, implemented as a circular buffer.
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#include <stdbool.h>
#include <stdlib.h>

typedef void (*showQueueItem)(void const *item);
typedef void (*freeQueueItem)(void *item);
typedef void *(*copyQueueItem)(void const *item);

typedef struct {
  void **buffer;        // Array of void pointers
  size_t capacity;      // Capacity of the queue
  size_t size;          // Number of elements in the queue
  size_t head;          // Index of the head of the queue
  size_t tail;          // Index of the tail of the queue
  showQueueItem show;   // Function to show an item
  freeQueueItem free;   // Function to free an item
  copyQueueItem copy;   // Function to copy an item
  char *delim;          // Delimiter for showing items 
                        // (default: ", ")
} queue;


  // Creates a new queue with given capacity
queue *newQueue(size_t capacity);

  // Sets queue to own the input data, deallocating
  // what is still in the queue when it is destroyed
void setQueueOwner(queue *Q, freeQueueItem free);

  // Sets queue to operate on copies of the input data
void setQueueCopy(queue *Q, copyQueueItem copy, 
                  freeQueueItem free);

  // Sets the function to show an item
void setQueueShow(queue *Q, showQueueItem show);

  // Sets the string delimiter for showing items
void setQueueDelim(queue *Q, char *delim);

  // Destroys the queue
void freeQueue(queue *Q);

  // Enqueues an element
void enqueue(queue *Q, void *element);

  // Dequeues an element
void *dequeue(queue *Q);

  // Peeks at the head of the queue
void *peekHead(queue *Q);

  // Peeks at the tail of the queue
void *peekTail(queue *Q);

  // Shows the queue
void showQueue(queue *Q);


  // True if the queue is empty
static inline bool isEmptyQueue(queue *Q) {
  return Q->head == Q->tail;
}

#endif  // QUEUE_H_INCLUDED