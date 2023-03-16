# Minimum Edit Distance

## Problem

Given two strings, find the minimum number of operations required to convert one string to another. The operations allowed are:

- Insert a character
- Delete a character
- Replace a character
- Swap two adjacent characters
- Copy a character
- Kill a sequence of characters

## DP Solution

Let `dp[i][j]` be the minimum number of operations required to convert the first `i` characters of the first string to the first `j` characters of the second string. Then, we have the following recurrence:

```python
if s1[i] == s2[j]:
    dp[i][j] = dp[i-1][j-1]
else:
    dp[i][j] = min(
        dp[i-1][j] + 1, # delete
        dp[i][j-1] + 1, # insert
        dp[i-1][j-1] + 1, # replace
        dp[i-2][j-2] + 1 if s1[i] == s2[j-1] and s1[i-1] == s2[j] else inf, # swap
        dp[i-1][j-1] + 1 if s1[i] == s2[j-1] else inf, # copy
        dp[i-1][j] + 1 if s1[i] == s2[j-1] else inf, # kill
    )
```

## Implementation

```python
def min_edit_dist(s1, s2):
    n = len(s1)
    m = len(s2)
    dp = [[0] * (m + 1) for _ in range(n + 1)]
    for i in range(n + 1):
        dp[i][0] = i
    for j in range(m + 1):
        dp[0][j] = j
    for i in range(1, n + 1):
        for j in range(1, m + 1):
            if s1[i - 1] == s2[j - 1]:
                dp[i][j] = dp[i - 1][j - 1]
            else:
                dp[i][j] = min(
                    dp[i - 1][j] + 1, # delete
                    dp[i][j - 1] + 1, # insert
                    dp[i - 1][j - 1] + 1, # replace
                    dp[i - 2][j - 2] + 1 if i >= 2 and j >= 2 and s1[i - 1] == s2[j - 2] and s1[i - 2] == s2[j - 1] else float('inf'), # swap
                    dp[i - 1][j - 1] + 1 if i >= 1 and j >= 2 and s1[i - 1] == s2[j - 2] else float('inf'), # copy
                    dp[i - 1][j] + 1 if i >= 1 and j >= 2 and s1[i - 1] == s2[j - 2] else float('inf'), # kill
                )
    return dp[n][m]
```
