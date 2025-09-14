/* 
  Generic Doubly Linked List (DLL) implementation
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/


#ifndef DLL_H_INCLUDED
#define DLL_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>  
#include <stdbool.h>

// function pointers for the DLL
typedef int (*dllCmpData)(void const *a, void const *b);
typedef void (*dllShowData)(void const *data);
typedef void *(*dllCpyData)(void const *data);
typedef void (*dllFreeData)(void *data);

// DLL data structures
typedef struct dllNode {
  void *dllData;          // pointer to the data
  struct dllNode *next;   // pointer to the next node
  struct dllNode *prev;   // pointer to the previous node
} dllNode;

typedef struct {
  dllNode *NIL;           // sentinel node
  dllNode *iter;          // iterator
  size_t size;            // size of the DLL
  dllCmpData cmp;         // comparison function
  dllShowData showData;   // show data function
  dllFreeData freeData;   // free data function
  dllCpyData copyData;    // copy data function
  char *delim;            // delimiter string
  char *label;            // label for the DLL
} dll;

// DLL function prototypes

  // Creates a new DLL 
dll *dllNew();

  // Sets the DLL to own the input data,
  // freeing it when the DLL is freed
void dllOwnData(dll *L, dllFreeData freeData);

  // Sets the DLL to make copies of the input data
  // If set, the DLL will only free the copies
void dllCopyData(dll *L, dllCpyData copyData, 
                 dllFreeData freeData);

  // Sets the delimiter string for the DLL
void dllSetDelim(dll *L, char *delim);

  // Sets the label for the DLL
void dllSetLabel(dll *L, char *label);

  // Sets the showData function for the DLL
void dllSetShow(dll *L, dllShowData showData);

  // Sets the comparison function for the DLL
void dllSetCmp(dll *L, dllCmpData cmp);

  // Frees the DLL
  // Input data is freed if the DLL owns it
void dllFree(dll *L);

  // Makes the DLL empty
void dllEmpty(dll *L);

  // Makes a copy of the DLL
dll *dllCopy(dll *L, dllCpyData copyData, 
             dllFreeData freeData);

  // Creates a new DLL node
dllNode *dllNewNode();

  // Pushes data to the front of the DLL
void dllPush(dll *L, void *data);

  // Inserts data in a sorted DLL
void dllInsert(dll *L, void *data);

  // Pushes data to the back of the DLL
void dllPushBack(dll *L, void *data);

  // Pops data from the front of the DLL
  // NULL is returned if the DLL is empty
void *dllPop(dll *L);

  // Pops data from the back of the DLL
  // NULL is returned if the DLL is empty
void *dllPopBack(dll *L);

  // Deletes the node at current iterator position
  // and moves the iterator to the next node
void dllDeleteCurr(dll *L);

  // Deletes a node from the DLL given its data
  // Returns true if the node was found and deleted
  // Returns false otherwise
bool dllDeleteData(dll *L, void *data);

  // Searches for a node in the DLL with a given key
  // returns the data of the node if found, NULL otherwise
void *dllFind(dll *L, void *key);

  // Searches for the next node in the DLL with a given key
  // starting from the current iterator position
  // returns the data of the node if found, NULL otherwise
void *dllFindNext(dll *L, void *key);

  // Shows the DLL using the showData function 
  // and the delimiter string (default is ", ")
void dllShow(dll *L);

  // Peeks at the data of the first node
  // NULL is returned if the DLL is empty
void *dllPeek(dll *L);

  // Peeks at the data of the last node
  // NULL is returned if the DLL is empty
void *dllPeekBack(dll *L);

  // Peeks at the data of the next node
  // NULL if the end of the DLL is reached
void *dllPeekNext(dll *L);

  // Peeks at the data of the previous node
  // NULL is the start of the DLL is reached
void *dllPeekPrev(dll *L);

  // Peeks at the data of the current node
  // NULL is returned if the DLL is empty
void *dllPeekCurr(dll *L);

  // Resets the list iterator
void dllResetIter(dll *L);

  // Sets the list iterator to the first node
void dllSetIterFirst(dll *L);

  // Sets the list iterator to the last node
void dllSetIterLast(dll *L);

  // Sets the list iterator to the first node
  // and returns the data of that node
void *dllFirst(dll *L);

  // Sets the list iterator to the last node
  // and returns the data of that node
void *dllLast(dll *L);

  // Returns true if the next node is the end of the DLL
bool dllEnd(dll *L);

  // Returns true if the previous node is the 
  // beginning of the DLL
bool dllStart(dll *L);

  // Updates the list iterator to the next node
  // and returns the data of that node
  // value is NULL if the end of the DLL is reached
void *dllNext(dll *L);

  // Updates the list iterator to the previous node
  // and returns the data of that node
  // NULL if the beginning of the DLL is reached
void *dllPrev(dll *L);

  // Checks if the DLL is empty
static inline bool dllIsEmpty (dll *L) {
  return L ? L->NIL->next == L->NIL : true;
}

  // Gets the size of the DLL
static inline size_t dllSize (dll *L) {
  return L ? L->size : 0;
}

  // Gets the dllData of a node
static inline void *dllData (dllNode *node) {
  return node ? node->dllData : NULL;
}

#endif // DLL_H_INCLUDED