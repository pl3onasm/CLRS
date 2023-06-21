#ifndef BST_H
#define BST_H

#include <stdio.h>
#include <stdlib.h>

// constants and macros
#define MAX_NAME_LEN 30
#define ROOT   tree->root

typedef short  bool;
#define true   1
#define false  0

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
  struct node *parent;
  struct node *left;
  struct node *right;
} node;

typedef struct {
  node *root;
} bst;

// function prototypes
student *newStudent (void);
bst *newBST (void);
void insert (bst *tree, student *s);
void freeBST (bst *tree);
node *search (node *x, int id);
void delete (bst *tree, node *z);
node *minimum (node *x);
void printStudent (student *s);
void printInorder (node *x, short *count);

#endif