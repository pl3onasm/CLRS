$\huge{\color{Cadetblue}\text{The Recursion Tree Method}}$

<br/>

The recursion tree method is a way to visualize the recurrence relation by drawing it as a tree. Each node represents the cost of a single subproblem, and the edges represent the recursive calls. By summing the costs at each level of the tree, we can often find a closed-form solution for the recurrence.

This technique is often used to make an educated guess about the solution, which can then be verified using the substitution method. 

<br/>

--------------------------------------------------------------------

$\Large{\color{cornflowerblue}\text{Example 1}}$

<br/>

Consider the recurrence for merge sort:

$$\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = 2T(n/2) + \Theta(n) \space}$$

<br/>

The recursion tree for this recurrence is as follows:

```text
                         T(n)
               ___________│___________
              ╱                       ╲
           T(n/2)                    T(n/2)
           ╱    ╲                    ╱    ╲
       T(n/4)   T(n/4)           T(n/4)  T(n/4)  
        ...       ...             ...      ...     
       ╱   ╲     ╱   ╲           ╱   ╲    ╱   ╲
     T(1) T(1) T(1) T(1) ..... T(1) T(1) T(1) T(1)
```

Each node at level $i$ contributes a cost proportional to the subproblem size, and there are $2^i$ nodes each of size $n/2^i$, so the total cost per level is $2^i \cdot c(n/2^i) = cn$. Assuming that the base case $T(1) = \Theta(1)$, the resulting cost structure per level is shown below:

```text
                         cn                         = cn
               ___________│___________
              ╱                       ╲
           c(n/2)                    c(n/2)         = cn
           ╱    ╲                    ╱    ╲
       c(n/4)   c(n/4)           c(n/4)  c(n/4)     = cn  
        ...       ...             ...      ...     
       ╱   ╲     ╱   ╲           ╱   ╲    ╱   ╲
     Θ(1) Θ(1) Θ(1) Θ(1) ..... Θ(1) Θ(1) Θ(1) Θ(1)  = cn = Θ(n)
```

<br/>  

This cost analysis can be summarized in the following table:

| Level | Number of Nodes | Cost per Node | Total Cost per Level |
|:------:|:----------------:|:---------------:|:----------------------:|
| $0$ | $1$ | $c \cdot n$ | $c \cdot n$ |
| $1$ | $2$ | $c \cdot n/2$ | $c \cdot n$ |
| $2$ | $4$ | $c \cdot n/4$ | $c \cdot n$ |
| ... | ... | ... | ... |
| $\log(n)$ | $n$ | $c \cdot 1$ | $c \cdot n$ |
| TOTAL | | | $c \cdot n \cdot (\log(n) + 1)$ |

<br/>

Since there are $\log(n + 1)$ levels in the tree, the total cost is:

$$
\begin{align*}
T(n) & = cn \cdot (\log(n) + 1) \\
& = cn \log(n) + cn \\
& = \Theta(n \space \log(n))
\end{align*}
$$

Therefore, we can guess that the solution to the recurrence is:

$$\color{mediumorchid}\boxed{\color{mediumpurple}\space T(n) = \Theta(n \space \log (n)) \space}$$

<br/>

$\color{lightslategray}{\textbf{Note}}$  
The recursion tree method visualizes how the total work is distributed across the levels of recursion. For divide-and-conquer recurrences such as merge sort, every level contributes an equal amount of work, leading to the characteristic $\Theta(n \log n)$ running time.

<br/>

--------------------------------------------------------------------

$\Large{\color{cornflowerblue}\text{Example 2}}$

<br/>

The recursion tree can also degenerate into a linear chain when the problem size decreases by a constant amount rather than by a fixed fraction. Consider the recurrence:

$$\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = T(n - 1) + \Theta(n^2) \space}$$

<br/>

