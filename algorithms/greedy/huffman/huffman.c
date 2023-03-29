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

#define par(i) ((i-1)/2)  // parent of node at index i

void *safeMalloc (int n) {
  /* checks if memory has been allocated successfully */
  void *p = malloc(n);
  if (p == NULL) {
    printf("Error: malloc(%d) failed. Out of memory?\n", n);
    exit(EXIT_FAILURE);}
  return p;
}

Node *newNode (char ch, int freq) {
  /* creates a new node */
  Node *node = safeMalloc(sizeof(Node));
  node->ch = ch;
  node->freq = freq;
  node->left = node->right = NULL;
  return node;
}

Heap *newHeap (int size) {
  /* creates a new heap */
  Heap *heap = safeMalloc(sizeof(Heap));
  heap->size = size;
  heap->n = 0;
  heap->nodes = safeMalloc(sizeof(Node *) * size);
  return heap;
}

void freeTree (Node *n) {
  /* frees the tree */
  if (n == NULL) return;
  freeTree(n->left);
  freeTree(n->right);
  free(n);
}

void freeHeap (Heap *hp) {
  /* frees the heap */
  free(hp->nodes);
  free(hp);
}

void swap (Heap *hp, int i, int j) {
  /* swaps the elements at indices i and j in the heap */
  Node *tmp = hp->nodes[i];
  hp->nodes[i] = hp->nodes[j];
  hp->nodes[j] = tmp;
}

void minHeapify(Heap *hp, int i){
  /* restores the min heap property in a top-down manner */
  int min = i, left = 2 * i + 1, right = 2 * i + 2;
  if (left < hp->n && hp->nodes[left]->freq < hp->nodes[i]->freq)
    min = left;
  if (right < hp->n && hp->nodes[right]->freq < hp->nodes[min]->freq)
    min = right;
  if (min != i) {
    swap(hp, i, min); 
    minHeapify(hp, min);
  }
}

void minHeapInsert(Heap *hp, Node *new){
  /* inserts new node into the min heap and restores 
     the min heap property in a bottom-up manner */
  int i = hp->n;
  hp->nodes[hp->n++] = new;
  while (i > 0 && hp->nodes[i]->freq < hp->nodes[par(i)]->freq) {
    swap(hp, i, par(i));
    i = par(i);
  }
}

Node *extractMin(Heap *hp){
  /* extracts the node with the minimum frequency */
  Node *min = hp->nodes[0];
  hp->nodes[0] = hp->nodes[--hp->n];
  minHeapify(hp, 0);
  return min;
}

void initMinHeap(Heap *hp){
  /* initializes the min heap */
  for (int i = hp->n/2 - 1; i >= 0; i--)
    minHeapify(hp, i);
}

Heap *readHeap (char chars[], int freq[], int len) {
  /* reads the min heap from the input */
  Heap *hp = newHeap(len);
  for (int i = 0; i < len; i++) {
    if (freq[i] > 0) {
      Node *node = newNode(chars[i], freq[i]);
      hp->nodes[hp->n++] = node;
    }
  }
  initMinHeap(hp);
  return hp;
}

Node *huffman (Heap *hp) {
  /* creates the Huffman tree */
  while (hp->n > 1) {
    Node *x = extractMin(hp);
    Node *y = extractMin(hp);
    Node *z = newNode('#', x->freq + y->freq);
    z->left = x; z->right = y;
    minHeapInsert(hp, z);
  }
  return extractMin(hp);
}

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
  if (n == NULL) return;
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
  Heap *hp1 = readHeap(chars1, freq1, len1); 
  Heap *hp2 = readHeap(chars2, freq2, len2);
  Node *root1 = huffman(hp1);
  Node *root2 = huffman(hp2);
 
  printExample(root1, 1);
  printExample(root2, 2);

  freeTree(root1);
  freeTree(root2);
  freeHeap(hp1);
  freeHeap(hp2);
  return 0; 
}
  
