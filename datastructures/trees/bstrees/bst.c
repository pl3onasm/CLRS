/* Binary search tree implementation, including the necessary memory
   management functions. */

#include "bst.h"

//::::::::::::::::::::::: memory management :::::::::::::::::::::::://

void *safeCalloc (int n, int size) {
  /* allocates n elements of size size, initializing them to 0, and
     checks whether the allocation was successful */
  void *ptr = calloc(n, size);
  if (ptr == NULL) {
    printf("Error: calloc(%d, %d) failed. Out of memory?\n", n, size);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

student *newStudent () {
  /* allocates memory for a new student record */
  student *s = safeCalloc(1, sizeof(student));
  return s;
}

node *newNode (student *s) {
  /* allocates memory for a new node */
  node *n = safeCalloc(1, sizeof(node));
  n->student = s;
  return n;
}

bst *newBST (void) {
  /* allocates memory for a new BST */
  bst *tree = safeCalloc(1, sizeof(bst));
  return tree;
}

void freeNode (node *n) {
  /* frees a node */
  free(n->student);
  free(n);
}

void freeNodes (node *x) {
  /* frees all nodes in the subtree rooted at x */
  if (x != NULL) {
    freeNodes(x->left);
    freeNodes(x->right);
    freeNode(x);
  }
}

void freeBST (bst *tree) {
  /* entirely frees a binary search tree */
  if (tree != NULL)
    freeNodes(ROOT);
  free(tree);
}

//::::::::::::::::::::::::: BST operations :::::::::::::::::::::::::://

void BSTinsert (bst *tree, student *s) {
  /* inserts a student record into the BST */
  node *n = newNode(s);
  node *y = NULL;
  node *x = ROOT;

  while (x != NULL) {
    y = x;
    if (s->id < x->student->id)
      x = x->left;
    else
      x = x->right;
  }

  n->parent = y;
  if (y == NULL)
    ROOT = n;
  else if (s->id < y->student->id)
    y->left = n;
  else
    y->right = n;
}

node *BSTsearch (node *x, int id) {
  /* searches for a student record in the BST */
  if (x == NULL || id == x->student->id)
    return x;
  if (id < x->student->id)
    return BSTsearch(x->left, id);
  else
    return BSTsearch(x->right, id);
}

void BSTtransplant (bst *tree, node *u, node *v) {
  /* replaces node u with node v in the BST */
  if (u->parent == NULL)
    ROOT = v;
  else if (u == u->parent->left)
    u->parent->left = v;
  else
    u->parent->right = v;
  if (v != NULL)
    v->parent = u->parent;
}

node *BSTminimum (node *x) {
  /* returns the node with the smallest key in 
  the subtree rooted at x */
  while (x->left != NULL)
    x = x->left;
  return x;
}

void BSTdelete (bst *tree, node *z) {
  /* deletes a node from the BST */
  if (z->left == NULL)
    BSTtransplant(tree, z, z->right);
  else if (z->right == NULL)
    BSTtransplant(tree, z, z->left);
  else {
    node *y = BSTminimum(z->right);
    if (y->parent != z) {
      BSTtransplant(tree, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    BSTtransplant(tree, z, y);
    y->left = z->left;
    y->left->parent = y;
  }
  freeNode(z);
}

//::::::::::::::::::::::::: print functions ::::::::::::::::::::::::://

void printStudent (student *s) {
  /* prints a student record */
  printf("%d | %s | %.2lf | %s %s\n", s->id, s->dob, s->gpa, s->fname, s->lname);
}

void printBST (node *x, short *count) {
  /* prints the student records in order of student ID, 20 at a time */
  char buffer[1024], c;
  if (x != NULL) {
    printBST(x->left, count);
    if (*count < 20){
      printStudent(x->student);
      *count += 1;
    } else if (*count == 20){
      printf("Print 20 more? (y/n): ");
      if ((fgets (buffer, 1024, stdin) && 
      sscanf(buffer, "%c", &c) != 1) || c != 'y')
        *count = 21;
      else
        *count = 0;
    }
    printBST(x->right, count);
  }
}