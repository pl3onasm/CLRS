/* Implementation of red-black trees, with nodes holding student records */
#include "rbt.h"

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

node *newNode (student *s, rbt *tree) {
  /* allocates memory for a new node */
  node *n = safeCalloc(1, sizeof(node));
  n->student = s;
  n->color = RED;
  n->parent = n->left = n->right = NIL;
  return n;
}

rbt *newRBT (void) {
  /* allocates memory for a new RBT */
  rbt *tree = safeCalloc(1, sizeof(rbt));
  NIL = newNode(NULL, tree);
  NIL->color = BLACK;
  ROOT = NIL;
  return tree;
}

void freeNode (node *n) {
  /* frees a node */
  free(n->student);
  free(n);
}

void freeNodes (node *x, rbt *tree) {
  /* frees all nodes in the subtree rooted at x */
  if (x != NIL) {
    freeNodes(x->left, tree);
    freeNodes(x->right, tree);
    freeNode(x);
  }
}

void freeRBT (rbt *tree) {
  /* entirely frees a binary search tree */
  freeNodes(ROOT, tree);
  free(NIL);
  free(tree);
}


//::::::::::::::::::::::::: RBT operations :::::::::::::::::::::::::://

node *RBTminimum (rbt *tree, node *x) {
  /* returns the node with the smallest key in the subtree rooted at x */
  while (x->left != NIL)
    x = x->left;
  return x;
}

node *RBTsearch (rbt *tree, int id) {
  /* searches for a student record in the RBT */
  node *x = ROOT;
  while (x != NIL && x->student->id != id) {
    if (id < x->student->id)
      x = x->left;
    else
      x = x->right;
  }
  return x;
}

void leftRotate (rbt *tree, node *x) {
  /* performs a left rotation on the subtree rooted at x */
  node *y = x->right;
  x->right = y->left;
  if (y->left != NIL)
    y->left->parent = x;
  y->parent = x->parent;
  if (x->parent == NIL)
    ROOT = y;
  else if (x == x->parent->left)
    x->parent->left = y;
  else 
    x->parent->right = y;
  y->left = x;
  x->parent = y;
}

void rightRotate (rbt *tree, node *x) {
  /* performs a right rotation on the subtree rooted at x */
  node *y = x->left;
  x->left = y->right;
  if (y->right != NIL)
    y->right->parent = x;
  y->parent = x->parent;
  if (x->parent == NIL)
    ROOT = y;
  else if (x == x->parent->right)
    x->parent->right = y;
  else 
    x->parent->left = y;
  y->right = x;
  x->parent = y;
}

void insertFixup (rbt *tree, node *z) {
  /* restores the red-black properties after insertion of z */
  while (z->parent->color == RED) {
    if (z->parent == z->parent->parent->left) {
      node *y = z->parent->parent->right;
      if (y->color == RED) {
        z->parent->color = BLACK;
        y->color = BLACK;
        z->parent->parent->color = RED;
        z = z->parent->parent;
      } else {
        if (z == z->parent->right) {
          z = z->parent;
          leftRotate(tree, z);
        }
        z->parent->color = BLACK;
        z->parent->parent->color = RED;
        rightRotate(tree, z->parent->parent);
      }
    } else {
      node *y = z->parent->parent->left;
      if (y->color == RED) {
        z->parent->color = BLACK;
        y->color = BLACK;
        z->parent->parent->color = RED;
        z = z->parent->parent;
      } else {
        if (z == z->parent->left) {
          z = z->parent;
          rightRotate(tree, z);
        }
        z->parent->color = BLACK;
        z->parent->parent->color = RED;
        leftRotate(tree, z->parent->parent);
      }
    }
  }
  ROOT->color = BLACK;
}

void RBTinsert (rbt *tree, student *s) {
  /* inserts a student record into the RBT */
  node *z = newNode(s, tree);
  node *y = NIL;
  node *x = ROOT;
  while (x != NIL) {
    y = x;
    if (z->student->id < x->student->id)
      x = x->left;
    else
      x = x->right;
  }
  z->parent = y;
  if (y == NIL)
    ROOT = z;
  else if (z->student->id < y->student->id)
    y->left = z;
  else
    y->right = z;
  insertFixup(tree, z);
}

void RBTtransplant (rbt *tree, node *u, node *v) {
  /* replaces the subtree rooted at u with the subtree rooted at v */
  if (u->parent == NIL)
    ROOT = v;
  else if (u == u->parent->left)
    u->parent->left = v;
  else 
    u->parent->right = v;
  v->parent = u->parent;
}

void deleteFixup (rbt *tree, node *x) {
  /* restores the red-black properties after deletion */
  while (x != ROOT && x->color == BLACK) {
    if (x == x->parent->left) {
      node *w = x->parent->right;
      if (w->color == RED) {
        w->color = BLACK;
        x->parent->color = RED;
        leftRotate(tree, x->parent);
        w = x->parent->right;
      }
      if (w->left->color == BLACK && w->right->color == BLACK) {
        w->color = RED;
        x = x->parent;
      } else {
        if (w->right->color == BLACK) {
          w->left->color = BLACK;
          w->color = RED;
          rightRotate(tree, w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = BLACK;
        w->right->color = BLACK;
        leftRotate(tree, x->parent);
        x = ROOT;
      }
    } else {
      node *w = x->parent->left;
      if (w->color == RED) {
        w->color = BLACK;
        x->parent->color = RED;
        rightRotate(tree, x->parent);
        w = x->parent->left;
      }
      if (w->right->color == BLACK && w->left->color == BLACK) {
        w->color = RED;
        x = x->parent;
      } else {
        if (w->left->color == BLACK) {
          w->right->color = BLACK;
          w->color = RED;
          leftRotate(tree, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = BLACK;
        w->left->color = BLACK;
        rightRotate(tree, x->parent);
        x = ROOT;
      }
    }
  }
  x->color = BLACK;
}

void RBTdelete (rbt *tree, node *z) {
  /* deletes a student record from the RBT */
  node *y = z;
  node *x;
  char y_originalColor = y->color;
  if (z->left == NIL) {
    x = z->right;
    RBTtransplant(tree, z, z->right);
  } else if (z->right == NIL) {
    x = z->left;
    RBTtransplant(tree, z, z->left);
  } else {
    y = RBTminimum(tree, z->right);
    y_originalColor = y->color;
    x = y->right;
    if (y->parent == z)
      x->parent = y;
    else {
      RBTtransplant(tree, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    RBTtransplant(tree, z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }
  if (y_originalColor == BLACK)
    deleteFixup(tree, x);
  freeNode(z);
}

//::::::::::::::::::::::::: print functions ::::::::::::::::::::::::://

void printStudent (student *s) {
  /* prints a student record */
  printf("%d | %s | %.2lf | %s %s\n", s->id, s->dob, s->gpa, s->fname, s->lname);
}

void printRBT (rbt *tree, node *x, short *count) {
  /* prints the student records in order of student ID, 20 at a time */
  char buffer[1024], c;
  if (x != NIL) {
    printRBT(tree, x->left, count);
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
    printRBT(tree, x->right, count);
  }
}