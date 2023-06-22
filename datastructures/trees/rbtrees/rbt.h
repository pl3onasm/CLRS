#ifndef RBT_H
#define RBT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// constants and macros
#define MAX_NAME_LEN 30
#define RED   'R'
#define BLACK 'B'
#define NIL   tree->nil
#define ROOT  tree->root
typedef short bool;
#define true 1
#define false 0

// data structures
typedef struct {
  int id;
  double gpa;
  char dob[11];
  char fname[MAX_NAME_LEN];
  char lname[MAX_NAME_LEN];
} student;

typedef struct node {
  student *student;
  char color;
  struct node *parent;
  struct node *left;
  struct node *right;
} node;

typedef struct {
  node *root, *nil;
} rbt; 


// function prototypes
student *newStudent (void);
rbt *newRBT (void);
void insert (rbt *tree, student *s);
void freeRBT (rbt *tree);
node *treeMinimum (node *x);
node *search (rbt *tree, int id);
void insert (rbt *tree, student *s);
void printStudent (student *s);
void printRBT (rbt *tree, node *root, short *count);
void delete (rbt *tree, node *z);

#endif