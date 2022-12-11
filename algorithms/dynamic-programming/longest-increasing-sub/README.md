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
Using the bottom-up approach we developed for the LCS problem, we can build a DP table $dp$ of size $n \times n$, where $n$ is the length of the array. The value of $dp[i][j]$ will be the length of the LCS between the first $i$ elements of the array and the first $j$ elements of the sorted array. The value of $dp[i][j]$ will be $dp[i-1][j-1] + 1$ if $nums[i-1] = sorted[j-1]$, and $\max(dp[i-1][j], dp[i][j-1])$ otherwise. The final answer will be the value of $dp[n][n]$.  
The time complexity of this solution is in $O(n^2)$, since sorting the array takes $O(n \log n)$ time and building the DP table takes $O(n^2)$ time. The space complexity is also in $O(n^2)$.

Implementation: [LIS - bottom-up DP (LCS)](https://github.com/pl3onasm/Algorithms/blob/main/algorithms/dynamic-programming/longest-increasing-sub/lis-2.c)

## Bottom-up DP (Optimized)

The previous solution does quite a bit of unnecessary work (such as sorting and removing duplicates). We can develop a DP bottom-up approach for the LIS problem where we only work with the original array, without a sorted copy, and we can also further reduce the space complexity by using only one row of the $dp$ table at a time, since we only need the previous row to compute the next row. For this we need to keep track of the maximum value of the $dp$ table at each iteration. The time complexity of this solution is still in $O(n^2)$, since building the $dp$ table takes $O(n^2)$ time. The space complexity, however, is now in $O(n)$.  
So, the table dp is now of size $n$, and the value of $dp[i]$ will be the length of the LIS ending at the $i$ th element of the array. For each $i$, we will iterate over all the elements $j$ before it, and if $nums[j] < nums[i]$, we will update $dp[i] = dp[j] + 1$ if $dp[j] \geq dp[i]$ (i.e. if the LIS ending at $j$ is longer than the LIS ending at $i$). The final answer will be the maximum value of the $dp$ table, i.e. the maximum value of $dp[i]$.  
We can also reconstruct an actual LIS. We can reconstruct the LIS by iterating backwards from the maximum value of the $dp$ table and taking the first value that is one less than the current value. This works because the lengths that we stored in the table $dp$ can be seen as the number of elements in the LIS ending at the $i$ th element of the array and so as indices of the LIS elements.  
For example, for the array $nums$, the final $dp$ table will be $[1, 1, 1, 2, 2, 3, 4, 5, 5, 3, 6]$. The maximum value of the $dp$ table is 6, and the index of this value is 10. So, the LIS ends at index 10: we add $nums[10] = 20$ to it. Now we can reconstruct the rest of the LIS by iterating backwards from index 10 and taking the first value that is one less than the current value. In this case, that value is 5, and the index of this value is 8. So our current reconstructed LIS now also includes $nums[8] = 15$. Next we iterate backwards from index 8 and take the first value that is one less than the current value. That value is 4, and the index of this value is 6. So are current reconstructed LIS now also includes $nums[6] = 10$. As we keep doing this, we thus get the final LIS = $[1, 2, 9, 10, 15, 20]$. This reconstruction takes $O(n)$ time.

Implementation: [LIS - bottom-up DP (optimized)](https://github.com/pl3onasm/Algorithms/blob/main/algorithms/dynamic-programming/longest-increasing-sub/lis-3.c)

## Optimal Solution in Linearithmic Time

We can actually do better than the previous solutions and solve the LIS problem in $O(n \log n)$ time. Just like before, the idea is to use an array $dp$ of size $n$, and to update it as we iterate over the elements of the original array. However, this time we will not store the length of the LIS ending at the $i$ th element of the array, but rather the smallest element that ends a LIS of length $i$.  
Each time we take a new element from the original array, we will check if it is greater than the last element of our current $dp$ array. If it is, we will append it to $dp$. Otherwise, we will find the first element in the $dp$ array that is greater than or equal to the current element, and replace it with the current element. This way, we will always have a sorted $dp$ array, and we will only need to update the first element that is greater than or equal to the current element. To find this element and update it, we can use binary search (since the array is sorted at any given point), which takes $O(\log n)$ time. The final length of the $dp$ array then is the length of the LIS.  

To find an actual LIS, we can keep an extra array $prev$ of size $n$. In order to keep things efficient when we also want to reconstruct a LIS and are not merely interested in its length, we should keep indexes instead of actual values in both the $dp$ and $prev$ arrays. We then update the $prev$ array, each time we update the $dp$ array by setting the value of $prev[i]$ to the value stored at the index one below the index where we added the current element to the $dp$ array. If that index happens to be 0, we set $prev[i] = -1$, to indicate that there is no previous element and that we reached the beginning of the LIS. When we are done updating the $dp$ array, the last element of the $dp$ array will be the last element of the LIS. So, we can reconstruct the LIS by iterating backwards from the index of the last element of the $dp$ array and taking the value of $prev[i]$ at each step, until we reach an index -1.  
For our example array $nums$, the final state of the $dp$ array will be $[2, 4, 9, 6, 8, 10]$, having length 6 which is the length of the LIS, and the $prev$ array will be $[-1, -1, -1, 2, 2, 4, 5, 6, 6, 4, 8]$. The last element of the $dp$ array is 10, so we add $nums[10] = 20$ to the LIS. Next we start reconstructing the rest of the LIS from index 10 in the $prev$ array. The value of $prev[10] = 8$, so we prepend $nums[8] = 15$ to the LIS. Next we see that $prev[8] = 6$, so we prepend $nums[6] = 10$ to the LIS. We keep doing this until we reach a value -1, and thus get the final LIS $[1, 2, 9, 10, 15, 20]$. This reconstruction takes $O(n)$ time.

Implementation: [LIS - optimal solution](https://github.com/pl3onasm/Algorithms/blob/main/algorithms/dynamic-programming/longest-increasing-sub/lis-4.c)
