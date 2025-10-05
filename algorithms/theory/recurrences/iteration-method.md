$\huge{\color{cadetblue}\text{The Iteration Method}}$

<br/>

The iteration method works by iteratively expanding (or unfolding) the recurrence until a recognizable pattern emerges, expressing the solution as a sum of terms. This sum can then be evaluated using known formulas for the sums of series.  

This method is particularly useful when:

- the recurrence does not immediately fit the form required by the master theorem

- it is simple enough to unroll by hand

The general steps of the iteration method are as follows:

1. **Unfold the recurrence and identify the pattern**: Start with the original recurrence and repeatedly substitute the recurrence into itself to express $T(n)$ in terms of $T$ of smaller arguments. As you do this, look for a pattern in the coefficients and the terms.

2. **Sum the series**: Once a pattern is identified, express the sum in a closed form using known formulas for series.

3. **Solve for the base case**: Finally, substitute the base case of the recurrence to find the final solution.

<br/>

> Depending on how the argument of $T$ decreases, the iteration may generate either a geometric or arithmetic series. In divide-and-conquer recurrences, the argument shrinks multiplicatively ($n/b$), while in decremental recurrences it shrinks additively ($n-1$). The three-step process, however, remains identical in both cases.

<br/>

We now illustrate the iteration method with several examples, covering both divide-and-conquer and decremental types of recurrences. The formulas used to sum the series are provided in the appendix at the end.

<br/>

--------------------------------------------------------------------------

$\Large{\color{cornflowerblue}\text{Example 1}}$

<br/>

Consider the recurrence for merge sort:

$$\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = 2T(n/2) + \Theta(n) \space}$$

<br/>

**Step 1: Unfold and find the pattern**

$$
\begin{align*}
T(n) &= 2T(n/2) + \Theta(n) \\
&= 2\left(2T(n/4) + \Theta(n/2)\right) + \Theta(n) \\
&= 4T(n/4) + 2\Theta(n/2) + \Theta(n) \\
&= 4\left(2T(n/8) + \Theta(n/4)\right) + 2\Theta(n/2) + \Theta(n) \\
&= 8T(n/8) + 4\Theta(n/4) + 2\Theta(n/2) + \Theta(n) \\
&\vdots \\
&= 2^k T(n/2^k) + \sum_{i=0}^{k-1} 2^i \Theta(n/2^i)
\end{align*}
$$

**Step 2: Sum the series**  

The sum forms a geometric series where each term is of order Θ(n). It can be simplified as follows:

$$
\begin{align*}
\sum_{i=0}^{k-1} 2^i \Theta(n/2^i) &= \Theta(n) \sum_{i=0}^{k-1} 1\\
& = \Theta(n) \cdot k
\end{align*}
$$  

**Step 3: Solve for the base case**

We stop expanding when $n/2^k = 1$, which gives $k = \log(n)$. Thus, we have:

$$
T(n) = 2^{\log(n)} T(1) + \Theta(n) \cdot \log(n)
$$

Since $2^{\log(n)} = n$ and assuming $T(1) = \Theta(1)$, we get:

$$
\begin{align*}
T(n) &= n \cdot \Theta(1) + \Theta(n \space \log(n)) \\
&= \Theta(n \space \log(n))
\end{align*}
$$


<br/>

--------------------------------------------------------------------------

$\Large{\color{cornflowerblue}\text{Example 2}}$

<br/>

Consider the recurrence:

$$\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = T(n-1) + \Theta(n) \space}$$

<br/>

**Step 1: Unfold and find the pattern**

$$
\begin{align*}
T(n) &= T(n-1) + \Theta(n) \\
&= \left(T(n-2) + \Theta(n-1)\right) + \Theta(n) \\
&= T(n-2) + \Theta(n-1) + \Theta(n) \\
&= \left(T(n-3) + \Theta(n-2)\right) + \Theta(n-1) + \Theta(n) \\
&= T(n-3) + \Theta(n-2) + \Theta(n-1) + \Theta(n) \\
&\vdots \\
&= T(n-k) + \sum_{i=0}^{k-1} \Theta(n-i)
\end{align*}
$$

**Step 2: Sum the series**

Here the argument decreases additively, so the pattern produces an arithmetic series. The sum can be simplified as follows:

$$
\begin{align*}
\sum_{i=0}^{k-1} \Theta(n-i) &= \Theta\left(\sum_{i=0}^{k-1} (n-i)\right) \\
&= \Theta\left(kn - \frac{k(k-1)}{2}\right) \\
\end{align*}
$$

**Step 3: Solve for the base case**

We stop expanding when $n-k = 1$, which gives $k = n-1$. Thus, we have:

$$
\begin{align*}
T(n) &= T(1) + \Theta\left((n-1)n - \frac{(n-2)(n-1)}{2}\right) \\
&= T(1) + \Theta(n^2)
\end{align*}
$$

Assuming $T(1) = \Theta(1)$, we get:

$$
T(n) = \Theta(1) + \Theta(n^2) = \Theta(n^2)
$$  

<br/>

--------------------------------------------------------------------------

$\Large{\color{cornflowerblue}\text{Example 3}}$

<br/>

Consider the recurrence:

$$\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = T(n - 1) + \Theta(n^2) \space}$$

<br/>

**Step 1: Unfold and find the pattern**

$$
\begin{align*}
T(n) &= T(n-1) + \Theta(n^2) \\
&= \left(T(n-2) + \Theta((n-1)^2)\right) + \Theta(n^2) \\
&= T(n-2) + \Theta((n-1)^2) + \Theta(n^2) \\
&= \left(T(n-3) + \Theta((n-2)^2)\right) + \Theta((n-1)^2) + \Theta(n^2) \\
&= T(n-3) + \Theta((n-2)^2) + \Theta((n-1)^2) + \Theta(n^2) \\
&\vdots \\
&= T(n-k) + \sum_{i=0}^{k-1} \Theta((n-i)^2)
\end{align*}
$$

