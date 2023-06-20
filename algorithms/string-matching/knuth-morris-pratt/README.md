# Knuth Morris Pratt Algorithm (KMP)

## Problem

The problem statement is the same as the one for the Rabin-Karp algorithm: given a text $T$ and a pattern $P$, find all the occurrences of $P$ in $T$. The output should be all indices showing where the pattern starts in the text (the so-called **shifts**). While the Rabin-Karp algorithm uses a rolling hash, the KMP algorithm uses a finite state machine.

## String matching automaton

The benefit of an automaton for string matching is that it examines each text character only once, taking constant time per character. Thus the matching time is linear in the length of the text. However, the preprocessing time needed to build the automaton is is in $O(m | \Sigma |)$, where $m$ is the length of the pattern and $|\Sigma|$ is the size of the alphabet.