Each node at level $i$ in the recursion tree contributes a cost proportional to the subproblem size, and there is only 1 node at each level, so the total cost per level is $1 \cdot c(n - i)^2 = c(n - i)^2$. Assuming that the base case $T(1) = \Theta(1)$, the resulting recursion tree along with its cost structure per level is shown below:

```text
T(n)                         = cn²
  │                    
  └ T(n-1)                   = c(n-1)²
      │
      └ T(n-2)               = c(n-2)²
          │
          └ T(n-3)           = c(n-3)²
              │
              └ ...          ...                    
                 │
                 └ T(1)      = Θ(1)
```

<br/>

This cost analysis can be summarized in the following table:

| Level | Number of Nodes | Cost per Node | Total Cost per Level |
|:------:|:----------------:|:---------------:|:----------------------:|
| $0$ | $1$ | $c \cdot n^2$ | $c \cdot n^2$ |
| $1$ | $1$ | $c \cdot (n-1)^2$ | $c \cdot (n-1)^2$ |
| $2$ | $1$ | $c \cdot (n-2)^2$ | $c \cdot (n-2)^2$ |
| ... | ... | ... | ... |
| $n-2$ | $1$ | $c \cdot 2^2$ | $c \cdot 2^2$ |
| $n-1$ | $1$ | $c \cdot 1^2$ | $c \cdot 1^2$ |
| TOTAL | | | $c \cdot \sum_{i=1}^{n} i^2$ |

<br/>

The total cost is:

$$
\begin{align*}
T(n) & = c \space \sum_{i=1}^{n} i^2 \\
& = c \cdot \frac{n(n+1)(2n+1)}{6} \\
& = \Theta(n^3)
\end{align*}
$$

Therefore, we can guess that the solution to the recurrence is:

$$\color{mediumorchid}\boxed{\color{mediumpurple}\space T(n) = \Theta(n^3) \space}$$

<br/>

$\color{lightslategray}{\textbf{Note}}$  
In this recurrence, each recursive call decreases the argument by one, producing a linear chain of subproblems. The work per level decreases quadratically, forming an arithmetic series of squares whose total cost grows as $\Theta(n^3)$.

<br/>

--------------------------------------------------------------------

$\Large{\color{cornflowerblue}\text{Example 3}}$

<br/>

Consider the recurrence:

$$\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = 3T(n/5) + \Theta(n) \space}$$

<br/>

The recursion tree for this recurrence is as follows:

```text
                                             T(n)
                    __________________________│__________________________    
                   ╱                          │                          ╲ 
            ____T(n/5)____             ____ T(n/5)____              ____T(n/5)____
           ╱      │       ╲           ╱       │       ╲            ╱       │      ╲         
      T(n/25)  T(n/25)  T(n/25)   T(n/25)  T(n/25)   T(n/25)   T(n/25)  T(n/25)  T(n/25)
        ...      ...     ...        ...      ...       ...      ...       ...      ...     
      ╱  │  ╲    ...   ╱  │  ╲      ...    ╱  │  ╲     ...    ╱  │  ╲     ...    ╱  │  ╲
  T(1) T(1) T(1) ... T(1) T(1) T(1) ... T(1) T(1) T(1) ... T(1) T(1) T(1) ... T(1) T(1) T(1)
```

<br/>

Each node at level $i$ contributes a cost proportional to the subproblem size, and there are $3^i$ nodes each of size $n/5^i$, so the total cost per level is $3^i \cdot c(n/5^i) = cn(3/5)^i$. Assuming that the base case $T(1) = \Theta(1)$, the resulting cost analysis can be summarized in the following table:

| Level | Number of Nodes | Cost per Node | Total Cost per Level |
|:------:|:----------------:|:---------------:|:----------------------:|
| $0$ | $1$ | $c \cdot n$ | $c \cdot n$ |
| $1$ | $3$ | $c \cdot (n/5)$ | $c \cdot n \cdot (3/5)$ |
| $2$ | $9$ | $c \cdot (n/25)$ | $c \cdot n \cdot (3/5)^2$ |
| ... | ... | ... | ... |
| $k$ | $3^k$ | $c \cdot (n/5^k)$ | $c \cdot n \cdot (3/5)^k$ |
| TOTAL | | | $c \cdot n \cdot \sum_{i=0}^{\log_5 n} (3/5)^i$ |

