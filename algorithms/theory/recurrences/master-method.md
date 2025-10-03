$\huge{\color{Cadetblue}\text{The Master Method}}$

<br/>

The master method provides a straightforward way to obtain asymptotic bounds for a broad class of divide-and-conquer recurrences of the following form:

$$
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = aT(n/b) + f(n) \space}
$$  

where $a \geq 1$ and $b > 1$ are constants. The number of subproblems is given by $a$, and the size of each subproblem is given by $n/b$. The function $f(n)$ is called the $\color{darkseagreen} \text{driving function}$. It describes the cost of the work done outside the recursive calls, i.e. the cost of dividing the problem and combining the results of the subproblems. So, in order to apply the master method, the recurrence must be in the specific form above, with a constant number of subproblems of equal size, and a driving function that is asymptotically positive.

The master method consists of three cases, which are determined by comparing the driving function $f(n)$ to the function $n^{\log_b a}$, which represents the total cost of the recursive calls at the leaves of the recursion tree ($\log_b n$ is the height of the recursion tree, and $a^{\log_b {n}}$ $= n^{\log_b {a}}$ is the number of leaves). The three cases are as follows:

1. If $f(n) = \mathcal{O}(n^{\log_b a - \epsilon})$ for some constant $\epsilon > 0$, then $T(n) = \Theta(n^{\log_b a})$. This case applies when the driving function is smaller by at least a polynomial factor than $n^{\log_b a}$, meaning that the cost of the work done outside the recursive calls is dominated by the cost of the recursive calls themselves.</br>  

2. If $f(n) = \Theta(n^{\log_b a} \log^k n)$ for some constant $k \geq 0$, then $T(n) = \Theta(n^{\log_b a} \log^{k+1} n)$. This case applies when the driving function asymptotically matches $n^{\log_b a}$ up to a logarithmic factor, meaning that the cost of the work done outside the recursive calls is comparable to the cost of the recursive calls themselves.</br>  

3. If $f(n) = \Omega(n^{\log_b a + \epsilon})$ for some constant $\epsilon > 0$, and if the regularity condition $af(n/b) \leq c \cdot f(n)$ holds for some constant $c< 1$ and sufficiently large $n$, then $T(n) = \Theta(f(n))$. This case applies when the driving function is polynomially larger than $n^{\log_b a}$, meaning that the cost of the work done outside the recursive calls dominates the cost of the recursive calls themselves. The regularity condition $af(n/b) \leq c \cdot f(n)$ prevents the driving function $f(n)$ from being so irregular that the recursive part sometimes dominates and sometimes does not, which would make it impossible to determine the overall asymptotic behavior of the recurrence. This ensures the work per recursive level shrinks geometrically, so the top-level $f(n)$ dominates. </br>  

It is good to note that it is assumed that the work done at the leaves of the recursion tree is constant, i.e., $T(1) = \Theta(1)$. If the base cost is not constant or there are offset terms, we need to resort to Akra-Bazzi or the recursion tree method.

Also noteworthy is that there is a significant difference in how the master method is presented in editions 3 and 4 of CLRS. This is why the recurrence $T(n) = 2T(n/2) + n \space \log (n)$ is not covered by the master method in edition 3, but it is covered in edition 4. In edition 3, the master method requires $f(n)$ to be polynomially larger or smaller than $n^{\log_b a}$, while in edition 4, it allows for more flexibility with logarithmic factors. The above presentation follows edition 4.

<br/>

$\Large{\color{darkseagreen}\text{Algorithm Examples}}$

<br/>

$\color{cornflowerblue}{\text{1. Binary Search}}$
  
The recurrence for the running time of binary search is given by:

$$  
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = T(n/2) + \Theta(1) \space}
$$

Here, $a = 1$, $b = 2$, and $f(n) = \Theta(1)$. We have $n^{\log_b a} = n^{\log 1} = n^0 = 1$. So, $f(n)$ asymptotically matches $n^{\log_b a}$:

$$
f(n) = \Theta(n^{\log_b a} \log^0 n)
$$

Hence, we are in case 2 of the master method, which gives us:

$$
T(n) = \Theta(\log (n))
$$

<br/>

$\color{cornflowerblue}{\text{2. Standard Matrix Multiplication}}$

If we use the divide-and-conquer approach to multiply two $n \times n$ matrices, the recurrence for the running time is given by:

$$
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = 8T(n/2) + \Theta(1)\space}\\
$$

Here, $a = 8$, $b = 2$, and $f(n) = \Theta(1)$. We have $n^{\log_b a} = n^{\log 8} = n^3$. So, $f(n)$ is polynomially smaller than $n^{\log_b a}$:

