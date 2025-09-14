$\huge{\color{Cadetblue}\text{Binary Search}}$

<br/>

$\Large{\color{rosybrown}\text{Problem}}$

Given a sorted array $A$ of $n$ elements, find a given element $x$ in $A$ in $\Theta(\log n)$ time.

<br/>

$\Large{\color{darkseagreen}\text{Solution}}$

The fact that the array is sorted is crucial to designing an algorithm in which we can search for $x$ in $\Theta(\log n)$ time. For this, we compare $x$ with the middle element $A[\lfloor n/2 \rfloor]$. If $x$ is equal to the middle element, we are done. If $x$ is less than the middle element, we search the left half of the array, and in the other case, we search the right half of the array. This process is repeated until we find $x$ or have narrowed the search to an empty subarray, in which case we conclude that $x$ is not in the array.

Since the array is halved at each step and comparing takes constant time, the recurrence for the running time is given by:

$$
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = T(n/2) + \Theta(1) \space}
$$

According to the master theorem, case 2, the solution is then $T(n) = \Theta(\log n)$.

<br/>

$\Large{\color{yellowgreen}\text{Example 1:}}$ $\Large{\color{olive} \text{n-th magical number}}$

Binary search can also be used to search a ${\color{darkseagreen}\text{solution space}}$. For example, consider the problem of finding the $n$-th magical number, given two positive integers $a$ and $b$. A magical number is a number that is divisible by either $a$ or $b$. We can use a binary search in this case because the set of magical numbers is ${\color{darkseagreen}\text{monotonically increasing}}$ (forming a virtual sorted array we can search through), and we are able to set upper and lower bounds on the solution space.

Since a magical number is divisible by either $a$ or $b$, the sharpest upper bound can be set to $n \cdot \min(a, b)$, and the lower bound to 1. At each step, we then check the middle number, and count the number of magical numbers up to and including the middle number. This total number of magical numbers can be computed by simply taking the sum of the quotients of the middle number divided by $a$ and $b$ minus the quotient of the middle number divided by the least common multiple of $a$ and $b$ (subtracting this last term is necessary to account for the fact that otherwise we would have counted the numbers divisible by *both* $a$ and $b$ twice: this is in fact an application of the principle of ${\color{darkseagreen}\text{inclusion-exclusion}}$).  

If the found number of magical numbers is less than $n$, we know that the $n$-th magical number is greater than the current middle number, and we set the lower bound to the middle number plus 1 and search the right half of the solution space. If the number of magical numbers is greater than $n$, we set the upper bound to the middle number and search the left half of the solution space. This process is repeated until we find the number of magical numbers to be exactly $n$.

Implementation: [Magical numbers](magic/magic.c)

<br/>

$\Large{\color{yellowgreen}\text{Example 2:}}$ $\Large{\color{olive} \text{Aggressive cows}}$

In the following problem, we want to find the ${\color{darkseagreen}\text{upper bound}}$ on the solution space, i.e. the pivot point above which there are no solutions anymore. For this, we use a function that specifically checks whether the new mid point still yields a solution and then refine the upper bound accordingly.

The problem involves $k$ aggressive cows and $n$ stalls. Each stall can hold at most one cow, and each cow must be put into a stall. The stalls are located on a straight line at positions $x_1, x_2, \dots, x_n$, and the goal is to assign the cows to the stalls such that the minimum distance between any two of them is as large as possible and the cows, being aggressive, are as far apart as possible and cannot attack each other.  

The solution space consists of the increasing sequence of minimum distances $d_i$ between the stalls. The lower bound is 0, and the upper bound is the distance between the first and last stall. Using a binary search, we can then find the largest distance $d$ such that each cow can be assigned to a stall and that the distance between any two of them is *at least* $d$.

Implementation: [Agressive cows](cows/cows.c)

<br/>

$\Large{\color{yellowgreen}\text{Example 3:}}$ $\Large{\color{olive} \text{Minimum maximum sum}}$

An example in which we want to find the ${\color{darkseagreen}\text{lower bound}}$ of the solution space is the problem of splitting an array into $k$ subarrays such that the maximum sum of the elements in each subarray is minimized. An implementation for $k = 3$ can be found here: [3-split](https://github.com/pl3onasm/Imperative-programming/blob/main/IP-Finals/2018/problem4/prob4-2.c).