<br/>

The total cost is:

$$
\begin{align*}
T(n) &= cn \space \sum_{i=0}^{\log_5 n} \left(\frac{3}{5}\right)^i \\
&= cn \cdot \frac{1 - (3/5)^{\log_5 n + 1}}{1 - (3/5)} \\
&= \frac{5c}{2}\space n \left(1 - \frac{3}{5} \space n^{\log_5(3/5)}\right) \\
&= \Theta(n)
\end{align*}
$$

Therefore, we can guess that the solution to the recurrence is:

$$\color{mediumorchid}\boxed{\color{mediumpurple}\space T(n) = \Theta(n) \space}$$  

<br/>

$\color{lightslategray}{\textbf{Note}}$  
In this recurrence, each level performs only a fraction of the work of the previous one because the branching factor (3) is smaller than the reduction factor (5).  
The geometric decay of per-level cost causes the total work to be dominated by the top level, resulting in $\Theta(n)$ overall.

<br/>

--------------------------------------------------------------------

$\Large{\color{cornflowerblue}\text{Example 4}}$

<br/>

Consider the recurrence:

$$\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = T(n/2) + T(n/4) + \Theta(n) \space}$$

<br/>

The recursion tree for this recurrence is as follows:

```text
                                       T(n)
                       _________________│_________________
                      ╱                                   ╲
              ____T(n/2)____                        ____T(n/4)___ 
             ╱              ╲                      ╱             ╲
         T(n/4)           T(n/8)               T(n/8)          T(n/16)  
         ╱    ╲           ╱    ╲               ╱    ╲          ╱     ╲
       ...    ...       ...    ...           ...     ...     ...     ...
       ╱                         ╲          ╱   ╲                   ╱   ╲
     ...      .....   .....      ...      ...   ...  ..... .....  T(1) T(1) 
     ╱                ╱   ╲              ╱   ╲      ......
   ...        ...... T(1) T(1)          T(1) T(1)
  ╱   ╲      ......              
T(1) T(1)  
```

<br/>

Each node at level $i$ contributes a cost proportional to the subproblem size, and the number of nodes at each level follows a pattern based on the recursive calls. At level $i$, the total cost can be expressed as $c \cdot n \cdot (3/4)^i$. The resulting cost analysis can be summarized as follows:

| Level | Number of Nodes | Costs per Node | Total Cost per Level |
|:------:|:----------------:|:---------------:|:----------------------:|
| $0$ | $1$ | $c \cdot n$ | $c \cdot n$ |
| $1$ | $2$ | $c \cdot \lbrace n/2, n/4 \rbrace$ | $c \cdot n \cdot (3/4)$ |
| $2$ | $4$ | $c \cdot \lbrace n/4, n/8, n/8, n/16 \rbrace$ | $c \cdot n \cdot (9/16)$ |
| $3$ | $8$ | $c \cdot \lbrace n/8, n/16, n/16, n/16,$ <br/> $n/32, n/32, n/32, n/64 \rbrace$ | $c \cdot n \cdot (27/64)$ |
| ... | ... | ... | ... |
| $k$ | $2^k$ | | $c \cdot n \cdot (3/4)^k$ |

<br/>

For the first $\lfloor \log_4 n \rfloor$ levels, every node exists, and so the per-level cost is exactly $c \cdot n \cdot (3/4)^i$. Beyond that point, some nodes disappear because their subproblem sizes fall below $1$.