$$
f(n) = \mathcal{O}(n^{\log_b a - \epsilon}) = \mathcal{O}(n^{3 - \epsilon})
$$

where $0 <\epsilon < 3$. Hence, we are in case 1 of the master method, which gives us:  

$$
T(n) = \Theta(n^3)
$$


$\color{cornflowerblue}{\text{3. Strassen's Matrix Multiplication}}$

The recurrence for the running time of Strassen's algorithm is given by:

$$
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = 7T(n/2) + \Theta(n^2) \space}
$$

Here, $a = 7$, $b = 2$, and $f(n) = \Theta(n^2)$. We have $n^{\log_b a} = n^{\log 7} \approx n^{2.81}$. So, $f(n)$ is polynomially smaller than $n^{\log_b a}$:

$$
f(n) = \mathcal{O}(n^{\log_b a - \epsilon}) = \mathcal{O}(n^{2.81 - \epsilon})
$$

where $0 <\epsilon < 0.81$. Hence, we are in case 1 of the master method, which gives us:

$$
T(n) = \Theta(n^{\log 7}) \approx \Theta(n^{2.81})
$$

<br/>

$\color{cornflowerblue}{\text{4. Merge Sort}}$

The recurrence for the running time of merge sort is given by:

$$
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = 2T(n/2) + \Theta(n) \space}
$$

Here, $a = 2$, $b = 2$, and $f(n) = \Theta(n)$. We have $n^{\log_b a} = n^{\log 2} = n^1 = n$. So, $f(n)$ asymptotically matches $n^{\log_b a}$:

$$
f(n) = \Theta(n^{\log_b a} \log^0 (n))
$$

Hence, we are in case 2 of the master method, which gives us:

$$
T(n) = \Theta(n \space \log (n))
$$

Note that this is the same recurrence and solution as for the closest pair of points problem and the linearithmic maximum subarray problem in the divide-and-conquer versions of those algorithms.

<br/>

$\color{cornflowerblue}{\text{5. Standard Integer Multiplication}}$

The recurrence for the running time of the divide-and-conquer version of the standard long multiplication algorithm for multiplying two $n$-digit integers is given by:

$$
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = 4T(n/2) + \Theta(n) \space}
$$  

Here, $a = 4$, $b = 2$, and $f(n) = \Theta(n)$. We have $n^{\log_b a} = n^{\log 4} = n^2$. So, $f(n)$ is polynomially smaller than $n^{\log_b a}$:

$$
f(n) = \mathcal{O}(n^{\log_b a - \epsilon}) = \mathcal{O}(n^{2 - \epsilon})
$$  

where $0 <\epsilon < 2$. Hence, we are in case 1 of the master method, which gives us:

$$
T(n) = \Theta(n^2)
$$

<br/>

$\color{cornflowerblue}{\text{6. Karatsuba's Algorithm}}$

The recurrence for the running time of Karatsuba's algorithm is given by:

$$
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = 3T(n/2) + \Theta(n) \space}$$

Here, $a = 3$, $b = 2$, and $f(n) = \Theta(n)$. We have $n^{\log_b a} = n^{\log 3} \approx n^{1.58}$. So, $f(n)$ is polynomially smaller than $n^{\log_b a}$:

$$
f(n) = \mathcal{O}(n^{\log_b a - \epsilon}) = \mathcal{O}(n^{1.58 - \epsilon})
$$

where $0 <\epsilon < 0.58$. Hence, we are in case 1 of the master method, which gives us:

$$
T(n) = \Theta(n^{\log 3}) \approx \Theta(n^{1.58})
$$

<br/>

$\Large{\color{darkseagreen}\text{Additional Exercises}}$

<br/>

$\color{cornflowerblue}{\text{1. Exercise 1}}$

The recurrence for a certain divide-and-conquer algorithm is given by:

$$
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = 2T(n/2) + \Theta(n^3) \space}
$$

Here, $a = 2$, $b = 2$, and $f(n) = \Theta(n^3)$. We have $n^{\log_b a} = n^{\log 2} = n^1 = n$. So, $f(n)$ is polynomially larger than $n^{\log_b a}$:

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

<br/>

$\color{cornflowerblue}{\text{2. Exercise 2}}$

The recurrence for a certain divide-and-conquer algorithm is given by:

$$
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = T(n/3) + \Theta(\log(n)) \space}
$$

Here, $a = 1$, $b = 3$, and $f(n) = \Theta(\log (n))$. We have $n^{\log_b a} = n^{\log_3 1} = n^0 = 1$. So, $f(n)$ asymptotically matches $n^{\log_b a}$ up to a logarithmic factor:

