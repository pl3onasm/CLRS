/* file: rbtstudents.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: this is a simple example of a red-black tree
    using student records as data.
    The student records are stored in a file, one record per line.
    This file is read into a red-black tree, and the user can
    then search for a student by id, delete or insert a student,
    or print all existing records in the tree. 
    The tree is ordered by student number.
   time complexity: all operations are in O(h), where h is the height
    of the tree. Since the tree is always balanced, this is O(lg n),
    where n is the number of nodes in the tree.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 30
#define RED   'R'
#define BLACK 'B'
#define NIL   tree->nil
#define ROOT  tree->root
typedef short bool;
#define true 1
#define false 0

//:::::::::::::::::::::::: data structures ::::::::::::::::::::::::://

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

void freeNodes (node *x, node *nil) {
  /* frees all nodes in the subtree rooted at x */
  if (x != nil) {
    freeNodes(x->left, nil);
    freeNodes(x->right, nil);
    freeNode(x);
  }
}

void freeRBT (rbt *tree) {
  /* entirely frees a binary search tree */
  freeNodes(ROOT, NIL);
  free(NIL);
  free(tree);
}

//::::::::::::::::::::::::: RBT operations :::::::::::::::::::::::::://

node *treeMinimum (node *x) {
  /* returns the node with the smallest key in the subtree rooted at x */
  while (x->left != NULL)
    x = x->left;
  return x;
}

