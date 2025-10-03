$\huge{\color{Cadetblue}\text{Akra-Bazzi Method}}$

<br/>

The Akra-Bazzi method is a powerful technique for solving a class of divide-and-conquer recurrences that are more general than those typically handled by the Master Theorem. It is particularly useful for recurrences that take the following form for $n > n_0$:

$$
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = f(n) + \sum_{i=1}^{k} a_i T(\frac{n}{b_i} + h_i(n)) \space}
$$

where:

- $a_i$ and $b_i$ are constants with $a_i > 0$ and $b_i > 1$,
- $h_i(n)$ are functions that satisfy $|h_i(n)| = O(n / \log^2 n)$,
- $f(n)$ must be polynomially bounded: there exists a constant $c$ such that $f(n) = O(n^c)$ and its derivative $f'(n)$ is also polynomially bounded,
- $k$ is a positive constant.

The Akra-Bazzi method provides a way to find the asymptotic behavior of $T(n)$ by solving the following integral equation:

$$
T(n) = \Theta\left( n^p \left( 1 + \int_{1}^{n} \frac{f(u)}{u^{p+1}} \space du \right) \right)
$$

where $p$ is the unique solution to the equation:

$$\sum_{i=1}^{k} \frac{a_i}{b_i^p} = 1$$  

<br/>

The Akra–Bazzi method generalizes the Master Theorem by allowing:

- Multiple subproblems with potentially different scaling factors $b_i$,

- Perturbed subproblem sizes via $h_i(n)$, as long as they’re small compared to $n$ (e.g. $O(n/\log^2 n)$),

- More general driving functions $f(n)$, provided they are polynomially bounded.

In this way, Akra–Bazzi subsumes all the cases handled by the Master Theorem, while also solving a much wider class of recurrences that may arise in advanced algorithms.

<br/>

$\Large{\color{darkseagreen}\text{Examples}}$

<br/>

$\color{cornflowerblue}{\text{1. Example 1}}$

The Akra-Bazzi method can also be applied to recurrences that would typically be solved using the Master Theorem, such as the recurrence for Mergesort:

$$
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = 2T(n/2) + n \space}
$$

Here, we have $a_1 = 2$, $b_1 = 2$, and $f(n) = n$. We need to find $p$ such that:

$$
\begin{align*}
\frac{2}{2^p} = 1 &\implies 2^{1-p} = 1 \\ 
& \implies p = 1
\end{align*}
$$

Now, we compute the integral:

$$
\begin{align*}
\int_{1}^{n} \frac{f(u)}{u^{p+1}}\space du &= \int_{1}^{n} \frac{u}{u^{1+1}}\space  du \\
&= \int_{1}^{n} \frac{1}{u} \space du \\
&= \ln(n)
\end{align*}
$$

Thus, we have:  

$$
\begin{align*}
T(n) &= \Theta\left( n^1 \left( 1 + \ln(n) \right) \right) \\
&= \Theta(n \space \ln(n) + n) \\
&= \Theta(n \space \ln(n)) \\
&= \Theta(n \space \frac {\log(n)}{\ln(2)}) \\
&= \Theta(n \space \log(n))
\end{align*}
$$

<br/>

$\color{cornflowerblue}{\text{2. Example 2}}$

The recurrence relation for the Quickselect algorithm, using the median-of-medians strategy for pivot selection, is given by:

$$
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = T(n/5) + T(7n/10) + \Theta(n) \space}
$$

<br/>

Here, we have $a_1 = 1$, $b_1 = 5$, $a_2 = 1$, $b_2 = \frac{10}{7}$, and $f(n) = \Theta(n)$. We need to find $p$ such that:

$$
\begin{align*}
&\frac{1}{5^p} + \frac{1}{(10/7)^p} = 1\\
\implies &\frac{1}{5^p} + \left(\frac{7}{10}\right)^p = 1
\end{align*}
$$

From the equation it is clear that $0 < p < 1$ must be the case. Using Newton's method to approximate the root, we find that $p \approx 0.8397803$, but as we will see, the exact value of $p$ is not necessary to determine the asymptotic behavior of $T(n)$.

Now, we compute the integral:  

