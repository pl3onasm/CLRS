$\huge{\color{cadetblue} \text{Selec} \text{tion sort}}$

<br/>

Just like [insertion sort](https://github.com/pl3onasm/CLRS/tree/main/algorithms/sorting/insertion-sort), selection sort is an ${\color{darkseagreen} \text{in-place}}$ comparison sort which sorts an array $A[0:n-1]$ in ascending order by maintaining a sorted subarray $A[0:i]$ and an unsorted subarray $A[i+1:n-1]$. At each iteration, the boundary between the two subarrays is moved one element to the right after selecting the minimum element from the unsorted subarray and swapping it with $A[i+1]$, thus extending the sorted subarray by one element. This process is repeated until the entire array is sorted.

The name comes from the fact that we are ${\color{darkseagreen} \text{selec} \text{ting the minimum}}$ element at each step, by performing a linear search through the unsorted subarray, and then place this element at the leftmost position of the unsorted subarray, so that the boundary between the two subarrays can be moved by one element to the right. This is in contrast to insertion sort, where we do not search the unsorted subarray for the minimum element, but simply take the next element from the unsorted subarray at $A[i+1]$ and then insert it into its correct position in the sorted subarray $A[0:i]$, by comparing it with each element in the sorted subarray from right to left until we find a smaller element or reach the left end of the sorted subarray.

The running time of selection sort is in ${\color{rosybrown}\mathcal{O}(n^2)}$, even if the array is already sorted, since we still need to iterate through the entire unsorted part of the array to determine the minimum element at each step of the algorithm. Unlike [bubble sort](https://github.com/pl3onasm/CLRS/tree/main/algorithms/sorting/bubble-sort), selection sort is ${\color{darkseagreen} \text{unstable}}$, since it swaps elements that are not adjacent, thus potentially changing the relative order of elements with the same value.

Implementation: [Selection sort](https://github.com/pl3onasm/CLRS/tree/main/algorithms/sorting/selection-sort/selectionsort.c)
