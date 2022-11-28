/* file: closestpair2.c
* author: David De Potter
* description: closest pair of points using divide and conquer
* This version is in O(nlogn), which is achieved by presorting
* not only on x-coordinates but also on y-coordinates. 
* We find T(n) = 2T(n/2) + Θ(n) = Θ(nlogn) by case 2 of 
* the master theorem.
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef struct point {
  double x;
  double y;
} point;

typedef struct pair {
  double dist;
  point p1;
  point p2;
} pair;

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

int xcompare(const void *a, const void *b) {
    point *p1 = (point*) a;
    point *p2 = (point*) b;
    return p1->x - p2->x;
}

// compare two points by y-coordinate
int ycompare(const void *a, const void *b) {
    point *p1 = (point*) a;
    point *p2 = (point*) b;
    return p1->y - p2->y;
}

// returns euclidean distance between two points
double distance(point p1, point p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

pair findClosestPairInStrip (point *ypoints, int ysize, double median, double min) {
  point *strip = safeCalloc (ysize, sizeof(point));
  int len = 0; pair p = {DBL_MAX, {0,0}, {0,0}};

  for (int i = 0; i < ysize; ++i) {
    if (abs(ypoints[i].x - median) < min) 
      strip[len++] = ypoints[i];
  }
  
  for (int i = 0; i < len; ++i) {
    for (int j = 1; j < 8 && i + j < len; ++j) {
      double d = distance (strip[i], strip[i+j]);
      if (d < min) {
        min = d;
        p.p1 = strip[i];
        p.p2 = strip[i+j];
        p.dist = d;
      }
    }
  }
  free (strip);
  return p;
}

pair findClosestPair(point *xpoints, point *ypoints, int ysize, int n) {
  pair pair1, pair2; double d1, d2, d3; 

  // base cases: 2 or 3 points
  if (n == 2) {
    pair1.p1 = xpoints[0];
    pair1.p2 = xpoints[1];
    pair1.dist = distance(xpoints[0], xpoints[1]);
    return pair1;
  }
  if (n == 3) {
    d1 = distance(xpoints[0], xpoints[1]);
    d2 = distance(xpoints[0], xpoints[2]);
    d3 = distance(xpoints[1], xpoints[2]);
    pair1.p1 = xpoints[0];
    pair1.p2 = xpoints[1];
    pair1.dist = d1;
    if (d2 < d1 && d2 < d3) {
      pair1.p1 = xpoints[0];
      pair1.p2 = xpoints[2];
      pair1.dist = d2;
    }
    if (d3 < d1 && d3 < d2) {
      pair1.p1 = xpoints[1];
      pair1.p2 = xpoints[2];
      pair1.dist = d3;
    }
    return pair1;
  }

  // divide
  point *yrpoints = safeCalloc(ysize, sizeof(point));
  point *ylpoints = safeCalloc(ysize, sizeof(point));
  
  // make y-sorted subarrays based on which side of 
  // the median the points are on
  int yl = 0, yr = 0, mid = n / 2;
  int median = xpoints[mid].x;
  for (int i = 0; i < ysize; ++i) {
    if (ypoints[i].x < median) 
      ylpoints[yl++] = ypoints[i];
    else 
      yrpoints[yr++] = ypoints[i];
  }
  // left half
  pair1 = findClosestPair(xpoints, ylpoints, yl, mid);   
  // right half       
  pair2 = findClosestPair(xpoints + mid, yrpoints, yr, n - mid); 
  double min = MIN(pair1.dist, pair2.dist); 
  free (yrpoints); free (ylpoints);
  
  // conquer
  pair pair3 = findClosestPairInStrip(ypoints, ysize, median, min);

  // return the closest pair
  d1 = pair1.dist; d2 = pair2.dist; d3 = pair3.dist; 
  if (d1 <= d2 && d1 <= d3) return pair1;
  if (d2 <= d1 && d2 <= d3) return pair2;
  return pair3;
}

int main(int argc, char *argv[]) {
  int n; 
  scanf("%d\n", &n);
  point *xpoints = safeCalloc(n, sizeof(point));
  point *ypoints = safeCalloc(n, sizeof(point));

  for (int i = 0; i < n; i++) {
    scanf("(%lf,%lf),", &xpoints[i].x, &xpoints[i].y);
    ypoints[i] = xpoints[i];
  } 
  
  qsort(xpoints, n, sizeof(point), xcompare);
  qsort(ypoints, n, sizeof(point), ycompare);

  pair pair = findClosestPair(xpoints, ypoints, n, n);

  printf("The closest distance is %lf between (%lf,%lf)" 
         " and (%lf,%lf).\n", pair.dist, pair.p1.x, 
          pair.p1.y, pair.p2.x, pair.p2.y);

  free(xpoints); 
  free(ypoints);

  return 0;
}