$$
\begin{align*}
\int_{1}^{n} \frac{f(u)}{u^{p+1}}\space du &= \int_{1}^{n} \frac{u}{u^{p+1}} \space du \\
&= \int_{1}^{n} u^{-p} \space du \\
&= \left[ \frac{u^{1-p}}{1-p} \right]_{1}^{n} \\
&= \frac{n^{1-p}}{1-p} - \frac{1^{1-p}}{1-p} \\
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

Note that we did not need the exact value of $p$ to determine that $T(n) = \Theta(n)$. Generally, this will be the case when the value of $p$ is smaller than the exponent of the polynomially bounded function $f(n)$. Here, we knew that $p < 1$ and $f(n) = \Theta(n)$, which has an exponent of $1$. In such cases, we can in fact apply the below theorem to directly conclude that $T(n) = \Theta(f(n))$.

Hence, this confirms that the Quickselect algorithm implemented with the median-of-medians strategy runs in linear time.
<br/>

$\Large{\color{mediumpurple}\text{Additional Theorem}}$

If $f(n) = \Theta(n^c)$ for some constant $c > 0$ and if $p < c$, then:

$$
T(n) = \Theta(f(n))
$$

<br/>

$\color{cornflowerblue}{\text{3. Example 3}}$

Given is a function that satisfies the recurrence relation:  

$$
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = 2T(n/2) + n/\log (n) \space}
$$

Note that this recurrence cannot be solved using the Master Theorem, since the driving function $f(n) = n/\log{n}$ cannot be expressed in the form $n^{\log_b a - \epsilon}$, $n^{\log_b a} \log^k n$, or $n^{\log_b a + \epsilon}$ for any $\epsilon > 0$ and $k \geq 0$.

Here, we have $a_1 = 2$, $b_1 = 2$, and $f(n) = n/\log (n)$. We need to find $p$ such that:

$$
\begin{align*}
\frac{2}{2^p} = 1 &\implies 2^{1-p} = 1 \\
& \implies p = 1
\end{align*}
$$

Now, we compute the integral:  

$$
\begin{align*}
\int_{1}^{n} \frac{f(u)}{u^{p+1}}\space du &= \int_{1}^{n} \frac{u/\log (u)}{u^{1+1}} \space du \\
&= \int_{1}^{n} \frac{1}{u\space \log (u)} \space du
\end{align*}
$$

Using substitution, let $v = \log(u)$, hence $dv = \frac{1}{u} du$. The bounds change accordingly: when $u = n_0 > 1$ $\rightarrow v = \log(n_0)$, and when $u = n$ $\rightarrow v = \log(n)$. We avoid using $u = 1$ as the lower bound since $\log(1) = 0$ would lead to a division by zero in the integral. Choosing any fixed constant $n_0 > 1$, for example $n_0 = 2$, will only shift the result by a constant, which does not affect the asymptotic $\Theta$ bound.

Thus, the integral becomes:

$$
\begin{align*}
\int_{2}^{n} \frac{1}{u\space \log (u)} \space du &= \int_{2}^{\log(n)} \frac{1}{v} \space dv \\
&= \left[ \ln(v) \right]_{\log(2)}^{\log(n)} \\
&= \ln(\log(n)) - \ln(\log(2)) \\
&= \ln(\log(n)) - \ln(1) \\
&= \ln(\log(n))
\end{align*}
$$

Thus, we have:  

$$
\begin{align*}
T(n) &= \Theta\left( n^1 \left( 1 + \ln(\log (n)) \right) \right) \\
&= \Theta(n \space \ln (\log (n)) + n) \\
&= \Theta(n \space \ln (\log (n))) \\
&= \Theta\left(n \space \frac{\log(\log (n))}{\ln(2)}\right) \\
&= \Theta(n \space \log(\log (n)))
\end{align*}
$$

<br/>

$\color{cornflowerblue}{\text{4. Example 4}}$

Given is a function that satisfies the recurrence relation:

$$
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = 3T(n/3) + 4T(n/4) + n^2 \space}
$$

Here, we have $a_1 = 3$, $b_1 = 3$, $a_2 = 4$, $b_2 = 4$, and $f(n) = n^2$. We need to find $p$ such that:

$$
\begin{align*}
&\frac{3}{3^p} + \frac{4}{4^p} = 1\\
\implies &\frac{1}{3^{p-1}} + \frac{1}{4^{p-1}} = 1
\end{align*}
$$

Plugging in $p = 1$ gives $2 > 1$ and plugging in $p = 2$ gives $\frac{3}{9} + \frac{4}{16} < 1$. So we have $1 < p < 2$. Seeing that $f(n) = n^2$ has an exponent of $2 > p$, we can conclude that $T(n) = \Theta(f(n)) = \Theta(n^2)$ using the additional theorem mentioned above.

<br/>

$\color{cornflowerblue}{\text{5. Example 5}}$

Given is a function that satisfies the recurrence relation:

$$
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = T(n/2) + T(n/3) + T(n/6) + n \log(n) \space}
$$

Here, we have $a_1 = a_2 = a_3 = 1$, $b_1 = 2$, $b_2 = 3$, $b_3 = 6$, and $f(n) = n \log(n)$. We need to find $p$ such that:

$$
\begin{align*}
&\frac{1}{2^p} + \frac{1}{3^p} + \frac{1}{6^p} = 1\\
\implies &\frac{1}{2^p} + \frac{1}{3^p} + \left(\frac{1}{2} \cdot \frac{1}{3}\right)^p = 1
\end{align*}
$$

Plugging in $p = 1$ gives $\frac{5}{6} + \frac{1}{6} = 1$. So we have $p = 1$.

Now, we compute the integral:

$$
\begin{align*}
\int_{1}^{n} \frac{f(u)}{u^{p+1}}\space du &= \int_{1}^{n} \frac{u \log(u)}{u^{1+1}} \space du \\
&= \int_{1}^{n} \frac{\log(u)}{u} \space du \\
\end{align*}
$$

Using substitution, let $v = \log(u)$, hence $dv = \frac{1}{u} du$. The bounds change accordingly: when $u = 1$ $\rightarrow v = \log(1) = 0$, and when $u = n$ $\rightarrow v = \log(n)$. Thus, the integral becomes:

$$
\begin{align*}
\int_{1}^{n} \frac{\log(u)}{u} \space du &= \int_{0}^{\log(n)} v \space dv \\
&= \left[ \frac{v^2}{2} \right]_{0}^{\log(n)} \\
&= \frac{\log^2(n)}{2} - \frac{0^2}{2} \\
&= \frac{\log^2(n)}{2}
\end{align*}
$$

Thus, we have:

$$
\begin{align*}
T(n) &= \Theta\left( n^1 \left( 1 + \frac{\log^2(n)}{2} \right) \right) \\
&= \Theta\left( n + \frac{n \log^2(n)}{2} \right) \\
&= \Theta(n \log^2(n))
\end{align*}
$$

<br/>

$\color{cornflowerblue}{\text{6. Example 6}}$

Given is a function that satisfies the recurrence relation:

$$
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = 3T(n/3) + 8T(n/4) + n^2 \log(n) \space}
$$

Here, we have $a_1 = 3$, $b_1 = 3$, $a_2 = 8$, $b_2 = 4$, and $f(n) = n^2 \log(n)$. We need to find $p$ such that:

$$
\begin{align*}
&\frac{3}{3^p} + \frac{8}{4^p} = 1\\
\implies &\frac{1}{3^{p-1}} + \frac{1}{2^{2p-3}} = 1
\end{align*}
$$

Plugging in $p = 1$ gives $3 > 1$ and plugging in $p = 2$ yields $\frac{5}{6} < 1$. So we have $1 < p < 2$. Seeing that $f(n) = n^2 \log(n)$ has an exponent of $2 > p$, we can conclude that $T(n) = \Theta(f(n)) = \Theta(n^2 \log(n))$ using the additional theorem mentioned above. In this case, this actually saves us considerable work, since the integral would be a bit more involved to compute.

<br/>

$\color{cornflowerblue}{\text{7. Example 7}}$

Given is a function that satisfies the recurrence relation:

$$
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = \frac{2}{3}T(n/3) + \frac{1}{3}T(2n/3) + \log (n) \space}
$$

Here, we have $a_1 = \frac{2}{3}$, $b_1 = 3$, $a_2 = \frac{1}{3}$, $b_2 = \frac{3}{2}$, and $f(n) = \log (n)$. We need to find $p$ such that:

