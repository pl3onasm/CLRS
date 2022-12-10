# Longest Increasing Subsequence (LIS)

## Problem

Given an array of integers, find the length of the longest monotonically increasing (i.e. strictly increasing) subsequence. A subsequence is a sequence that can be derived from the original array by deleting some or no elements without changing the order of the remaining elements.  

More formally, given an array $nums$ of length $n$, find the length of the LIS such that $nums[i] < nums[j]$, for all $i < j \leq n$. Note that we are looking for both the length and an actual subsequence, which is not necessarily unique.  
For example, given the array $nums = [12, 8, 1, 4, 2, 9, 10, 18, 15, 7, 20]$, the length of the LIS is 6, and an actual LIS is $[1, 2, 9, 10, 15, 20]$. This LIS is not unique, however. For example, $[1, 4, 9, 10, 18, 20]$ is also a valid LIS.

## Brute Force Solution

A brute force solution would be to generate all increasing subsequences of the array while keeping track of the longest one. The time complexity of this solution is $O(2^n)$, where $n$ is the length of the array.

Implementation: [LIS - Brute Force](https://github.com/pl3onasm/Algorithms/blob/main/algorithms/dynamic-programming/longest-increasing-sub/lis-1.c)

## Bottom-up DP (LCS)

We can come up with a DP solution by handling the LIS problem in the same way as we handled the [LPS problem](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/longest-palin-sub), i.e. by using the DP solution for the [LCS algorithm](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming/longest-common-sub).  
We can do so because searching for the longest increasing subsequence is the same as searching for the longest possible sorted subsequence of the array, and so we are in fact looking for the LCS between the array and its sorted version without duplicates (since the LIS must be strictly increasing).
So for the example array $nums$, the sorted version of the array is $[1, 2, 4, 7, 8, 9, 10, 12, 15, 18, 20]$, and a non-unique LCS between the two is $[1, 2, 9, 10, 15, 20]$, which is a non-unique LIS of the array $nums$.  
Using the bottom-up approach we developed for the LCS problem, we can build a DP table $dp$ of size $n \times n$, where $n$ is the length of the array. The value of $dp[i][j]$ will be the length of the LCS between the first $i$ elements of the array and the first $j$ elements of the sorted array. The value of $dp[i][j]$ will be $dp[i-1][j-1] + 1$ if $nums[i-1] = sorted[j-1]$, and $max(dp[i-1][j], dp[i][j-1])$ otherwise. The final answer will be the value of $dp[n][n]$.  
The time complexity of this solution is in $O(n^2)$, since sorting the array takes $O(n \log n)$ time and building the DP table takes $O(n^2)$ time. The space complexity is also in $O(n^2)$.

Implementation: [LIS - bottom-up DP (LCS)](https://github.com/pl3onasm/Algorithms/blob/main/algorithms/dynamic-programming/longest-increasing-sub/lis-2.c)

## Bottom-up DP (Optimized)

The previous solution does quite a bit of unnecessary work (such as sorting and removing duplicates). We can develop a DP bottom-up approach for the LIS problem where we only work with the original array, without a sorted copy, and we can also further reduce the space complexity by using only one row of the DP table at a time, since we only need the previous row to compute the next row. For this we need to keep track of the maximum value of the DP table at each iteration.The time complexity of this solution is still in $O(n^2)$, since building the DP table takes $O(n^2)$ time. The space complexity is in $O(n)$.  
So, the table dp is now of size $n$, and the value of $dp[i]$ will be the length of the LIS ending at the $i$th element of the array. For each $i$, we will iterate over all the elements $j$ before it, and if $nums[j] < nums[i]$, we will update $dp[i] = dp[j] + 1$ if $dp[j] \geq dp[i]$ (i.e. if the LIS ending at $j$ is longer than the LIS ending at $i$). The final answer will be the maximum value of the DP table, i.e. the maximum value of $dp[i]$.  
We can also reconstruct an actual LIS. You could keep an extra array $prev$ of size $n$ for this, where the value of $prev[i]$ is the index of the previous element of the LIS ending at the $i$ th element of the array. However, this is not necessary, since we can reconstruct the LIS by iterating backwards from the maximum value of the DP table and taking the first value that is one less than the current value. This works because the lengths that we stored in the table $dp$ can be seen as the number of elements in the LIS ending at the $i$th element of the array and so as indices of the LIS.
For example, for the array $nums$, the final DP table will be $[1, 1, 1, 2, 2, 3, 4, 5, 5, 3, 6]$. The maximum value of the DP table is 6, and the index of this value is 10. So, the LIS ends at index 10. Now we can reconstruct the LIS by iterating backwards from index 10 and taking the first value that is one less than the current value. In this case, the first value that is one less than the current value is 5, and the index of this value is 8. So are current reconstructed LIS now also includes $nums[8] = 15$. Next we iterate backwards from index 8 and take the first value that is one less than the current value. In this case, the first value that is one less than the current value is 4, and the index of this value is 6. So are current reconstructed LIS now also includes $nums[6] = 10$. As we keep doing this, we thus get the final LIS $[1, 2, 8, 10, 15, 20]$. This reconstruction takes $O(n)$ time.

Implementation: [LIS - bottom-up DP (optimized)](https://github.com/pl3onasm/Algorithms/blob/main/algorithms/dynamic-programming/longest-increasing-sub/lis-3.c)

## Optimal Solution in Linearithmic Time

We can actually do better than the previous solutions and solve the LIS problem in $O(n \log n)$ time. The idea is to keep track of the LIS in a separate array, and to update this array as we iterate over the original array. Each time we encounter a new element, we will check if it is greater than the last element of the LIS array. If it is, we will append it to the LIS array. Otherwise, we will find the first element in the LIS array that is greater than or equal to the current element, and replace it with the current element. This way, we will always have a sorted LIS array, and we will only need to update the first element that is greater than or equal to the current element. To find this element, we can use binary search (since the LIS array is sorted at any given point), which takes $O(\log n)$ time. The final length of the LIS array then is the length of the LIS.  

To find an actual LIS, we can keep track of the previous element of the LIS at each step in an extra array $prev$ of size $n$, and we can then iterate backwards from the last element of the LIS to find the actual LIS. This will take $O(n)$ time. However, to make the solution even more efficient, we can also use a single array to keep track of the LIS array and the extra array $prev$ at the same time. We can do so by storing the *index* of the previous element of the LIS at each step in the LIS array, instead of storing the actual value. This way, we will only need to keep track of the LIS array, and there is no need for an extra array $prev$.
For example, for the array $nums$, the LIS array will be $[1, 2, 9, 10, 15, 20]$, and the extra array $prev$ will be $[-1, 0, 2, 3, 4, 5]$. The index of the last element of the LIS array is 5, and the actual LIS is $[1, 2, 9, 10, 15, 20]$. We can then iterate backwards from index 5 to find the actual LIS, using the LIS array to find the previous element of the LIS at each step.

The overall time complexity of this solution is in $O(n \log n)$, since iterating over the array takes $O(n)$ time and each binary search takes $O(\log n)$ time. The space complexity is in $O(n)$, since we need to keep track of the LIS array.  

Implementation: [LIS - optimal solution](https://github.com/pl3onasm/Algorithms/blob/main/algorithms/dynamic-programming/longest-increasing-sub/lis-4.c)
