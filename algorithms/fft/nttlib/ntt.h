#ifndef NTT_H_INCLUDED
#define NTT_H_INCLUDED

#include <stdlib.h>
#include <complex.h>
typedef complex double cdbl;


  // returns the forward fast fourier transform
  // of an array of complex numbers
cdbl *ntt(cdbl *arr, size_t arrLen);

  // returns the backward fast fourier transform
  // of an array of complex numbers
cdbl *intt(cdbl *arr, size_t arrLen);

  // returns the convolution of two input arrays
  // representing complex coefficients of polynomials
cdbl *convolve(cdbl *x, cdbl *y, size_t xLen, size_t yLen);


#endif // NTT_H_INCLUDED
