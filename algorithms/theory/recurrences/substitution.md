$\huge{\color{Cadetblue}\text{The Substitution Method}}$

<br/>

The substitution method is the most general technique for solving recurrences. It consists of two steps:

- Guess the form of the solution using symbolic constants, often inspired by recursion trees or other intuition.
- Prove the bound correct using mathematical induction.

Unlike the recursion tree or Master Theorem, this method requires a good guess, which may not always be obvious. Substitution can be used to prove both upper and lower bounds.

To prove an upper bound, we show that the recurrence solution is less than or equal to our guess for sufficiently large $n$.

To prove a lower bound, we show the opposite inequality.

<br/>

$\Large{\color{darkseagreen}\text{Examples}}$

<br/>

$\Large{\color{cornflowerblue}\text{1. Example 1}}$

Consider the recurrence:

$$\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = 2T(n/2) + n \space}$$

We know from recursion trees that the solution should be $T(n) = \mathcal{O}(n \space \log (n))$. To prove this, we try to show that $T(n) \leq c n \space \log (n)$ for some constant $c > 0$ and sufficiently large $n$, as the definition of $\mathcal{O}$ notation requires.

$\color{mediumorchid}{\text{1.1 Base Case}}$

Pick a small value, say $n = 2$.

Then $T(2) = 2T(1) + 2$. Since $T(1)$ is finite, we can always choose $c$ large enough so that $T(2) \leq c \cdot 2 \space \log 2 = 2c$.

$\color{mediumorchid}{\text{1.2 Inductive Step}}$

Induction hypothesis (IH):

Assume that the hypothesis holds for all values smaller than $n$, i.e., $T(k) \leq c k \space \log (k)$ for all $k < n$.

Then:

$$
\begin{align*}
T(n) & = 2T(n/2) + n \\
& \leq 2 \left( c \frac{n}{2} \space \log \left( \frac{n}{2} \right) \right) + n \quad \text{(IH)} \\
& = c n \space \log \left( \frac{n}{2} \right) + n \\
& = c n \left( \log (n) - \log (2) \right) + n \\
& = c n \space \log (n) - c n + n \\
& = c n \space \log (n) + n (1 - c)
\end{align*}
$$

To complete the inductive step, we need to show that $T(n) \leq c n \space \log (n)$.
This will be true if $n (1 - c) \leq 0$, which holds for $c \geq 1$. Hence, by induction, we have shown that $T(n) = \mathcal{O}(n \space \log (n))$.

A matching lower bound can be shown similarly, thus proving that $T(n) = \Theta(n \space \log (n))$.

<br/>

$\Large{\color{cornflowerblue}\text{2. Example 2}}$
 
Consider the recurrence:

$$\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = T(n/2) + T(n/4) + n^2 \space}$$

We will guess that $T(n) = \mathcal{O}(n^2)$ and prove it by induction.

$\color{mediumorchid}{\text{2.1 Base Case}}$

Pick a small value, say $n = 4$.

Then $T(4) = T(2) + T(1) + 16$. Since $T(2)$ and $T(1)$ are finite, there exists a constant $c$ large enough such that $T(4) \leq 16 + T(2) + T(1) \leq c \cdot 4^2 = 16c$.

$\color{mediumorchid}{\text{2.2 Inductive Step}}$

Induction hypothesis (IH):
Assume that the hypothesis holds for all values smaller than $n$, i.e., $T(k) \leq c k^2$ for all $k < n$.

Then:

$$
\begin{align*}
T(n) & = T(n/2) + T(n/4) + n^2 \\
& \leq c \left( \frac{n}{2} \right)^2 + c \left( \frac{n}{4} \right)^2 + n^2 \quad \text{(IH)} \\
& = c \frac{n^2}{4} + c \frac{n^2}{16} + n^2 \\
& = \frac{4c n^2 + c n^2 + 16 n^2}{16} \\
& = \frac{(5c + 16) n^2}{16}
\end{align*}
$$

To complete the inductive step, we need to show that $T(n) \leq c n^2$.
This will be true if $\frac{(5c + 16) n^2}{16} \leq c n^2$ or equivalently if $5c + 16 \leq 16c$, which holds for $c \geq \frac{16}{11}$. Hence, by induction, we have shown that $T(n) = \mathcal{O}(n^2)$. 

A matching lower bound can be shown similarly, thus proving that $T(n) = \Theta(n^2)$.

<br/>

$\Large{\color{cornflowerblue}\text{3. Example 3}}$

Consider the recurrence:

$$\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = T(n - 1) + n \space}$$

We will guess that $T(n) = O(n^2)$ and prove it by induction.

$\color{mediumorchid}{\text{3.1 Base Case}}$

Pick a small value, say $n = 1$.

Then $T(1)$ is finite, so we can always choose $c$ large enough so that $T(1) \leq c \cdot 1^2 = c$, and the inequality holds for $n = 1$.

$\color{mediumorchid}{\text{3.2 Inductive Step}}$

Induction hypothesis (IH):
Assume that the hypothesis holds for all values smaller than $n$, i.e., $T(k) \leq c k^2$ for all $k < n$.

Then:

$$
\begin{align*}
T(n) & = T(n - 1) + n \\
& \leq c (n - 1)^2 + n \quad \text{(IH)} \\
& = c (n^2 - 2n + 1) + n \\
& = c n^2 - 2c n + c + n \\
& = c n^2 + n (1 - 2c) + c
\end{align*}
$$

