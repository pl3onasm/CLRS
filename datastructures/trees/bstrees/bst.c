/* 
  Generic binary search tree implementation 
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#include "bst.h"
#include "../../../lib/clib/clib.h"

//::::::::::::::::::::::: Memory management :::::::::::::::::::::::://

node *newNode (void *data) {
  // allocates memory for a new node
  node *n = safeCalloc(1, sizeof(node));
  n->data = data;
  return n;
}

tree *newTree () {
  // allocates memory for a new tree
  tree *tree = safeCalloc(1, sizeof(tree));
  return tree;
}

void freeNode (node *n) {
  // frees a node 
  if (n != NULL){
    free(n->data);
    free(n);
  }
}

void freeNodes (node *x) {
  // frees all nodes in the subtree rooted at x 
  if (x != NULL) {
    freeNodes(x->left);
    freeNodes(x->right);
    freeNode(x);
  }
}

void freeTree (tree *T) {
  // entirely frees a tree
  if (T != NULL)
    freeNodes(ROOT);
  free(T);
}

//:::::::::::::::::::::::: Tree operations ::::::::::::::::::::::::://

void insertNode (tree *T, node *n, int (*cmp)(void *, void *)) {
  // inserts a node into the tree, using the given comparison function
  node *y = NULL;
  node *x = ROOT;

  while (x != NULL) {
    y = x;
    if (cmp(n->data, x->data) < 0)
      x = x->left;
    else
      x = x->right;
  }

  n->parent = y;
  if (y == NULL)
    ROOT = n;
  else if (cmp(n->data, y->data) < 0)
    y->left = n;
  else
    y->right = n;
}

node *searchKey (node *x, void *key, int (*cmp)(void *, void *)) {
  // searches for a key in the tree 
  if (x == NULL || cmp(x->data, key) == 0)
    return x;
  if (cmp(key, x->data) < 0)
    return searchKey(x->left, key, cmp);
  else
    return searchKey(x->right, key, cmp);
}

void transplant (tree *T, node *u, node *v) {
  // replaces node u with node v in the tree 
  if (u->parent == NULL)
    ROOT = v;
  else if (u == u->parent->left)
    u->parent->left = v;
  else
    u->parent->right = v;
  if (v != NULL)
    v->parent = u->parent;
}

node *treeMinimum (node *x) {
  // returns the node with the smallest key in 
  // the subtree rooted at x 
  while (x->left != NULL)
    x = x->left;
  return x;
}

void deleteNode (tree *T, node *z) {
  // deletes a node from the tree 
  if (z->left == NULL)
    transplant(T, z, z->right);
  else if (z->right == NULL)
    transplant(T, z, z->left);
  else {
    node *y = treeMinimum(z->right);
    if (y->parent != z) {
      transplant(T, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    transplant(T, z, y);
    y->left = z->left;
    y->left->parent = y;
  }
  freeNode(z);
}

//::::::::::::::::::::::::::: Printing ::::::::::::::::::::::::::::://

void printTree (node *x, short *count, void (*printData)(void *)) {
  // prints the data in the tree in order, 20 items at a time 
  char buffer[100], c;
  if (x != NULL) {
    printTree(x->left, count, printData);
    if (*count < 20){
      printData(x->data);
      *count += 1;
    } else if (*count == 20){
      printf("Print 20 more? (y/n): ");
      if ((fgets (buffer, 100, stdin) && 
      sscanf(buffer, "%c", &c) != 1) || c != 'y')
        *count = 21;
      else
        *count = 0;
      clearStdin(buffer);
    }
    printTree(x->right, count, printData);
  }
}

void printNode (node *n, void (*printData)(void *)) {
  // prints the data in a node 
  if (n != NULL)
    printData(n->data);
}

//:::::::::::::::::::::::: File operations ::::::::::::::::::::::::://

void writeTreeToFile (node *x, FILE *fp, void (*printData)(void *, FILE *)) {
  // writes the data in the tree in order to given file 
  if (x != NULL) {
    writeTreeToFile(x->left, fp, printData);
    printData(x->data, fp);
    writeTreeToFile(x->right, fp, printData);
  }
}

void buildTreeFromFile (tree *T, char *filename, size_t dataSize,
  int (*cmp)(void *, void *), bool (*dataFromStr)(void *, char *)) {
  // reads data from input file and inserts it into the tree
  FILE *fp; char buffer[100];
  size_t lineNr = 0;

  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Error: could not open file %s\n", filename);
    freeTree(T);
    exit(EXIT_FAILURE);
  }

  while (fgets(buffer, 100, fp) != NULL) {
    lineNr++;
    void *data = safeCalloc(1, dataSize);
    if (! dataFromStr(data, buffer)) {
      printf("Error: invalid input data on line %lu.\n"
             "Check file %s for errors and try again.\n", lineNr, filename);
      free(data);
      freeTree(T);
      fclose(fp);
      exit(EXIT_FAILURE);
    }
    node *n = newNode(data);
    insertNode(T, n, cmp);
  }
  fclose(fp);
}