$$
\begin{align*}
&\frac{2/3}{3^p} + \frac{1/3}{(3/2)^p} = 1\\
\implies &\frac{2}{3^{p+1}} + \frac{2^p}{3^{p+1}} = 1 \\
\implies &\frac{2 + 2^p}{3^{p+1}} = 1 \\
\implies &2 + 2^p = 3^{p+1}
\end{align*}
$$

This equation is satisfied for $p = 0$, since $2 + 2^0 = 3^{0+1}$.

Now, we compute the integral:

$$
\begin{align*}
\int_{1}^{n} \frac{f(u)}{u^{p+1}}\space du &= \int_{1}^{n} \frac{\log(u)}{u^{0+1}} \space du \\
&= \int_{1}^{n} \frac{\log(u)}{u} \space du \\
\end{align*}  
$$

This is the same integral as in Example 5, which we already computed to be $\frac{\log^2(n)}{2}$.

Thus, we have:

$$
\begin{align*}
T(n) &= \Theta\left( n^0 \left( 1 + \frac{\log^2(n)}{2} \right) \right) \\
&= \Theta\left( 1 + \frac{\log^2(n)}{2} \right) \\
&= \Theta(\log^2(n))
\end{align*}
$$

<br/>

$\color{cornflowerblue}{\text{8. Example 8}}$

Given is a function that satisfies the recurrence relation:

$$
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = \frac{1}{3}T(n/3) + 1/n \space}
$$

Note that this recurrence cannot be solved using the Master Theorem, since the driving function $f(n) = 1/n$ cannot be expressed in the form $n^{\log_b a - \epsilon}$, $n^{\log_b a} \log^k n$, or $n^{\log_b a + \epsilon}$ for any $\epsilon > 0$ and $k \geq 0$.

Here, we have $a_1 = \frac{1}{3}$, $b_1 = 3$, and $f(n) = 1/n$. We need to find $p$ such that:

$$
\begin{align*}
\frac{1/3}{3^p} = 1 &\implies \frac{1}{3^{p+1}} = 1 \\
& \implies p = -1
\end{align*}
$$

Now, we compute the integral:

$$
\begin{align*}
\int_{1}^{n} \frac{f(u)}{u^{p+1}}\space du &= \int_{1}^{n} \frac{1/u}{u^{-1+1}} \space du \\
&= \int_{1}^{n} \frac{1}{u} \space du \\
&= \ln(n)
\end{align*}
$$

Thus, we have:

$$
\begin{align*}
T(n) &= \Theta\left( n^{-1} \left( 1 + \ln(n) \right) \right) \\
&= \Theta\left( \frac{1}{n} + \frac {\ln(n)}{n} \right) \\
&= \Theta\left( \frac {\ln(n)}{n} \right)\\
&= \Theta\left( \frac {\log(n)}{n \space \ln(2)} \right) \\
&= \Theta\left( \frac {\log(n)}{n} \right)
\end{align*}
$$

<br/>

$\color{cornflowerblue}{\text{9. Example 9}}$

Given is a function that satisfies the recurrence relation:

$$
\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = T(n/2) + T(n/4) + T(n/8) + n \space}
$$  

Here, we have $a_1 = a_2 = a_3 = 1$, $b_1 = 2$, $b_2 = 4$, $b_3 = 8$, and $f(n) = n$. We need to find $p$ such that:

$$
\begin{align*}
&\frac{1}{2^p} + \frac{1}{4^p} + \frac{1}{8^p} = 1\\
\implies &\frac{1}{2^p} + \frac{1}{(2^2)^p} + \frac{1}{(2^3)^p} = 1 \\
\implies &\frac{1}{2^p} + \frac{1}{2^{2p}} + \frac{1}{2^{3p}} = 1
\end{align*}
$$

Plugging in $p = 0$ gives $3 > 1$ and plugging in $p = 1$ gives $\frac{7}{8} < 1$. So we have $0 < p < 1$. Since $f(n) = n$ has an exponent of $1 > p$, we can conclude that $T(n) = \Theta(f(n)) = \Theta(n)$ using the additional theorem mentioned above.









































