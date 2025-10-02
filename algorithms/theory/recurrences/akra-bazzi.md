$\huge{\color{Cadetblue}\text{Akra-Bazzi Method}}$

<br/>

The Akra-Bazzi method is a powerful technique for solving a class of divide-and-conquer recurrences that are more general than those typically handled by the Master Theorem. It is particularly useful for recurrences that take the following form for \( n > n_0 \):

$$ 
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = f(n) + \sum_{i=1}^{k} a_i T(\frac{n}{b_i} + h_i(n)) \space}
$$

where:

- \( a_i \) and \( b_i \) are constants with \( a_i > 0 \) and \( b_i > 1 \),
- \( h_i(n) \) are functions that satisfy \( |h_i(n)| = O(n / \log^2 n) \),
- \( f(n) \) must be polynomially bounded: \(| f'(n) | \leq n^c \) for some constant \( c \),
- \( k \) is a positive constant.

The Akra-Bazzi method provides a way to find the asymptotic behavior of \( T(n) \) by solving the following integral equation:

$$ T(n) = \Theta\left( n^p \left( 1 + \int_{1}^{n} \frac{f(u)}{u^{p+1}} du \right) \right) $$

where \( p \) is the unique solution to the equation:

$$ \sum_{i=1}^{k} \frac{a_i}{b_i^p} = 1 $$  

<br/>

$\Large{\color{darkseagreen}\text{Examples}}$

<br/>

$\color{cornflowerblue}{\text{1. Example 1}}$

The Akra-Bazzi method can also be applied to recurrences that would typically be solved using the Master Theorem, such as the recurrence for Mergesort:

$$\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = 2T(n/2) + n \space}$$

Here, we have \( a_1 = 2 \), \( b_1 = 2 \), and \( f(n) = n \). We need to find \( p \) such that:

$$ 
\begin{align*}
\frac{2}{2^p} = 1 &\implies 2^{1-p} = 1 \\ 
& \implies p = 1
\end{align*}
$$

Now, we compute the integral:

$$ 
\begin{align*}
\int_{1}^{n} \frac{f(u)}{u^{p+1}} du &= \int_{1}^{n} \frac{u}{u^{1+1}} du \\
&= \int_{1}^{n} \frac{1}{u} du \\
&= \ln(n)
\end{align*}
$$

Thus, we have:
$$
\begin{align*}
T(n) &= \Theta\left( n^1 \left( 1 + \ln(n) \right) \right) \\
&= \Theta(n \ln(n) + n) \\
&= \Theta(n \ln(n)) \\
&= \Theta(n \frac {\log(n)}{\ln(2)}) \\
&= \Theta(n \log(n)) 
\end{align*}
$$

<br/>

$\color{cornflowerblue}{\text{2. Example 2}}$

The recurrence relation for the Quickselect algorithm, using the median-of-medians strategy for pivot selection, is given by:

$$
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = T(n/5) + T(7n/10) + \Theta(n) \space}
$$

<br/>

Here, we have \( a_1 = 1 \), \( b_1 = 5 \), \( a_2 = 1 \), \( b_2 = \frac{10}{7} \), and \( f(n) = \Theta(n) \). We need to find \( p \) such that:

$$ 
\begin{align*}
&\frac{1}{5^p} + \frac{1}{(10/7)^p} = 1\\
\implies &\frac{1}{5^p} + \left(\frac{7}{10}\right)^p = 1
\end{align*}
$$

From the equation it is clear that \( 0 < p < 1 \) must be the case. Using Newton's method to approximate the root, we find that \( p \approx 0.8397803 \).

Now, we compute the integral:

$$ 
\begin{align*}
\int_{1}^{n} \frac{f(u)}{u^{p+1}} du &= \int_{1}^{n} \frac{u}{u^{p+1}} du \\
&= \int_{1}^{n} u^{-p} du \\
&= \left[ \frac{u^{1-p}}{1-p} \right]_{1}^{n} \\
&= \frac{n^{1-p} - 1}{1-p}
\end{align*}
$$

Thus, we have:
$$
\begin{align*}
T(n) & = \Theta\left( n^p \left( 1 + \frac{n^{1-p} - 1}{1-p} \right) \right) \\
&= \Theta\left( n^p \cdot \frac{n^{1-p}}{1-p} \right) \\
&= \Theta\left( \frac{n}{1-p} \right) = \Theta(n)
\end{align*}
$$

Thus, this confirms that the Quickselect algorithm implemented with the median-of-medians strategy runs in linear time.

<br/>

$\color{cornflowerblue}{\text{3. Example 3}}$

Given is a function that satisfies the recurrence relation:

$$
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = 2T(n/2) + n/\log n \space}
$$

Note that this recurrence cannot be solved using the Master Theorem, since the driving function \( f(n) = n/\log n \) cannot be expressed in the form \( n^{\log_b a - \epsilon} \), \( n^{\log_b a} \log^k n \), or \( n^{\log_b a + \epsilon} \) for any \( \epsilon > 0 \) and \( k \geq 0 \).

Here, we have \( a_1 = 2 \), \( b_1 = 2 \), and \( f(n) = n/\log n \). We need to find \( p \) such that:

$$ 
\begin{align*}
\frac{2}{2^p} = 1 &\implies 2^{1-p} = 1 \\ 
& \implies p = 1
\end{align*}
$$

Now, we compute the integral:

$$ 
\begin{align*}
\int_{1}^{n} \frac{f(u)}{u^{p+1}} du &= \int_{1}^{n} \frac{u/\log u}{u^{1+1}} du \\
&= \int_{1}^{n} \frac{1}{u \log u} du \\
&= \ln(\log n)
\end{align*}
$$

Thus, we have:
$$
\begin{align*}
T(n) &= \Theta\left( n^1 \left( 1 + \ln(\log n) \right) \right) \\
&= \Theta(n \ln (\log n) + n) \\
&= \Theta(n \ln (\log n)) \\
&= \Theta\left(n \frac{\log(\log n)}{\ln(2)}\right) \\
&= \Theta(n \log(\log n))
\end{align*}
$$

