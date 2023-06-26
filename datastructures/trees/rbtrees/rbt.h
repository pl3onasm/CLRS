#ifndef RBT_H
#define RBT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// constants and macros
#define MAX_NAME_LEN 30
#define NIL   tree->nil
#define ROOT  tree->root

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
  enum {RED, BLACK} color;
  struct node *parent;
  struct node *left;
  struct node *right;
} node;

typedef struct {
  node *root, *nil;
} rbt; 

typedef enum {
  false = 0,
  true = 1
} bool;

// function prototypes
student *newStudent (void);
rbt *newRBT (void);
void RBTinsert (rbt *tree, student *s);
void freeRBT (rbt *tree);
node *RBTminimum (rbt *tree, node *x);
node *RBTsearch (rbt *tree, int id);
void printStudent (student *s);
void printRBT (rbt *tree, node *root, short *count);
void RBTdelete (rbt *tree, node *z);

#endif