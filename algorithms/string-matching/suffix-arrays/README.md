$\huge{\color{Cadetblue}\text{Suffix arrays}}$

<br/>

If, besides finding occurrences of a pattern in a text, we are also interested in finding things like the longest repeated substring, the number of distinct substrings, the longest common substring between two texts, or the longest palindromic substring in a text, we can use a data structure called a ${\color{mediumorchid}\text{suffix array}}$, denoted $SA[0:n-1]$. It is a permutation of the starting indices $0,1,\dots,n-1$ such that the corresponding suffixes of a given string $T[0:n-1]$ occur in lexicographic order. In other words,

$$
T[SA[0]:n-1] < T[SA[1]:n-1] < \dots < T[SA[n-1]:n-1].
$$

Thus, we do not store the suffixes themselves, but only the starting index of each suffix in the original string. For example, the suffix array for the string $T=$ `banana` is the second column in the table below.

Along with the suffix array, we also want to compute the ${\color{mediumorchid}\text{LCP array}}$, which yields the longest common prefix between adjacent suffixes in the suffix array, i.e. the length of the longest common prefix between $T[SA[i-1]:n-1]$ and $T[SA[i]:n-1]$ for $i=1,2,\ldots,n-1$. We define $LCP[0]=0$. For example, the longest common prefix between the suffixes `ana` and `anana` is `ana`, so $LCP[2]=3$, and the longest common prefix between the suffixes `banana` and `na` is the empty string, so $LCP[4]=0$. The third column in the table below is the LCP array for the string `banana`.

&nbsp;

<div align="center">

${\color{peru}\text{i}}$ | ${\color{peru}\text{SA[i]}}$ | ${\color{peru}\text{LCP[i]}}$ | ${\color{peru}\text{suffix}}$ |
-------|:-----:|:------:|:-------|
0 | 5 | 0 | a |
1 | 3 | 1 | ana |
2 | 1 | 3 | anana |
3 | 0 | 0 | banana |
4 | 4 | 0 | na |
5 | 2 | 2 | nana |

</div>

<br/>

$\Large{\color{darkseagreen}\text{Applications}}$

<br/>

$\Large{\color{rosybrown}\text{1. String matching}}$

After constructing the suffix array, we can find all occurrences of a pattern $P[0:m-1]$ in a text $T[0:n-1]$ by performing a ${\color{mediumorchid}\text{binary search}}$ on the suffix array. Since the suffix array is lexicographically sorted, all suffixes that start with the pattern $P$ occur in one contiguous interval.

For example, if we want to find all occurrences of the pattern `ana` in the text `banana`, we can use one binary search to find the first suffix that starts with `ana` and a second binary search to find the first suffix that is lexicographically greater than the matching range. These two boundaries identify the matching suffixes at suffix-array indices 1 and 2.

Each comparison between $P$ and a suffix takes $\mathcal{O}(m)$ time in the worst case, and each binary search performs $\mathcal{O}(\log n)$ comparisons. Thus, the matching interval can be found in $\mathcal{O}(m\log n)$ time. If there are $k$ occurrences, reporting their starting indices takes an additional $\mathcal{O}(k)$ time, so the total running time is ${\color{cornflowerblue}\mathcal{O}(m\log n + k)}$.

The LCP array can be used to reduce repeated character comparisons during the searches, but it is not required to obtain the above bound.[^1]

[^1]: If the actual occurrence positions are returned, an additional $\Omega(k)$ output cost is unavoidable.

<br/>

$\Large{\color{rosybrown}\text{2. Longest repeated substring}}$

We can also find a longest repeated substring in a text by finding the maximum value in the LCP array. If this maximum value occurs at index $i$, then a longest repeated substring is the substring starting at index $SA[i]$ with length $LCP[i]$. In the example above, the maximum value in the LCP array is 3. Thus, a longest repeated substring starts at $SA[2]=1$ and has length $LCP[2]=3$, and is the substring `ana`.

If all distinct longest repeated substrings are required, equal substrings arising from several positions with the same maximum LCP value may have to be deduplicated.

<br/>

$\Large{\color{rosybrown}\text{3. Number of distinct substrings}}$

Another thing we can easily compute is the cardinality of the set containing all distinct substrings of the text. The suffix starting at $SA[i]$ has length $n-SA[i]$, so it contributes that many prefixes as candidate substrings. However, its first $LCP[i]$ prefixes already occur among lexicographically preceding suffixes and therefore do not contribute new distinct substrings. Hence the suffix contributes

$$
(n-SA[i])-LCP[i]
$$

new distinct substrings.

Summing over all suffixes gives

$$
\sum_{i=0}^{n-1}(n-SA[i]-LCP[i])
=
{\color{cornflowerblue}\frac{n(n+1)}{2}-\sum_{i=0}^{n-1}LCP[i]}.
$$

In the example above, the number of distinct substrings is

$$
\frac{6\cdot7}{2}-(0+1+3+0+0+2)=15.
$$

This is indeed the cardinality of the set: {`b`, `a`, `n`, `ba`, `an`, `na`, `ban`, `ana`, `nan`, `bana`, `anan`, `nana`, `banan`, `anana`, `banana`}.

