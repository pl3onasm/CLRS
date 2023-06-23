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

#include "rbt.h"

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
    RBTinsert(tree, s);
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
            "(5) save and exit\n\n");

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
        n = RBTsearch(tree, s->id);
        if (n == NIL) {
          // insert into RBT  
          RBTinsert(tree, s);
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
        n = RBTsearch(tree, id);
        if (n != NIL) {
          // delete from RBT
          RBTdelete(tree, n);
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
        n = RBTsearch(tree, id);
        if (n != NIL)
          printStudent(n->student);
        else
          printf("Error: student with id %d not found\n", id);
        break;
      case 4:
        printRBT(tree, ROOT, &count);
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