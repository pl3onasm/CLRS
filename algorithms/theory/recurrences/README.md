$\huge{\color{Cadetblue}\text{Recurrence relations}}$

<br/>

This folder provides an overview of the major methods for solving recurrence relations, an important technique in the analysis of algorithms. Each method is introduced conceptually and illustrated through detailed examples.

---

$\Large{\color{darkseagreen}\text{Overview}}$

<br/>

A **recurrence relation** expresses the running time of a recursive algorithm in terms of the running time of smaller subproblems. For instance, many divide-and-conquer algorithms can be described by a relation of the form:

$$
T(n) = aT(n/b) + f(n)
$$

where:

- **a** is the number of subproblems,
- **b** is the factor by which the problem size is reduced in each recursive call, and  
- **f(n)** is the work done outside the recursive calls (the *driving function*).

Solving such recurrences allows us to predict the asymptotic growth of the algorithm’s time complexity. Different forms of recurrence require different analytical techniques, and the methods below cover the most common cases encountered in algorithm analysis.

---

$\Large{\color{darkseagreen}\text{Available methods}}$

<br/>

| Method | Description |
|:--|:--|
| [Iteration Method](iteration-method.md) | Expand or unroll the recurrence repeatedly until a pattern emerges, then sum the resulting terms directly. |
| [Substitution Method](substituion.md) | Prove an asymptotic bound by guessing the form of the solution and showing it satisfies the recurrence via induction. |
| [Recursion Tree Method](recursion-tree.md) | Visualize the recurrence as a tree of costs per level, then sum the work across all levels to find a closed-form expression. |
| [Master Method](master-method.md) | Apply a formulaic theorem that provides asymptotic bounds for divide-and-conquer recurrences of the form \(T(n)=aT(n/b)+f(n)\). |
| [Akra–Bazzi Method](akra-bazzi.md) | A generalization of the Master Method that applies to unbalanced recurrences and irregular subproblem sizes. |
