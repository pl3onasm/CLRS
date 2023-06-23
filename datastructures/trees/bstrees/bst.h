#ifndef BST_H
#define BST_H

#include <stdio.h>
#include <stdlib.h>

// constants and macros
#define MAX_NAME_LEN 30
#define ROOT   tree->root

// data structures and types
typedef struct {
  int id;
  double gpa;
  char dob[11];
  char fname[MAX_NAME_LEN];
  char lname[MAX_NAME_LEN];
} student;

typedef struct node {
  student *student;
  struct node *parent;
  struct node *left;
  struct node *right;
} node;

typedef struct {
  node *root;
} bst;

typedef enum {
  false = 0,
  true = 1
} bool;

// function prototypes
student *newStudent (void);
bst *newBST (void);
void BSTinsert (bst *tree, student *s);
void freeBST (bst *tree);
node *BSTsearch (node *x, int id);
void BSTdelete (bst *tree, node *z);
node *BSTminimum (node *x);
void printStudent (student *s);
void printBST (node *x, short *count);

#endif