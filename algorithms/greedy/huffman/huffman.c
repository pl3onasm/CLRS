/* file: huffman.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: Huffman coding
     using a greedy approach and a minimum priority queue
   time complexity: O(n log n), where n is the number 
     of characters in the input
   assumption: the input text contains only characters from 
     the extended ASCII table 
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define LEFT(i)   (2*i + 1)
#define RIGHT(i)  (2*i + 2)
#define PARENT(i) ((i-1)/2)
#define LENGTH    256
#define MAX(a, b) ((a) > (b) ? (a) : (b))
typedef unsigned int uint;

//:::::::::::::::::::::::: data structures ::::::::::::::::::::::::://

typedef struct node {
  int freq;     // frequency of the character
  char ch;      // character
  struct node *left, *right;
} node;

typedef struct {
  int size;     // capacity of the heap
  int n;        // number of elements in the heap
  node **nodes; // array of pointers to nodes
} heap;

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

void *safeRealloc (void *ptr, int newSize) {
  /* reallocates memory and checks whether the allocation was successful */
  ptr = realloc(ptr, newSize);
  if (ptr == NULL) {
    printf("Error: realloc(%d) failed. Out of memory?\n", newSize);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

//::::::::::::::::::::::::: tree functions ::::::::::::::::::::::::://

node *newNode () {
  /* creates a new node */
  node *n = safeCalloc(1, sizeof(node));
  n->ch = -1;
  return n;
}

void freeTree (node *n) {
  /* frees the tree */
  if (!n) return;
  freeTree(n->left);
  freeTree(n->right);
  free(n);
}

uint getHeight (node *n) {
  /* returns the height of the tree */
  if (!n) return 0;
  return 1 + MAX(getHeight(n->left), getHeight(n->right));
}

void showTree(node *n, short *code, int level) {
  /* can be used to print the Huffman tree */
  if (!n) return;
  for (int i = 0; i < level; i++)
    printf(i == level - 1 ? "  |%hd " : "  ", code[level-1]);
  if (!n->left && !n->right)  // leaf node
    printf("'%c'\n", n->ch);
  else  // internal node
    printf("\n");

  code[level] = 0;
  showTree(n->left, code, level+1);
  code[level] = 1;
  showTree(n->right, code, level+1);
}

//::::::::::::::::::::::::: heap functions ::::::::::::::::::::::::://

heap *newHeap (int size) {
  /* creates a new heap */
  heap *H = safeCalloc(size, sizeof(heap));
  H->size = size;
  H->n = 0;
  H->nodes = safeCalloc(size, sizeof(node*));
  return H;
}

void freeHeap (heap *H) {
  /* frees the heap */
  free(H->nodes);
  free(H);
}

void swap (heap *H, int i, int j) {
  /* swaps the elements at indices i and j in the heap */
  node *tmp = H->nodes[i];
  H->nodes[i] = H->nodes[j];
  H->nodes[j] = tmp;
}

void minHeapify(heap *H, int i){
  /* restores the min heap property in a top-down manner */
  int min = i, l = LEFT(i), r = RIGHT(i);
  if (l < H->n && H->nodes[l]->freq < H->nodes[i]->freq)
    min = l;
  if (r < H->n && H->nodes[r]->freq < H->nodes[min]->freq)
    min = r;
  if (min != i) {
    swap(H, i, min); 
    minHeapify(H, min);
  }
}

void minHeapInsert(heap *H, node *new){
  /* inserts new node into the min heap and restores 
     the min heap property in a bottom-up manner */
  int i = H->n;
  H->nodes[H->n++] = new;
  while (i > 0 && H->nodes[i]->freq < H->nodes[PARENT(i)]->freq) {
    swap(H, i, PARENT(i));
    i = PARENT(i);
  }
}

node *extractMin(heap *H){
  /* extracts the node with the minimum frequency */
  node *min = H->nodes[0];
  H->nodes[0] = H->nodes[--H->n];
  minHeapify(H, 0);
  return min;
}

void initMinHeap(heap *H){
  /* initializes the min heap */
  for (int i = H->n/2 - 1; i >= 0; i--)
    minHeapify(H, i);
}

heap *buildHeap (uint *freqs) {
  /* builds the min heap from the input */
  heap *H = newHeap(LENGTH);
  for (int i = 0; i < LENGTH; i++) {
    if (freqs[i]) {
      node *n = newNode();
      n->ch = i;
      n->freq = freqs[i];
      H->nodes[H->n++] = n;
    }
  }
  initMinHeap(H);
  return H;
}

//::::::::::::::::::::::::: input / output ::::::::::::::::::::::::://

void readInput (uint *freqs) {
  /* reads the input text and counts 
  the frequencies of the characters */
  char ch;
  while ((ch = getchar()) != EOF)
    if (ch >= 32 && ch < LENGTH)  // only printable characters
      freqs[ch-0]++;
}

void showCode(short *arr, int n) {
  /* prints the code for a character */
  for (int i = 0; i < n; ++i)
    printf("%hd", arr[i]);
  printf("\n");
}

void showCodes(node *n, short *code, int level) {
  /* prints the huffman tree by level */
  if (!n->left && !n->right) {
    printf("%4c %7d      ", n->ch, n->freq);
    showCode(code, level);
    return;
  }
  code[level] = 0;
  showCodes(n->left, code, level + 1);
  code[level] = 1;
  showCodes(n->right, code, level + 1);
}

//::::::::::::::::::::::: huffman functions :::::::::::::::::::::::://

node *huffman (heap *H) {
  /* creates the Huffman tree */
  while (H->n > 1) {
    node *z = newNode();
    z->left = extractMin(H);
    z->right = extractMin(H);
    z->freq = z->left->freq + z->right->freq;
    minHeapInsert(H, z);
  }
  return extractMin(H);
}

//::::::::::::::::::::::::::::: main ::::::::::::::::::::::::::::::://

int main (int argc, char *argv[]) {
  
  // reads the input and stores the frequencies
  uint freqs[LENGTH] = {0};
  readInput(freqs);   

  // builds a binary min heap from the frequencies
  heap *H = buildHeap(freqs);
  
  // creates the Huffman tree
  node *tree = huffman(H);    

  // height of the tree is the maximum code length
  uint height = getHeight(tree);  
  short *code = safeCalloc(height, sizeof(short));

  // prints the codes and frequencies of the characters
  printf("char    freq      code\n");
  showCodes(tree, code, 0);
  //showTree(tree, code, 0);  // uncomment to print the tree
 
  freeTree(tree);
  freeHeap(H);
  free(code);
  return 0; 
}
  
