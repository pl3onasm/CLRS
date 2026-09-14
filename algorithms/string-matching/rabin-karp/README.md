$\huge{\color{Cadetblue}\text{Rabin-Karp algorithm}}$  

$\huge{\color{Cadetblue}\text{(RKA)}}$

<br/>

$\Large{\color{Rosybrown}\text{Problem}}$

Given a pattern $P[0:m-1]$ and a text $T[0:n-1]$, find all occurrences of $P$ in $T$. It is assumed that $n \geq m$. The result should consist of the indices indicating where each occurrence of $P$ in $T$ starts, i.e. the valid shifts for $P$ in $T$.

<br/>

$\Large{\color{darkseagreen}\text{Rabin-Karp algorithm}}$

The Rabin-Karp algorithm avoids having to compare all the characters of $T[s:s+m-1]$ with $P$ at each shift $s$ by using a hash function. The idea is to precompute the hash value of $P$ and of $T_m$ (the $m$-character prefix of $T$), and then to recompute the hash value of $T[s:s+m-1]$ at each shift $s$ in constant time by using a ${\color{mediumorchid}\text{rolling hash function}}$[^1]. Only if the hash values of $P$ and $T[s:s+m-1]$ match do we compare their $m$ characters explicitly to determine whether the shift is valid or merely a ${\color{mediumorchid}\text{spurious hit}}$ (a false positive) caused by a ${\color{mediumorchid}\text{hash collision}}$ (two different strings having the same hash value).

The precomputation of the hash values of $P$ and $T_m$ can be done in $\mathcal{O}(m)$ time. The expected time complexity of the algorithm is then $\mathcal{O}(n+m)$ under the usual assumption that hash collisions and genuine matches requiring verification are sufficiently rare. The hash values of the substrings of $T$ are recomputed in constant time as the ${\color{darkkhaki}\text{text window}}$ of size $m$ slides to the right by one character at each shift $s$. The worst-case time complexity occurs when $T[s:s+m-1]$ has the same hash value as $P$ for all shifts $s$, since each such hash match requires an additional $\Theta(m)$-time character-by-character verification. In that case, the complexity of the algorithm degenerates to $\mathcal{O}((n-m+1)m)$, the same as that of the naive algorithm. For example, this happens when $P=a^m$ and $T=a^n$, so that every possible shift is a genuine match, or when the hash function is a poor one and produces too many collisions.

Fortunately, we can greatly reduce the probability of hash collisions by using a good hash function. For example, a merely ${\color{peru}\text{additive}}$ rolling hash function is definitely not good enough. Such a hash function represents a string as the sum of the values of its characters and recomputes the hash value by adding the value of the new character in the window and subtracting the value of the character that just left the window. This is a poor hash function because it is easy to find two strings that have the same hash value. A much better hash function is the ${\color{peru}\text{polynomial}}$ rolling hash function, which the Rabin-Karp algorithm uses. The hash value of a string $S$ of length $m$ is then computed as follows:

<br/>

$$
{\color{darkslateblue}\boxed{
\large
{\color{rosybrown}
h(S)=
\left(
\sum_{i=0}^{m-1} S[i]\cdot d^{m-1-i}
\right)
\bmod p
}
}}
$$

where:

- $d=|\Sigma|$ is the size of the alphabet
- $p$ is a sufficiently large prime modulus, chosen to help reduce the probability of hash collisions
- $S[i]$ is the value of the $i$-th character of the string $S$ taken from the alphabet $\Sigma$.

The precomputation of the hash values of $P$ and $T_m$ can be done in $\mathcal{O}(m)$ time by applying ${\color{darkkhaki}\text{Horner's rule}}$ to the above formula:

$$
h(S)
=
(((S[0]\cdot d+S[1])\cdot d+S[2])\cdot d
+\dots+S[m-1])
\bmod p
$$

After that, the hash value of the $m$-sized text window at each new shift $s+1$ can be recomputed in constant time as follows:

$$
h(T[s+1:s+m])
=
\bigl(
d\cdot(h(T[s:s+m-1])-T[s]\cdot k)
+T[s+m]
\bigr)
\bmod p
$$

where $k$ is precomputed as

$$
k=d^{m-1}\bmod p.
$$

The subtraction of $T[s]\cdot k$ is necessary to remove the contribution of the character that just left the window, and the addition of $T[s+m]$ adds the contribution of the new character that just entered the text window. The multiplication by $d$ shifts the remaining character contributions one position to the left in the polynomial representation. The hash values of $P$ and the new text window $T[s+1:s+m]$ can then be compared in constant time to check if the new shift is potentially valid. If the hash values match, then the algorithm proceeds to compare all the characters of $P$ with the text window $T[s+1:s+m]$ to verify if the match is indeed valid. Note that these hash values are reduced modulo $p$ at each step to keep them within a bounded range. If the resulting hash values are assumed to be approximately uniformly distributed modulo $p$, the probability of a collision is approximately $1/p$.

Implementation: [Rabin-Karp](https://github.com/pl3onasm/AADS/blob/main/algorithms/string-matching/rabin-karp/rka.c)

[^1]: A hash function $h:\Sigma^\ast\rightarrow\mathbb{Z}$ maps a string $s$ to an integer $h(s)$. A rolling hash function is one whose value can be recomputed in constant time as the text window of length $m$ slides to the right by one character.
