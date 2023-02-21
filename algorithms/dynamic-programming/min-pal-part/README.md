# Minimal Palindrome Partitioning

Given a string, we can cut it into pieces, called substrings, thus creating a certain partitioning of the string. A partitioning of a string is a palindrome partitioning if every substring of the partition is a palindrome. For example, aba|b|bbabb|a|b|aba is a palindrome partitioning of ababbbabbababa, where each | denotes a partition boundary or a cut. The given partitioning has 5 cuts. The goal, however, is to find the palindrome partitioning with the fewest cuts. For the example above, the minimal palindrome partitioning has 3 cuts. The three cuts are a|babbbab|b|ababa. This partitioning is not unique, however. Another minimal palindrome partitioning is a|bab|bbabb|ababa.
If the entire string is a palindrome on its own, then the minimum number of cuts is obviously zero. If a string of $n$ characters contains no palindromes at all, then the minimum number of cuts is $n-1$: each character is a palindrome on its own, and the cuts are made between each pair of adjacent characters.  
The input to the program is a string of $n$ characters. The output is the minimum number of cuts needed to partition the string into palindromes, as well as the indices in the string where the cuts should be made.  

## Brute Force

The brute force approach is to try all possible partitions of the string and check if each partition is a palindrome. The time complexity of this approach is exponential. 