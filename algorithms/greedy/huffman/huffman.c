/* file: huffman.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: Huffman coding
     using a greedy approach and a minimum priority queue
   time complexity: O(n log n), where n is the number 
     of characters in the input
*/ 

#include <stdio.h>
#include <stdlib.h>

#define LEFT(i)   (2*i + 1)
#define RIGHT(i)  (2*i + 2)
#define PARENT(i) ((i-1)/2)

//:::::::::::::::::::::::: data structures ::::::::::::::::::::::::://

typedef struct Node {
  int freq;     // frequency of the character
  char ch;      // character
  struct Node *left, *right;
} Node;

typedef struct {
  int size;     // size of the heap
  int n;        // number of elements in the heap
  Node **nodes; // array of pointers to nodes
} Heap;

//::::::::::::::::::::::: memory management :::::::::::::::::::::::://

void *safeMalloc (int n) {
  /* checks if memory has been allocated successfully */
  void *p = malloc(n);
  if (p == NULL) {
    printf("Error: malloc(%d) failed. Out of memory?\n", n);
    exit(EXIT_FAILURE);}
  return p;
}

//::::::::::::::::::::::::: tree functions ::::::::::::::::::::::::://

Node *newNode (char ch, int freq) {
  /* creates a new node */
  Node *node = safeMalloc(sizeof(Node));
  node->ch = ch;
  node->freq = freq;
  node->left = node->right = NULL;
  return node;
}

void freeTree (Node *n) {
  /* frees the tree */
  if (n == NULL) return;
  freeTree(n->left);
  freeTree(n->right);
  free(n);
}

//::::::::::::::::::::::::: heap functions ::::::::::::::::::::::::://

Heap *newHeap (int size) {
  /* creates a new heap */
  Heap *heap = safeMalloc(sizeof(Heap));
  heap->size = size;
  heap->n = 0;
  heap->nodes = safeMalloc(sizeof(Node *) * size);
  return heap;
}

void freeHeap (Heap *H) {
  /* frees the heap */
  free(H->nodes);
  free(H);
}

void swap (Heap *H, int i, int j) {
  /* swaps the elements at indices i and j in the heap */
  Node *tmp = H->nodes[i];
  H->nodes[i] = H->nodes[j];
  H->nodes[j] = tmp;
}

void minHeapify(Heap *H, int i){
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

void minHeapInsert(Heap *H, Node *new){
  /* inserts new node into the min heap and restores 
     the min heap property in a bottom-up manner */
  int i = H->n;
  H->nodes[H->n++] = new;
  while (i > 0 && H->nodes[i]->freq < H->nodes[PARENT(i)]->freq) {
    swap(H, i, PARENT(i));
    i = PARENT(i);
  }
}

Node *extractMin(Heap *H){
  /* extracts the node with the minimum frequency */
  Node *min = H->nodes[0];
  H->nodes[0] = H->nodes[--H->n];
  minHeapify(H, 0);
  return min;
}

void initMinHeap(Heap *H){
  /* initializes the min heap */
  for (int i = H->n/2 - 1; i >= 0; i--)
    minHeapify(H, i);
}

Heap *readHeap (char chars[], int freq[], int len) {
  /* reads the min heap from the input */
  Heap *H = newHeap(len);
  for (int i = 0; i < len; i++) {
    if (freq[i] > 0) {
      Node *node = newNode(chars[i], freq[i]);
      H->nodes[H->n++] = node;
    }
  }
  initMinHeap(H);
  return H;
}

//:::::::::::::::::::::::: print functions ::::::::::::::::::::::::://

void showArray(int arr[], int n) {
  for (int i = 0; i < n; ++i)
    printf("%d", arr[i]);
  printf("\n");
}

void showCodes(Node *n, int code[], int level) {
  if (n->left) {
    code[level] = 0;
    showCodes(n->left, code, level+1);
  }
  if (n->right) {
    code[level] = 1;
    showCodes(n->right, code, level+1);
  }
  if (!n->left && !n->right) { // leaf node
    printf("'%c' = ", n->ch);
    showArray(code, level);
  }
}

void showTree(Node *n, int level){
  if (!n) return;
  for (int i = 0; i < level; i++)
    printf(i == level - 1 ? "  |-" : "  ");
  if (!n->left && !n->right)  // leaf node
    printf("%d '%c'\n", n->freq, n->ch);
  else  // internal node
    printf("%d\n", n->freq);
  showTree(n->left, level+1);
  showTree(n->right, level+1);
}

void printExample(Node *root, int example) {
  int code[10];  
  printf("Example %d\n", example); 
  printf("---------\n"); 
  showCodes(root, code, 0);
  printf("\nHuffman tree:\n\n  ");
  showTree(root, 0);
  printf("\n");
}

//::::::::::::::::::::::: huffman functions :::::::::::::::::::::::://

Node *huffman (Heap *H) {
  /* creates the Huffman tree */
  while (H->n > 1) {
    Node *x = extractMin(H);
    Node *y = extractMin(H);
    Node *z = newNode('#', x->freq + y->freq);
    z->left = x; z->right = y;
    minHeapInsert(H, z);
  }
  return extractMin(H);
}

//::::::::::::::::::::::::::::: main ::::::::::::::::::::::::::::::://

int main (int argc, char *argv[]) {
  //EXAMPLE 1: taken from the textbook
  char chars1[] = {'a','b','c','d','e','f'};
  int freq1[] = {45,13,12,16,9,5};

  //EXAMPLE 2: "Huffman coding is a data compression algorithm."
  char chars2[] = {'H','u','f','m','a','n',' ','c','o','d',
                   'i','g','s','t','p','r','e','l','h','.'};
  int freq2[] = {1,1,2,3,5,3,6,2,4,2,4,2,3,2,1,1,1,1,1,1};
  
  int len1 = sizeof(chars1) / sizeof(chars1[0]);
  int len2 = sizeof(chars2) / sizeof(chars2[0]);
  Heap *H1 = readHeap(chars1, freq1, len1); 
  Heap *H2 = readHeap(chars2, freq2, len2);
  Node *root1 = huffman(H1);
  Node *root2 = huffman(H2);
 
  printExample(root1, 1);
  printExample(root2, 2);

  freeTree(root1);
  freeTree(root2);
  freeHeap(H1);
  freeHeap(H2);
  return 0; 
}
  
