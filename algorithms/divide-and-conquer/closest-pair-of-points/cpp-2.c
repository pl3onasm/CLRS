/* 
  file: cpp-2.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: closest pair of points using divide and conquer
  time complexity: O(nlogn), which is achieved by presorting
    not only on x-coordinates but also on y-coordinates. 
    We find T(n) = 2T(n/2) + Θ(n) = Θ(nlogn) by case 2 of 
    the master theorem.
*/ 

#include <math.h>
#include <float.h>
#include "../../../lib/clib.h"
#define MIN(a,b) ((a) < (b) ? (a) : (b))

//===================================================================
// Defines a point and a pair of points

typedef struct point {
  double x;             // x-coordinate of point
  double y;             // y-coordinate of point
} point;

typedef struct pair {
  point p1;             // first point of the pair
  point p2;             // second point of the pair
  double dist;          // distance between the two points
} pair;

//===================================================================
// Compares two points by their x-coordinate
int compareXs(const void *a, const void *b) {
  return ((point*) a)->x - ((point*) b)->x;
}

//===================================================================
// Compares two points by their y-coordinate
int compareYs(const void *a, const void *b) {
  return ((point*) a)->y - ((point*) b)->y;
}

//===================================================================
// Returns the Euclidean distance between two points p1 and p2
double distance(point p1, point p2) {
  return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

//===================================================================
// Sets pair p to points p1 and p2 with distance dist
void setPair (pair *p, point p1, point p2, double dist) {
  p->p1 = p1;
  p->p2 = p2;
  p->dist = dist;
}

//===================================================================
// Finds closest pair in strip of width 2*delta around median
pair findClosestPairInStrip (point *ypoints, size_t ysize, 
                             double median, double delta) {
 
  point *strip = safeCalloc(ysize, sizeof(point));
  size_t len = 0; pair p = {{0,0}, {0,0}, DBL_MAX};

    // make strip of points within 2*delta around median
  for (size_t i = 0; i < ysize; ++i) 
    if (abs(ypoints[i].x - median) < delta) 
      strip[len++] = ypoints[i];
  
    // pass through strip in groups of 8 points at a time
    // and keep track of the new closest pair (if any)
  for (size_t i = 0; i < len; ++i)
      // check next 7 points
    for (size_t j = 1; j < 8 && i + j < len; ++j) {  
      double d = distance (strip[i], strip[i+j]);
        // new closest pair found?
      if (d < delta) {  
        delta = d;
        setPair (&p, strip[i], strip[i+j], d);
      }
    }

  free (strip);
  return p;
}

//===================================================================
// Finds closest pair of points using a divide and conquer approach
pair findClosestPair(point *xpoints, point *ypoints, 
                     size_t ysize, size_t n) {

  pair minpair, pair1, pair2, pair3; 
  double d1, d2, d3; 

    // BASE CASES: brute force for n <= 3
  if (n <= 3) {
    d1 = distance(xpoints[0], xpoints[1]);
    setPair (&minpair, xpoints[0], xpoints[1], d1);
    if (n == 2) return minpair;   
    d2 = distance(xpoints[0], xpoints[2]);
    d3 = distance(xpoints[1], xpoints[2]);
    if (d2 < d1 && d2 < d3) 
      setPair (&minpair, xpoints[0], xpoints[2], d2);
    if (d3 < d1 && d3 < d2) 
      setPair (&minpair, xpoints[1], xpoints[2], d3);
    return minpair;
  }

    // DIVIDE: split ypoints into left and right half based on
    //         whether they are left or right of the median
  point *ylpoints = safeCalloc(n, sizeof(point));
  point *yrpoints = safeCalloc(n, sizeof(point));

  size_t yl = 0, yr = 0, mid = n / 2;
  size_t median = xpoints[mid].x;
  for (size_t i = 0; i < ysize; ++i) {
    if (ypoints[i].x < median) 
      ylpoints[yl++] = ypoints[i];
    else 
      yrpoints[yr++] = ypoints[i];
  }

    // CONQUER: find closest pair in each half
  pair1 = findClosestPair(xpoints, ylpoints, yl, mid);         
  pair2 = findClosestPair(xpoints + mid, yrpoints, yr, n - mid); 
  free (yrpoints); free (ylpoints);
  
    // COMBINE: find closest pair in strip of width 2*delta around 
    //          median and return the closest of the three pairs
  double delta = MIN(pair1.dist, pair2.dist); 
  pair3 = findClosestPairInStrip(ypoints, ysize, median, delta);

  d1 = pair1.dist; d2 = pair2.dist; d3 = pair3.dist; 
  if (d1 <= d2 && d1 <= d3) return pair1;
  if (d2 <= d1 && d2 <= d3) return pair2;
  return pair3;
}

//===================================================================

int main() {

  size_t n; 
  assert(scanf("%zu\n", &n) == 1);

  point *xpoints = safeCalloc(n, sizeof(point));
  point *ypoints = safeCalloc(n, sizeof(point));

    // read points from stdin
  for (int i = 0; i < n; i++) {
    assert(scanf("(%lf,%lf),", &xpoints[i].x, &xpoints[i].y) == 2);
    ypoints[i] = xpoints[i];
  } 
  
    // presort points once by x and once by y coordinates
  qsort(xpoints, n, sizeof(point), compareXs);
  qsort(ypoints, n, sizeof(point), compareYs);

    // find closest pair of points
  pair pair = findClosestPair(xpoints, ypoints, n, n);

    // print result
  printf("Closest distance: %lf\n"
         "Found between (%lf,%lf) and (%lf,%lf)\n", 
          pair.dist, pair.p1.x, pair.p1.y,
          pair.p2.x, pair.p2.y);

  free(xpoints); 
  free(ypoints);
  return 0;
}