The deepest level occurs when $n / 2^i = 1$, i.e. at $i = \log_2 n$. This is a unique all-halves path down the left side of the tree. The shallowest level occurs when $n / 4^i = 1$, i.e. at $i = \log_4 n$. This is a unique all-quarters path down the right side of the tree.

Thus, we know that $T(n)$ is bounded from above by $cn \sum_{i=0}^{\log_2 n} (3/4)^i$ and from below by $cn \sum_{i=0}^{\log_4 n} (3/4)^i$. Let us analyze the upper bound:

$$
\begin{align*}
T(n) & = cn \sum_{i=0}^{\log_2 n} \left(\frac{3}{4}\right)^i \\
&= cn \cdot \frac{1 - (3/4)^{\log_2 n + 1}}{1 - (3/4)} \\
&= 4cn \left(1 - \frac{3}{4}\space n^{\log_2(3/4)}\right) \\
&= \Theta(n)
\end{align*}
$$

We see that the logarithmic term vanishes because $\log_2(3/4)$ is negative. A similar analysis for the lower bound also yields $\Theta(n)$. Since both bounds are in $\Theta(n)$, we conclude that the solution to the recurrence is:

$$\color{mediumorchid}\boxed{\color{mediumpurple}\space T(n) = \Theta(n) \space}$$

<br/>

$\color{lightslategray}{\textbf{Note}}$  
This recurrence mixes two shrinking factors, $\tfrac{1}{2}$ and $\tfrac{1}{4}$. Despite its irregular branching, the per-level cost decreases geometrically by $(3/4)^i$, leading to a total work linear in $n$. The recursion tree method thus remains effective even for mixed-branch recurrences.

<br/>

--------------------------------------------------------------------

$\Large{\color{cornflowerblue}\text{Example 5}}$

<br/>

Consider the recurrence:

$$\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = 2T(n - 1) + \Theta(1) \space}$$

<br/>

The recursion tree for this recurrence is as follows:

```text
                         T(n)
               ___________│___________
              ╱                       ╲
           T(n-1)                    T(n-1)
           ╱    ╲                    ╱    ╲
       T(n-2)   T(n-2)           T(n-2)  T(n-2)  
        ...       ...             ...      ...     
       ╱   ╲     ╱   ╲           ╱   ╲    ╱   ╲
     T(1) T(1) T(1) T(1) ..... T(1) T(1) T(1) T(1)
```

<br/>

Each node at level $i$ contributes a constant cost $c$, and there are $2^i$ nodes at that level. Thus, the total cost per level is $2^i \cdot c = c \cdot 2^i$. Assuming that the base case $T(1) = \Theta(1)$, the resulting cost analysis can be summarized as follows:

| Level | Number of Nodes | Cost per Node | Total Cost per Level |
|:------:|:----------------:|:---------------:|:----------------------:|
| $0$ | $1$ | $c$ | $c$ |
| $1$ | $2$ | $c$ | $c \cdot 2$ |
| $2$ | $4$ | $c$ | $c \cdot 4$ |
| ... | ... | ... | ... |
| $n - 1$ | $2^{n-1}$ | $c$ | $c \cdot 2^{n-1}$ |
| TOTAL | | | $c \cdot \sum_{i=0}^{n-1} 2^i$ |

<br/>

The deepest level occurs when $n - i = 1$, i.e. at $i = n - 1$. Thus, the total cost is:

$$
\begin{align*}
T(n) & = c \sum_{i=0}^{n-1} 2^i \\
&= c \cdot (2^n - 1) \\
&= \Theta(2^n)
\end{align*}
$$

Therefore, the solution to the recurrence is:

$$\color{mediumorchid}\boxed{\color{mediumpurple}\space T(n) = \Theta(2^n) \space}$$

<br/>

$\color{lightslategray}{\textbf{Note}}$  
Branching doubles at every level while the problem size decreases only by $1$. The recursion tree therefore has exponential width and linear depth, yielding an exponential total number of nodes and $\Theta(2^n)$ time.