$$
f(n) = \Theta(n^{\log_b a} \log^1 n) = \Theta(\log (n))
$$

where $k = 1$. Hence, we are in case 2 of the master method, which gives us:

$$
T(n) = \Theta(\log^2 (n))
$$

<br/>

$\color{cornflowerblue}{\text{3. Exercise 3}}$

The recurrence for a certain divide-and-conquer algorithm is given by:

$$
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = 3T(n/4) + \Theta(n \space \log (n)) \space}
$$

Here, $a = 3$, $b = 4$, and $f(n) = \Theta(n \log (n))$. We have $n^{\log_b a} = n^{\log_4 3} \approx n^{0.79}$. So, $f(n)$ is polynomially larger than $n^{\log_b a}$:

$$
f(n) = \Omega(n^{\log_b a + \epsilon}) = \Omega(n^{0.79 + \epsilon})
$$

where $0 <\epsilon < 0.21$. We also need to check the regularity condition:

$$
af(n/b) = 3f(n/4) = 3\Theta((n/4) \log(n/4)) \leq c \cdot f(n)
$$  

for some constant $c < 1$ and sufficiently large $n$. We can choose $c = 3/4$, which satisfies the condition. Hence, we are in case 3 of the master method, which gives us:

$$
T(n) = \Theta(f(n)) = \Theta(n \space \log (n))
$$

<br/>

$\color{cornflowerblue}{\text{4. Exercise 4}}$

The recurrence for a certain divide-and-conquer algorithm is given by:

$$
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = 2T(n/4) + \Theta(\sqrt{n}) \space}
$$

Here, $a = 2$, $b = 4$, and $f(n) = \Theta(\sqrt{n})$. We have $n^{\log_b a} = n^{\log_4 2} = n^{1/2} = \sqrt{n}$. So, $f(n)$ asymptotically matches $n^{\log_b a}$:

$$
f(n) = \Theta(n^{\log_b a} \log^0 n) = \Theta(\sqrt{n})
$$

where $k = 0$. Hence, we are in case 2 of the master method, which gives us:

$$
T(n) = \Theta(\sqrt{n} \log (n))
$$

<br/>

$\color{cornflowerblue}{\text{5. Exercise 5}}$

The recurrence for a certain divide-and-conquer algorithm is given by:

$$
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = 2T(n/4) + n \space}
$$

Here, $a = 2$, $b = 4$, and $f(n) = \Theta(n)$. We have $n^{\log_b a} = n^{\log_4 2} = n^{1/2} = \sqrt{n}$. So, $f(n)$ is polynomially larger than $n^{\log_b a}$:

$$
f(n) = \Omega(n^{\log_b a + \epsilon}) = \Omega(n^{1/2 + \epsilon})
$$

where $0 <\epsilon < 1/2$. We also need to check the regularity condition:

$$
af(n/b) = 2f(n/4) = 2\Theta(n/4) = \Theta(n/2) \leq c \cdot f(n)
$$

for some constant $c < 1$ and sufficiently large $n$. We can choose $c = 3/4$, which satisfies the condition. Hence, we are in case 3 of the master method, which gives us:

$$
T(n) = \Theta(f(n)) = \Theta(n)
$$

<br/>

$\color{cornflowerblue}{\text{6. Exercise 6}}$

The recurrence for a certain divide-and-conquer algorithm is given by:

$$
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = 9T(n/3) + n^2 \log^3(n) \space}
$$

Here, $a = 9$, $b = 3$, and $f(n) = \Theta(n^2 \log^3 n)$. We have $n^{\log_b a} = n^{\log_3 9} = n^2$. So, $f(n)$ asymptotically matches $n^{\log_b a}$ up to a logarithmic factor:

$$
f(n) = \Theta(n^{\log_b a} \log^3 (n))
$$

where $k = 3$. Hence, we are in case 2 of the master method, which gives us:

$$
T(n) = \Theta(n^2 \log^4 (n))
$$

<br/>

$\color{cornflowerblue}{\text{7. Exercise 7}}$

The recurrence for a certain divide-and-conquer algorithm is given by:

$$
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = 2T(n/2) + n\space\log(n) \space}
$$  

Here, $a = 2$, $b = 2$, and $f(n) = n\log (n)$. We have $n^{\log_b a} = n^{\log 2} = n^1 = n$. Since $f(n)$ matches $n^{\log_b a}$ up to a logarithmic factor, we are in case 2 of the master theorem: 

$$
f(n) = \Theta(n^{\log_b a} \space \log^k (n))
$$

where $k = 1$. Hence, we have:

$$
T(n) = \Theta(n \space \log^{k+1} (n)) = \Theta(n\space \log^2 (n))
$$  

