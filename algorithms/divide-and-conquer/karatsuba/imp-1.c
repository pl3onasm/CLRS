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
// Intermediate results are first stored in prod->digits as integers
// and in the end everything is converted back to char
Nat *mulNat(Nat *x, Nat *y) {

  size_t n = x->size + y->size;
  Nat *prod = newNat(n);

    // multiplies each digit of x with each digit of y
    // and adds the result to the corresponding position
  for (size_t i = x->size; i--; ) 
    for (size_t j = y->size; j--; ) {
      int s = (x->digits[i] - '0') * (y->digits[j] - '0') 
              + prod->digits[i + j + 1];
      prod->digits[i + j + 1] = s % 10;
      prod->digits[i + j] += s / 10;
    }

    // compute start index
  while (prod->start < n && prod->digits[prod->start] == 0)
    prod->start++;

    // convert back to char
  for (size_t i = prod->start; i < n; i++)
    prod->digits[i] += '0';
  
  prod->size = n - prod->start;
  return prod;
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