<br/>

$\Large{\color{rosybrown}\text{4. Longest palindromic substring}}$

Yet another application is finding the longest palindromic substring in $T$. For this, we concatenate the text with a separator character that does not occur in the text and with the reverse of the text:

$$
S=T\#T^R.
$$

We then construct the suffix array and LCP array for $S$. The separator prevents a common prefix from crossing the boundary between $T$ and $T^R$.

A palindrome can be detected by comparing a suffix from the original half with the appropriately mirrored suffix from the reversed half. For an odd-length palindrome centered at position $c$, we compare the suffix of $S$ starting at $c$ with the suffix starting at $2n-c$. If their longest common prefix has length $l$, then the palindrome centered at $c$ has length $2l-1$. For an even-length palindrome centered between positions $c-1$ and $c$, we compare the suffix starting at $c$ with the suffix starting at $2n+1-c$; if their longest common prefix has length $l$, then the palindrome has length $2l$.

The LCP of any two suffixes can be obtained from the suffix array and LCP array by taking the minimum LCP value between their two positions in suffix-array order; this can be supported efficiently with a range-minimum-query data structure. Checking all possible centers then yields the longest palindromic substring.

For example, if $T=$ `banana`, then

```text
banana#ananab
```

is used. The longest palindromic substring is `anana`, with length 5.

<br/>

$\Large{\color{darkseagreen}\text{Building the suffix array}}$

The suffix array can be constructed in $\mathcal{O}(n\log n)$ time using the prefix-doubling approach associated with Manber and Myers. The idea is to repeatedly double the number of characters by which the suffixes are ranked. Starting with prefixes of length 1, the algorithm ranks the suffixes by prefixes of length

$$
1,2,4,8,\dots
$$

until the considered prefix length is at least $n$.

Initially, each suffix is ranked according to its first character. Suppose that, in some iteration, the suffixes have already been ranked according to their first $\ell$ characters. To rank them according to their first $2\ell$ characters, suffix $i$ is represented by the pair

$$
(rank[i],rank[i+\ell]),
$$

where the first component is the rank of its first half and the second component is the rank of its second half. If $i+\ell\geq n$, a special sentinel rank smaller than every ordinary rank is used for the missing second half.

These pairs can be sorted in $\mathcal{O}(n)$ time because the ranks are integers in a range of size $\mathcal{O}(n)$. A stable counting sort is first performed on the second rank and then on the first rank. The second pass must be stable so that, among suffixes with the same first rank, the relative ordering by second rank is preserved. New ranks are then assigned according to the resulting ordering, $\ell$ is doubled, and the process repeats until $\ell\geq n$.

Since each iteration takes $\mathcal{O}(n)$ time and there are $\mathcal{O}(\log n)$ iterations, the total construction time is ${\color{cornflowerblue}\mathcal{O}(n\log n)}$.

<br/>

$\Large{\color{darkseagreen}\text{Building the LCP array}}$

Building the LCP array directly in lexicographic order can take $\mathcal{O}(n^2)$ time, since adjacent suffixes may need to be compared character by character from the beginning. However, it is possible to build the LCP array in $\mathcal{O}(n)$ time with Kasai's algorithm by processing the suffixes in ${\color{peru}\text{text order}}$, i.e. in the order of their starting positions in the original text.

Suppose the suffix $T[i:n-1]$ shares a common prefix of length $l$ with its lexicographic predecessor. After removing the first character from both suffixes, the resulting suffixes still share a prefix of length at least $l-1$. This implies that, when moving from suffix $T[i:n-1]$ to suffix $T[i+1:n-1]$, the next LCP computation can start with a known lower bound of $\max(l-1,0)$ instead of starting again at 0.

To use this efficiently, we construct an inverse suffix array $rank$ such that $rank[i]$ gives the position of suffix $T[i:n-1]$ in the suffix array. We then traverse the suffixes in text order. For each suffix, we compare it with its lexicographic predecessor starting from the retained LCP length, extend the match until a mismatch is found, store the resulting LCP value, and decrement the retained length by one before moving to the next suffix.

Although the inner comparison loop may advance several characters during one iteration, the retained LCP length decreases by at most one between successive suffixes. Consequently, over the entire algorithm it can increase only $\mathcal{O}(n)$ times. Thus, the complete LCP array is constructed in ${\color{cornflowerblue}\mathcal{O}(n)}$ time.

<br/>

$\Large{\color{darkseagreen}\text{Implementation}}$

The implementation reads a text from standard input and computes the suffix array and LCP array for it. It then reads a pattern from standard input and finds all occurrences of the pattern in the text. It also finds the longest repeated substring, the number of distinct substrings, and the longest palindromic substring in the text.

Implementation: [Suffix array](https://github.com/pl3onasm/CLRS/blob/main/algorithms/string-matching/suffix-arrays/sa.c)

<br/>

$\Large{\color{darkseagreen}\text{Playlist}}$

[![Suffix arrays](https://img.youtube.com/vi/zqKlL3ZpTqs/0.jpg)](https://www.youtube.com/watch?v=zqKlL3ZpTqs&list=PLDV1Zeh2NRsCQ_Educ7GCNs3mvzpXhHW5)
