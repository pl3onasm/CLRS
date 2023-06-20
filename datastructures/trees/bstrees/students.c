/* file: bststudents.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: this is a simple example of a binary search tree
    using student records as data.
    The student records are stored in a file, one record per line.
    This file is read into a binary search tree, and the user can
    then search for a student by name, delete or insert a student,
    or print all existing records in the tree. Since only student
    numbers are unique, the tree is ordered by student number.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 30

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
  struct node *parent;
  struct node *left;
  struct node *right;
} node;

typedef struct {
  node *root;
} bst;

typedef short bool;
#define true 1
#define false 0

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
  /* allocates memory for a new binary search tree */
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
  if (tree->root != NULL)
    freeNodes(tree->root);
  free(tree);
}

//::::::::::::::::::::::::: BST operations :::::::::::::::::::::::::://

void insert (bst *tree, student *s) {
  /* inserts a student record into the BST */
  node *n = newNode(s);
  node *y = NULL;
  node *x = tree->root;

  while (x != NULL) {
    y = x;
    if (s->id < x->student->id)
      x = x->left;
    else
      x = x->right;
  }

  n->parent = y;
  if (y == NULL)
    tree->root = n;
  else if (s->id < y->student->id)
    y->left = n;
  else
    y->right = n;
}

node *search (bst *tree, int id) {
  /* searches for a student record in the binary search tree */
  node *x = tree->root;
  while (x != NULL && id != x->student->id) {
    if (id < x->student->id)
      x = x->left;
    else
      x = x->right;
  }
  return x;
}

void transplant (bst *tree, node *u, node *v) {
  /* replaces node u with node v in the BST */
  if (u->parent == NULL)
    tree->root = v;
  else if (u == u->parent->left)
    u->parent->left = v;
  else
    u->parent->right = v;
  if (v != NULL)
    v->parent = u->parent;
}

node *minimum (node *x) {
  /* returns the node with the smallest key in 
  the subtree rooted at x */
  while (x->left != NULL)
    x = x->left;
  return x;
}

void delete (bst *tree, node *z) {
  /* deletes a node from the BST */
  if (z->left == NULL)
    transplant(tree, z, z->right);
  else if (z->right == NULL)
    transplant(tree, z, z->left);
  else {
    node *y = minimum(z->right);
    if (y->parent != z) {
      transplant(tree, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    transplant(tree, z, y);
    y->left = z->left;
    y->left->parent = y;
  }
  freeNode(z);
}

//::::::::::::::::::::::::: print functions ::::::::::::::::::::::::://

void printStudent (student *s) {
  /* prints a student record */
  printf("%d %s %.2lf %s %s\n", s->id, s->dob, s->gpa, s->fname, s->lname);
}

void printInorder (node *x) {
  /* prints the student records in the BST in order */
  if (x != NULL) {
    printInorder(x->left);
    printStudent(x->student);
    printInorder(x->right);
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

void writeToFile (node *x, FILE *fp) {
  /* prints the student records in the BST in order to given file */
  if (x != NULL) {
    writeToFile(x->left, fp);
    fprintf(fp, "%d %s %.2lf %s %s\n", x->student->id, x->student->dob,
            x->student->gpa, x->student->fname, x->student->lname);
    writeToFile(x->right, fp);
  }
}

void readFromFile (bst *tree, char *filename) {
  /* reads student records from input file and inserts them into the BST */
  FILE *fp;
  student *s = newStudent();

  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Error: could not open file %s\n", filename);
    exit(EXIT_FAILURE);
  }

  while (fscanf(fp, "%d %s %lf %s %s", &s->id, s->dob, &s->gpa, 
                s->fname, s->lname) != EOF) {
    if (!validStudent(s)) {
      printStudent(s);
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
  short c;

  if (argc != 2) {
    printf("Usage: %s <student records file>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  bst *tree = newBST();
  readFromFile(tree, argv[1]);

  while (1) {
    printf("Enter command: \n"
            "(1) insert\n"
            "(2) delete\n"
            "(3) search\n"
            "(4) print\n"
            "(5) exit\n\n");

    if (scanf("%hd", &c) != 1) {
      printf("Error: invalid command\n");
      fgets(buffer, 1024, stdin);
      continue;
    }

    switch (c) {
      case 1:
        printf("Enter student id, dob, gpa, first name, last name: ");
        student *s = newStudent();
        if (scanf("%d %s %lf %s %s", &s->id, s->dob, &s->gpa, s->fname, s->lname) != 5
           || !validStudent(s)) {
          fgets(buffer, 1024, stdin);
          printf("Error: invalid student record\n");
          free(s);
          continue;
        }
        n = search(tree, s->id);
        if (!n) {
          // insert into BST  
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
        if (scanf("%d", &id) != 1) {
          fgets(buffer, 1024, stdin);
          printf("Error: invalid id\n");
          continue;
        }
        n = search(tree, id);
        if (n != NULL) {
          // delete from BST
          delete(tree, n);
          printf("Deleted student with id %d\n", id);
        } else
          printf("Error: student with id %d not found\n", id);
        break;
      case 3:
        printf("Enter student id: ");
        if (scanf("%d", &id) != 1 || id < 1000000 || id > 9999999) {
          fgets(buffer, 1024, stdin);
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
        printInorder(tree->root);
        break;
      case 5:
        // overwrite student records file with updated BST
        fp = fopen(argv[1], "w+");
        if (fp == NULL) {
          printf("Error: could not open file %s\n", argv[1]);
          exit(EXIT_FAILURE);
        }
        writeToFile(tree->root, fp);
        fclose(fp);
        freeBST(tree);
        exit(EXIT_SUCCESS);
      default:
        printf("Error: invalid command\n");
    }
    printf("\n");
  }

  return 0;
}