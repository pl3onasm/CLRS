$\huge{\color{Cadetblue}\text{Knuth Morris Pratt}}$  

$\huge{\color{Cadetblue}\text{Algorithm (KMP)}}$

<br/>

$\Large{\color{Rosybrown}\text{Problem}}$

Given a pattern $P[0:m-1]$ and a text $T[0:n-1]$, find all occurrences of $P$ in $T$. It is assumed that $n \geq m$. The result should consist of the indices indicating where each occurrence of $P$ in $T$ starts, i.e. the valid shifts for $P$ in $T$.

<br/>

$\Large{\color{darkseagreen}\text{The KMP algorithm}}$

If we look at the [string-matching automaton](https://github.com/pl3onasm/CLRS/tree/main/algorithms/string-matching/finite-automata), it is clear that the preprocessing step to compute $\delta$ is the most expensive part of the algorithm. In fact, it does more preprocessing than necessary, since it computes a transition for every state and every possible character in the alphabet $\Sigma$, resulting in a table containing $\Theta(|\Sigma| \cdot m)$ entries, even though during the matching process only one transition is needed for each character of $T$. Furthermore, the construction does not exploit the internal structure of the pattern $P$ itself to avoid unnecessary work. Instead, it brute-forces a complete transition table for all states and all characters in the alphabet.

This is where the KMP algorithm comes in: it does not compute an exhaustive transition function $\delta$ in advance, but instead computes the transitions on the fly during the matching process, by using a precomputed ${\color{peru}\text{prefix function }\pi}$ $\space : \space \lbrace 0, 1, \dots, m \rbrace \rightarrow \lbrace 0, 1, \dots, m \rbrace$, which explicitly captures the self-overlap structure of $P$ and uses it to determine how far the pattern can be shifted after a mismatch. It maps a state $q$ to the length of the longest prefix of $P$ that is also a ${\color{cornflowerblue}\text{proper}}$ suffix of $P_q$. In other words, it tells us how the pattern matches against shifts of itself, thus guiding the overall matching process by indicating how far the pattern can be shifted in case of a mismatch. This prefix function is defined as follows:

<br/>

$$
{\color{maroon}\boxed{
\begin{aligned}
{\color{peru}\pi(0)} &= 0,\\
{\color{peru}\pi(q)} &= {\color{darkgoldenrod}\max}
\lbrace k \space : \space 0 \leq k < q \land P_k \sqsupset P_q \rbrace,
\qquad q > 0.
\end{aligned}
}}
$$

<br/>

In case of a mismatch, the algorithm will then shift the pattern $P$ by $q - \pi(q)$ characters to the right, where $q$ is the current state of the matching process, so that the current character of $T$ is next compared with $P[\pi(q)]$. This is possible because the prefix function $\pi$ tells us that the first $\pi(q)$ characters of $P$ are also the last $\pi(q)$ characters of $P_q$, so that these first $\pi(q)$ characters do not have to be compared again. If the comparison is successful, the state is incremented by one, and the next character of $T$ is compared with the updated $P[q]$ (which is now the next character of $P$). If the state reaches $m$, we have found an occurrence of $P$ in $T$ and the shift is added to the result. The state then falls back to $\pi(m)$, which allows overlapping occurrences to be found. If a comparison fails, the pattern is shifted again according to the prefix function, until there is a match or the state reaches $0$.  

Thus, the matching process actually simulates a finite automaton with $m+1$ states, where the ${\color{rosybrown}\text{transition function}}$ is computed on the fly and can be characterized as follows: [^1]

$$
{\color{rosybrown}\delta(q, a) = }
\begin{cases}
q + 1 & \scriptsize \text{if } q < m \land a = P[q] \\
{\color{peru}q'} + 1 & \scriptsize \text{if } a = P[q'] \\
{\color{peru}q'} & \scriptsize \text{otherwise}
\end{cases}
$$

where $q'$ is the largest fallback state in the prefix chain whose next pattern character matches $a$. It can be defined as:

$$
{\color{peru}q' = }
\begin{cases}
\max \lbrace k \space : \space k \in {\color{darkkhaki}\pi^\ast(q)} \land P[k] = a \rbrace \\
\qquad \scriptsize \text{if } \exists k \in {\color{darkkhaki}\pi^\ast(q)} \space : \space a = P[k] \\
0 \quad\space\space \scriptsize \text{otherwise}
\end{cases}
$$

[^1]: Since we are working with 0-based indices (contrary to the book), $P[q]$ is in fact the $(q+1)$-st character of $P$. Prefix function values, just like state values, pertain to string lengths, not indices. Thus, there is always a difference of 1 when switching between state or prefix function values and string indices.

The ${\color{darkkhaki}\text{iterated prefix function } \pi^\ast}$ generates the set of all possible fallback states, by recursively applying the prefix function to the current state until the obtained state is $0$. It can be defined as:

$$
{\color{darkkhaki}\pi^\ast(q) = }
\begin{cases}
\lbrace 0 \rbrace & \scriptsize \text{if } \pi(q) = 0 \\
\lbrace \pi(q) \rbrace \cup \pi^\ast(\pi(q)) & \scriptsize \text{otherwise}
\end{cases}
$$

The set $\pi^\ast(q)$ is only a mathematical description of the possible fallback states. The implementation does not construct this set or search it explicitly. Instead, it follows the prefix links $q := \pi(q)$ one at a time. Although several such fallbacks may occur for a single text character, their total number over the complete matching process is $\mathcal{O}(n)$.

The prefix function can be computed in $\Theta(m)$ time and the matching process takes $\Theta(n)$ time. Hence the total running time of the algorithm is $\Theta(m+n)$, which is $\Theta(n)$ under the assumption $n \geq m$.

Implementation: [KMP Algorithm](https://github.com/pl3onasm/AADS/blob/main/algorithms/string-matching/knuth-morris-pratt/kmp.c)