To complete the inductive step, we need to show that $T(n) \leq c n^2$.
This will be true if $n (1 - 2c) + c \leq 0$. This inequality holds for sufficiently large $n$ if $c \geq \frac{1}{2}$. Hence, by induction, we have shown that $T(n) = \mathcal{O}(n^2)$.

Moreover, since $T(n) = T(n - 1) + n$ expands to the sum of the first $n$ integers $T(n) = 1 + 2 + \dots + n$ $= \frac{n(n + 1)}{2}$, we also know that $T(n) = \Omega(n^2)$. Thus, we conclude that $T(n) = \Theta(n^2)$.

<br/>

$\Large{\color{cornflowerblue}\text{4. Example 4}}$

Consider the recurrence:

$$\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = 2T(n/3) + \Theta(n) \space}$$

We guess that $T(n) = \mathcal{O}(n)$ and prove it by induction. To prove this with substitution, we show that $T(n) \leq c n$ for some constant $c > 0$ and sufficiently large $n$.

$\color{mediumorchid}{\text{4.1 First attempt}}$

Suppose we try the induction hypothesis (IH):

Assume that the hypothesis holds for all values smaller than $n$, i.e., $T(k) \leq c k$ for all $k < n$.

Substituting into the recurrence:

$$
\begin{align*}
T(n) & = 2T(n/3) + \Theta (n) \\
& \leq 2 \left( c \space \frac{n}{3} \right) + \Theta (n) \quad \text{(IH)} 
\end{align*}
$$

This is not strong enough: the $\Theta(n)$ term prevents us from concluding that $T(n) \leq c n$ for any fixed choice of $c$.

$\color{mediumorchid}{\text{4.2 Strengthening the IH}}$

We strengthen the induction hypothesis by subtracting a lower-order term (see the trick explained in CLRS4, 4.3). We will try to prove that $T(n) \leq c n - d$ for some constants $c, d > 0$ and sufficiently large $n$.

$\color{mediumorchid}{\text{4.2.1 Base Case}}$

For small values such as $n = 3$, the recurrence yields $T(3) = 2T(1) + \Theta(3)$. Since $T(1)$ is finite, we can adjust $d$ and increase $c$ if necessary to ensure that $T(3) \leq 3c - d$.

$\color{mediumorchid}{\text{4.2.2 Inductive Step}}$

Induction hypothesis (IH):
Assume that the strenghthened hypothesis holds for all values smaller than $n$, i.e., $T(k) \leq c k - d$ for all $k < n$.

Then:

$$
\begin{align*}
T(n) & \leq 2(c (n/3) - d) + \Theta(n) \quad \text{(IH)} \\
& = \frac{2cn}{3} - 2d + \Theta(n)
\end{align*}
$$

Let the hidden constant in $\Theta(n)$ be $k > 0$, so that $\Theta(n) \leq k n$ for sufficiently large $n$.

Then:

$$
\begin{align*}
T(n) & \leq \frac{2cn}{3} - 2d + k n \\
& = n \left( \frac{2c}{3} + k \right) - 2d
\end{align*}
$$

To complete the inductive step, we need to show that $T(n) \leq c n - d$.
This will be true if:

$$n \left( \frac{2c}{3} + k \right) - 2d \leq c n - d$$ 

or equivalently if:

$$n \left( c - \frac{2c}{3} - k \right) \geq d$$

To satisfy $T(n) \leq c n - d$, it suffices that:

$$c - \frac{2c}{3} - k > 0 \implies c > 3k$$
 
First pick $c > 3k$. Then, for sufficiently large $n$, the inequality holds automatically, and you can enlarge $d$ if necessary to ensure that the base cases hold.

Hence, by induction, we have shown that $T(n) = \mathcal{O}(n)$.

<br/>

$\Large{\color{cornflowerblue}\text{5. Example 5}}$

Consider the recurrence (for $n > 2$):

$$\color{saddlebrown}\boxed{\color{rosybrown}\space T(n) = T(n - 1) + T(n - 2) + 1 \space}$$

with finite base values T(0) and T(1) > 0.

We will prove a lower bound $T(n) = \Omega(\phi^n)$, where $\phi = \frac{1 + \sqrt{5}}{2} \approx 1.618$ is the golden ratio that satisfies $\phi^2 = \phi + 1$.

$\color{mediumorchid}{\text{5.1 Base Case}}$

Pick $n = 2$.

Then: $T(2) = T(1) + T(0) + 1$. Choose any constant $c > 0$ small enough so that $T(0) \geq c \phi^0 = c$ and $T(1) \geq c \phi^1 = c\phi$ (which is always possible since $T(0)$, $T(1)$ are fixed positives). Then $T(2) \geq c\phi + c + 1 \geq c \phi^2$, so the base case holds.

$\color{mediumorchid}{\text{5.2 Inductive Step}}$

Induction hypothesis (IH):
Assume that the hypothesis holds for all arguments smaller than $n$, i.e., $T(k) \geq c \phi^k$ for all $k < n$.

Then:

$$
\begin{align*}
T(n) & = T(n - 1) + T(n - 2) + 1 \\
& \geq c \phi^{n - 1} + c \phi^{n - 2} + 1 \quad \text{(IH)} \\ 
& = c (\phi^{n - 1} + \phi^{n - 2}) + 1 \\
& = c \phi^n + 1 \quad \text{(since } \phi^{n - 1} + \phi^{n - 2} = \phi^n \text{)}\\
& = c \phi^n 
\end{align*}
$$

This recurrence is essentially the Fibonacci recurrence with a small additive constant. The Fibonacci numbers themselves grow like $\phi^n$, which is why the golden ratio appears in this proof.

By induction, we have shown that $T(n) = \Omega(\phi^n)$.






