node *search (rbt *tree, int id) {
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

void insert (rbt *tree, student *s) {
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

void transplant (rbt *tree, node *u, node *v) {
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
  /* restores the red-black properties after deletion of x */
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

void delete (rbt *tree, node *z) {
  /* deletes a student record from the RBT */
  node *y = z;
  node *x;
  char y_originalColor = y->color;
  if (z->left == NIL) {
    x = z->right;
    transplant(tree, z, z->right);
  } else if (z->right == NIL) {
    x = z->left;
    transplant(tree, z, z->left);
  } else {
    y = treeMinimum(z->right);
    y_originalColor = y->color;
    x = y->right;
    if (y->parent == z)
      x->parent = y;
    else {
      transplant(tree, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    transplant(tree, z, y);
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
  printf("%d %s %.2lf %s %s\n", s->id, s->dob, s->gpa, s->fname, s->lname);
}

void printInorder (node *nil, node *x, short *count) {
  /* prints the student records in order of student number 20 at a time */
  char buffer[1024], c;
  if (x != nil) {
    printInorder(nil, x->left, count);
    if (*count < 20){
      printStudent(x->student);
      *count += 1;
    } else if (*count == 20){
      *count = 0;
      printf("Print 20 more? (y/n): ");
      if ((fgets (buffer, 1024, stdin) && 
      sscanf(buffer, "%c", &c) != 1) || c != 'y')
        return;
    }
    printInorder(nil, x->right, count);
  }
}

//::::::::::::::::::::::::: validation functions :::::::::::::::::::://

bool validDOB (char *dob) {
  /* checks if a date of birth is valid */
  int day, month, year;
  if (sscanf(dob, "%d-%d-%d", &year, &month, &day) != 3)
    return false;
  if (day < 1 || day > 31)
    return false;
  if (month < 1 || month > 12)
    return false;
  if (year < 1900 || year > 2017)
    return false;
  return true;
}

bool validStudent (student *s) {
  /* checks if a student record is valid */
  if (s->id < 1000000 || s->id > 9999999)
    return false;
  if (s->gpa < 0 || s->gpa > 10)
    return false;
  if (!validDOB(s->dob))
    return false;
  if (strlen(s->fname) < 2 || strlen(s->lname) < 2)
    return false;
  return true;
}

//:::::::::::::::::::::::::: file operations :::::::::::::::::::::::://

void writeToFile (node *nil, node *x, FILE *fp) {
  /* prints the student records in the RBT in order to given file */
  if (x != nil) {
    writeToFile(nil, x->left, fp);
    fprintf(fp, "%d %s %.2lf %s %s\n", x->student->id, x->student->dob,
            x->student->gpa, x->student->fname, x->student->lname);
    writeToFile(nil, x->right, fp);
  }
}

void readFromFile (rbt *tree, char *filename) {
  /* reads student records from input file and inserts them into the RBT */
  FILE *fp; char buffer[1024], c;
  student *s = newStudent();

  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Error: could not open file %s\n", filename);
    exit(EXIT_FAILURE);
  }

  while (fgets(buffer, 1024, fp) != NULL) {
    if (sscanf(buffer, "%d %s %lf %s %s %c", &s->id, s->dob, &s->gpa,
               s->fname, s->lname, &c) != 5 || !validStudent(s)) {
      fputs(buffer, stdout);
      printf("Error: invalid student record\n");
      exit(EXIT_FAILURE);
    }
    insert(tree, s);
    s = newStudent();
  }
  free(s);
  fclose(fp);
}

//::::::::::::::::::::::::::::: main :::::::::::::::::::::::::::::::://

int main (int argc, char *argv[]) {
  FILE *fp; node *n;
  char buffer[1024];
  short c, d, count = 0;

  if (argc != 2) {
    printf("Usage: %s <student records file>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  rbt *tree = newRBT();
  readFromFile(tree, argv[1]);

  while (true) {
    
    printf("\nEnter command: \n"
            "(1) insert\n"
            "(2) delete\n"
            "(3) search\n"
            "(4) print\n"
            "(5) store and exit\n\n");

    if (fgets(buffer, 1024, stdin) && sscanf(buffer, "%hd %hd", &c, &d) != 1) {
      printf("Error: invalid command\n"); 
      continue;
    }
    
    switch (c) {
      case 1:
        printf("Enter student id, dob, gpa, first name, last name: ");
        student *s = newStudent();
        if ((fgets(buffer, 1024, stdin) && sscanf(buffer, "%d %s %lf %s %s", 
        &s->id, s->dob, &s->gpa, s->fname, s->lname) != 5) || !validStudent(s)) {
          printf("Error: invalid student record\n");
          free(s);
          continue;
        }
        n = search(tree, s->id);
        if (n == NIL) {
          // insert into RBT  
          insert(tree, s);
          printf("Inserted student with id %d\n", s->id);
        } else {
          printf("Error: student with id %d already exists\n", s->id);
          free(s);
        }
        break;
      case 2:
        printf("Enter student id: ");
        int id;
        if ((fgets(buffer, 1024, stdin) && sscanf(buffer, "%d", &id) != 1)
        || id < 1000000 || id > 9999999) {
          printf("Error: invalid id\n");
          continue;
        }
        n = search(tree, id);
        if (n != NIL) {
          // delete from RBT
          delete(tree, n);
          printf("Deleted student with id %d\n", id);
        } else
          printf("Error: student with id %d not found\n", id);
        break;
      case 3:
        printf("Enter student id: ");
        if ((fgets(buffer, 1024, stdin) && sscanf(buffer, "%d", &id) != 1)
        || id < 1000000 || id > 9999999) {
          printf("Error: invalid id\n");
          continue;
        }
        n = search(tree, id);
        if (n != NULL)
          printStudent(n->student);
        else
          printf("Error: student with id %d not found\n", id);
        break;
      case 4:
        printInorder(NIL, ROOT, &count);
        count = 0;
        break;
      case 5:
        // overwrite student records file with updated RBT
        fp = fopen(argv[1], "w+");
        if (fp == NULL) {
          printf("Error: could not open file %s\n", argv[1]);
          exit(EXIT_FAILURE);
        }
        writeToFile(NIL, ROOT, fp);
        fclose(fp);
        freeRBT(tree);
        exit(EXIT_SUCCESS);
      default:
        printf("Error: invalid command\n");
    }
  }
  return 0;
}