**Step 2: Sum the series**

Here the argument decreases additively, so the pattern produces an arithmetic series of squares. The sum can be simplified as follows:

$$
\begin{align*}
\sum_{i=0}^{k-1} \Theta((n-i)^2) &= \Theta\left(\sum_{i=0}^{k-1} (n-i)^2\right) \\
&= \Theta\left(kn^2 - 2n\sum_{i=0}^{k-1} i + \sum_{i=0}^{k-1} i^2\right) \\
&= \Theta\left(kn^2 - 2n \cdot \frac{(k-1)k}{2} + \frac{(k-1)k(2k-1)}{6}\right) \\
&= \Theta\left(kn^2 - n(k-1)k + \frac{(k-1)k(2k-1)}{6}\right) \\
\end{align*}
$$

**Step 3: Solve for the base case**

We stop expanding when $n-k = 1$, which gives $k = n-1$. Thus, we have:

$$T(n) = T(1) + \Theta(n^3)$$

Assuming $T(1) = \Theta(1)$, we get:

$$
T(n) = \Theta(1) + \Theta(n^3) = \Theta(n^3)
$$

<br/>

--------------------------------------------------------------------------

$\Large{\color{cornflowerblue}\text{Example 4}}$

<br/>

Consider the recurrence:

$$\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = T(n/3) + \log(n) \space}$$

<br/>

**Step 1: Unfold and find the pattern**

$$
\begin{align*}
T(n) &= T(n/3) + \log(n) \\
&= \left(T(n/9) + \log(n/3)\right) + \log(n) \\
&= T(n/9) + \log(n/3) + \log(n) \\
&= \left(T(n/27) + \log(n /9)\right) + \log(n/3) + \log(n) \\
&= T(n/27) + \log(n/9) + \log(n/3) + \log(n) \\
&\vdots \\
&= T(n/3^k) + \sum_{i=0}^{k-1} \log(n/3^i)
\end{align*}
$$

**Step 2: Sum the series**

$$
\begin{align*}
\sum_{i=0}^{k-1} \log(n/3^i) &= \sum_{i=0}^{k-1} (\log(n) - \log(3^i)) \\
&= \sum_{i=0}^{k-1} \log(n) - \sum_{i=0}^{k-1} i \log(3) \\
&= k \log(n) - \log(3) \cdot \frac{(k-1)k}{2} \\
\end{align*}
$$

**Step 3: Solve for the base case**

We stop expanding when $n/3^k = 1$, which gives $k = \log_3(n)$. Thus, we have:

$$
T(n) = T(1) + \Theta(\log^2(n))
$$

Assuming $T(1) = \Theta(1)$, we get:

$$
T(n) = \Theta(1) + \Theta(\log^2(n)) = \Theta(\log^2(n))
$$  

<br/>

--------------------------------------------------------------------------

$\Large{\color{cornflowerblue}\text{Example 5}}$

<br/>

Consider the recurrence:

$$\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = 2T(n - 1) + 3 \space}$$

<br/>

**Step 1: Unfold and find the pattern**

$$
\begin{align*}
T(n) &= 2T(n-1) + 3 \\
&= 2\left(2T(n-2) + 3\right) + 3 \\
&= 4T(n-2) + 6 + 3 \\
&= 4\left(2T(n-3) + 3\right) + 6 + 3 \\
&= 8T(n-3) + 12 + 6 + 3 \\
&\vdots \\
&= 2^k T(n-k) + 3\sum_{i=0}^{k-1} 2^i
\end{align*}
$$

**Step 2: Sum the series**

$$
\begin{align*}
\sum_{i=0}^{k-1} 2^i &= 2^k - 1
\end{align*}
$$

**Step 3: Solve for the base case**

We stop expanding when $n-k = 1$, which gives $k = n-1$. Assuming $T(1) = \Theta(1)$, we obtain:

$$
\begin{align*}
T(n) &= 2^{n-1} T(1) + 3(2^{n-1} - 1) \\
&= 2^{n-1} \Theta(1) + 3 \cdot 2^{n-1} - 3 \\
&= \Theta(2^n)
\end{align*}
$$

<br/>

--------------------------------------------------------------------------

$\Large{\color{mediumpurple}\text{Appendix: Summation Formulas}}$

<br/>

The following standard summation formulas are frequently used when applying the iteration method to simplify the series that appear after unfolding a recurrence:
<br/>

**1. Sum of the first n natural numbers**:  

$$
\sum_{i=1}^{n} i = \frac{n(n+1)}{2}
$$

<br/>

**2. Sum of the squares of the first n natural numbers**:  

$$
\sum_{i=1}^{n} i^2 = \frac{n(n+1)(2n+1)}{6}
$$

<br/>

**3. Sum of a geometric series**:  
   
$$
\sum_{i=0}^{n-1} r^i = \frac{r^n - 1}{r - 1} \quad \text{for } r \neq 1
$$

<br/>

**4. Sum of logarithms**:  

For $n$ and $k$ as integers with $k \leq n$:

$$
\sum_{i=0}^{k-1} \log(n - i) = \log\left(\frac{n!}{(n-k)!}\right)
$$

<br/>

**5. Sum of an arithmetic series**:  

For an arithmetic series with first term $a$, common difference $d$, and $n$ terms:

$$
\sum_{i=0}^{n-1} (a + id) = \frac{n}{2} \left( 2a + (n-1)d \right)
$$

<br/>

