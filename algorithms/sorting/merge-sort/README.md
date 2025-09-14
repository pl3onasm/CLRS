$\huge{\color{cadetblue} \text{Merge sort}}$

<br/>

Merge sort is a comparison sort based on the  ${\color{darkseagreen} \text{divide and conquer}}$ paradigm. It starts out by computing the middle index, after which it divides the input array into two subarrays of equal size. It then recursively calls itself on each of the subarrays, until the base case is reached, which is when the subarrays contain only one element and are trivially sorted. At this point, recursion starts to unwind, and the merge procedure is called, which combines the two sorted subarrays into a single sorted array. This merging is done by maintaining two pointers, one for each subarray, and comparing the elements at the pointers. At each comparison, the smaller element is copied into the sorted output array, and the pointer is incremented. This process is repeated until one of the subarrays is exhausted, at which point the remaining elements of the other subarray are copied into the end of the sorted output array. The merge procedure is repeated recursively until recursion has unwound completely, and the entire input array is sorted.

Since the algorithm divides the problem into two subproblems of half the size, and combines the solutions to the subproblems in $\Theta(n)$ time, the recurrence for the running time of merge sort is given by:

$$
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = 2T(n/2) + \Theta(n) \space}
$$

It can be solved using the master theorem, case 2, which yields the solution ${\color{rosybrown}T(n) = \Theta(n \log n)}$. This is also the best case running time, since the algorithm always divides the input array into two subarrays, regardless of the input. Merge sort is a ${\color{darkseagreen} \text{stable}}$ sort, meaning that the relative order of equal elements is preserved. It is also an ${\color{darkseagreen} \text{out-of-place}}$ sort, since it requires additional memory to store the sorted output array. The space complexity is $\Theta(n)$.

Implementation: [Merge Sort](https://github.com/pl3onasm/CLRS/tree/main/algorithms/sorting/merge-sort/mergesort.c)

A generic implementation: [Generic Merge Sort](https://github.com/pl3onasm/CLRS/tree/main/algorithms/sorting/merge-sort/genmergesort.c)

<br/>

<br/>

$\huge{\color{cadetblue} \text{WHAM sort}}$

<br/>

WHAM sort is an optimization of merge sort. The name is an acronym for the names of its creators, Wim Hesselink and Arnold Meijster. It draws on the observation that merge sort is inefficient when the input array is already entirely or partially sorted, since it will still follow the same old recipe of dividing the original problem into two equal-sized subproblems around the middle index, regardless of the input. WHAM sort improves on this by dividing the input array around the first index where the elements are out of order, i.e. the first index where an ${\color{darkseagreen} \text{inversion}}$ occurs. This means that the left subarray is already sorted, and WHAM sort only needs to be recursively called on the unsorted right subarray which is stretched to the same length as the left part (or until the right bound if stretching exceeds the length of the input array). Once the base case is reached, and recursion starts to unwind, the merge procedure combines the two sorted subarrays into a single sorted array and is repeated recursively until the entire input array is sorted.

Although WHAM sort still has a worst-case running time of ${\color{rosybrown}\mathcal{O}(n \log n)}$, it is able to take advantage of any existing order in the input array and perform significantly better than merge sort in those cases where the input array is already partially or entirely sorted. It also has a best-case running time of $\mathcal{O}(n)$, which is an improvement over the best-case running time of $\mathcal{O}(n \log n)$ for merge sort. Just like merge sort, WHAM sort is a ${\color{darkseagreen} \text{stable}}$ sort, and an ${\color{darkseagreen} \text{out-of-place}}$ sort, with a space complexity of $\Theta(n)$.

Implementation: [WHAM Sort](https://github.com/pl3onasm/CLRS/tree/main/algorithms/sorting/merge-sort/whamsort.c)
