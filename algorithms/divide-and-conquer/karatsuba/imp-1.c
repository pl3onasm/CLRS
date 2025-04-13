/* 
  file: imp-1.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: standard long multiplication
  time complexity: O(n²), where n is the number 
    of digits of the maximum of the two input numbers
*/

#include "../../../lib/clib.h"
#include "natlib/nat.h"

//===================================================================
// Returns the product of x and y using standard long multiplication
// Intermediate results are first stored in res->digits as integers
// and in the end everything is converted back to char
Nat *mulNat(Nat *x, Nat *y) {

  Nat *res = newNat(x->size + y->size + 1);
  res->size = x->size + y->size;

    // multiplies each digit of x with each digit of y
    // and adds the result to the corresponding position
  for (size_t i = x->size; i--; ) 
    for (size_t j = y->size; j--; ) {
      int s = (x->digits[i] - '0') * (y->digits[j] - '0') 
              + res->digits[i + j + 1];
      res->digits[i + j + 1] = s % 10;
      res->digits[i + j] += s / 10;
    }

    // compute start index and
  while (res->start < res->size && res->digits[res->start] == 0)
    res->start++;

    // convert back to char
  for (size_t i = res->start; i < res->size; i++)
    res->digits[i] += '0';
  
  res->size -= res->start;
  return res;
}

//===================================================================

int main() {

  Nat *x = readNat();
  Nat *y = readNat();

  Nat *prod = mulNat(x, y);
  showNat(prod);
  
  freeNat(x);
  freeNat(y);
  freeNat(prod);
  return 0;
}