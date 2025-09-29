$\huge{\color{Cadetblue}\text{The Master Method}}$

The master method provides a straightforward way to obtain asymptotic bounds for a broad class of divide-and-conquer recurrences of the following form:

$$
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = aT(n/b) + f(n) \space}
$$  

where $a \geq 1$ and $b > 1$ are constants, and $f(n)$ is called the $\color{darkseagreen} \text{driving function}$, which describes the cost of the work done outside the recursive calls, i.e. the cost of dividing the problem and combining the results of the subproblems. The number of subproblems is given by $a$, and the size of each subproblem is given by $n/b$. So, in order to apply the master method, the recurrence must be in this specific form, with a constant number of subproblems of equal size, and a driving function that is asymptotically positive.

The master method consists of three cases, which are determined by comparing the driving function $f(n)$ to the function $n^{\log_b a}$, which represents the total cost of the recursive calls at the leaves of the recursion tree ($\log_b a$ is the height of the tree, and $n^{\log_b a}$ is the number of leaves). The three cases are as follows:

1. If $f(n) = \mathcal{O}(n^{\log_b a - \epsilon})$ for some constant $\epsilon > 0$, then $T(n) = \Theta(n^{\log_b a})$. This case applies when the driving function is polynomially smaller than $n^{\log_b a}$, meaning that the cost of the work done outside the recursive calls is dominated by the cost of the recursive calls themselves.
2. If $f(n) = \Theta(n^{\log_b a} \log^k n)$ for some constant $k \geq 0$, then $T(n) = \Theta(n^{\log_b a} \log^{k+1} n)$. This case applies when the driving function is asymptotically equal to $n^{\log_b a}$ up to a logarithmic factor, meaning that the cost of the work done outside the recursive calls is comparable to the cost of the recursive calls themselves.  
3. If $f(n) = \Omega(n^{\log_b a + \epsilon})$ for some constant $\epsilon > 0$, and if the regularity condition $af(n/b) \leq c \cdot f(n)$ holds for some constant $c< 1$ and sufficiently large $n$, then $T(n) = \Theta(f(n))$. This case applies when the driving function is polynomially larger than $n^{\log_b a}$, meaning that the cost of the work done outside the recursive calls dominates the cost of the recursive calls themselves. The regularity condition $af(n/b) \leq c \cdot f(n)$ prevents the driving function $f(n)$ from being so irregular that the recursive part sometimes dominates and sometimes does not, which would make it impossible to determine the overall asymptotic behavior of the recurrence.

If none of these cases apply, then the master method cannot be used to solve the recurrence, and other methods should be used instead.

$\Large{\color{darkseagreen}\text{Examples}}$

1. **Binary Search**
   The recurrence for the running time of binary search is given by:

    $$  
    \color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = T(n/2) + \Theta(1) \space}
    $$

    Here, $a = 1$, $b = 2$, and $f(n) = \Theta(1)$. We have $n^{\log_b a} = n^{\log_2 1} = n^0 = 1$. So, $f(n)$ is asymptotically equal to $n^{\log_b a}$:

    $$
    f(n) = \Theta(n^{\log_b a} \log^0 n)
    $$

    Hence, we are in case 2 of the master method, which gives us:

    $$
    T(n) = \Theta(\log n)
    $$

2. **Strassen's Algorithm for Matrix Multiplication**
   The recurrence for the running time of Strassen's algorithm is given by:

    $$
    \color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = 7T(n/2) + \Theta(n^2) \space}
    $$

    Here, $a = 7$, $b = 2$, and $f(n) = \Theta(n^2)$. We have $n^{\log_b a} = n^{\log_2 7} \approx n^{2.81}$. So, $f(n)$ is polynomially smaller than $n^{\log_b a}$:

    $$
    f(n) = \mathcal{O}(n^{\log_b a - \epsilon}) = \mathcal{O}(n^{2.81 - \epsilon})
    $$

    where $0 <\epsilon < 0.81$. Hence, we are in case 1 of the master method, which gives us:

    $$
    T(n) = \Theta(n^{\log_2 7}) \approx \Theta(n^{2.81})
    $$

3. **Merge Sort**
   The recurrence for the running time of merge sort is given by:

   $$
   \color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = 2T(n/2) + \Theta(n) \space}
   $$

   Here, $a = 2$, $b = 2$, and $f(n) = \Theta(n)$. We have $n^{\log_b a} = n^{\log_2 2} = n^1 = n$. So, $f(n)$ is asymptotically equal to $n^{\log_b a}$:

    $$
    f(n) = \Theta(n^{\log_b a} \log^0 n)
    $$
  
   Hence, we are in case 2 of the master method, which gives us:

   $$
   T(n) = \Theta(n \log n)
   $$

   Note that this is the same recurrence and solution as for the closest pair of points problem using the divide and conquer approach.

4. **Karatsuba's Algorithm for Integer Multiplication**
   The recurrence for the running time of Karatsuba's algorithm is given by:

   $$
   \color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = 3T(n/2) + \Theta(n) \space}
   $$

   Here, $a = 3$, $b = 2$, and $f(n) = \Theta(n)$. We have $n^{\log_b a} = n^{\log_2 3} \approx n^{1.58}$. So, $f(n)$ is polynomially smaller than $n^{\log_b a}$:

   $$
   f(n) = \mathcal{O}(n^{\log_b a - \epsilon}) = \mathcal{O}(n^{1.58 - \epsilon})
   $$

   where $0 <\epsilon < 0.58$. Hence, we are in case 1 of the master method, which gives us:

   $$
   T(n) = \Theta(n^{\log_2 3}) \approx \Theta(n^{1.58})
   $$

5. **Matrix chain multiplication**
   The recurrence for the running time of the matrix chain multiplication problem using a naive divide and conquer approach is given by:

   $$
   \color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = 2T(n/2) + \Theta(n^3) \space}
   $$

   Here, $a = 2$, $b = 2$, and $f(n) = \Theta(n^3)$. We have $n^{\log_b a} = n^{\log_2 2} = n^1 = n$. So, $f(n)$ is polynomially larger than $n^{\log_b a}$:

   $$
   f(n) = \Omega(n^{\log_b a + \epsilon}) = \Omega(n^{1 + \epsilon})
   $$

   where $0 <\epsilon < 2$. We also need to check the regularity condition:

   $$
   af(n/b) = 2f(n/2) = 2\Theta((n/2)^3) = \Theta(n^3/4) \leq c \cdot f(n)
   $$

   for some constant $c < 1$ and sufficiently large $n$. We can choose $c = 1/2$, which satisfies the condition. Hence, we are in case 3 of the master method, which gives us:

   $$
   T(n) = \Theta(f(n)) = \Theta(n^3)
   $$