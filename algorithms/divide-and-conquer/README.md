$\huge{\color{Cadetblue}\text{Divide-and-Conquer}}$ 

<br/>

$\Large{\color{Rosybrown}\text{Overview}}$

| ${\color{peru}\text{CLRS}}$ | ${\color{peru}\text{Topic}}$ |
|:---|:---|
| 4.1 [ed3] | [Maximum Subarray](https://github.com/pl3onasm/CLRS/tree/main/algorithms/divide-and-conquer/max-subarray) |
| 4.2 | [Strassen's Matrix Multiplication](https://github.com/pl3onasm/CLRS/tree/main/algorithms/divide-and-conquer/strassen)|
| – | [Karatsuba's Integer Multiplication](https://github.com/pl3onasm/CLRS/tree/main/algorithms/divide-and-conquer/karatsuba)|
| Ex 2.3 | [Binary Search](https://github.com/pl3onasm/CLRS/tree/main/algorithms/divide-and-conquer/binsearch)|
| Prob 2-4 | [Inversion Count](https://github.com/pl3onasm/CLRS/tree/main/algorithms/divide-and-conquer/inversion-count)|
| 9.2-3 | [Quickselect](https://github.com/pl3onasm/CLRS/tree/main/algorithms/divide-and-conquer/quickselect)|
| 33.4 [ed3]| [Closest Pair of Points](https://github.com/pl3onasm/CLRS/tree/main/algorithms/divide-and-conquer/closest-pair-of-points)|
| 2.3 | [Merge Sort](https://github.com/pl3onasm/CLRS/tree/main/algorithms/sorting/merge-sort)|
| 7.1-3 | [Quicksort](https://github.com/pl3onasm/CLRS/tree/main/algorithms/sorting/quick-sort)|
| 30 | [Fast Fourier Transform](https://github.com/pl3onasm/CLRS/tree/main/algorithms/fft)|

<br/>

$\Large{\color{Rosybrown}\text{The D-and-C Approach}}$

When a problem is too difficult to solve directly, it is often possible to attack the problem by dividing it into subproblems that are themselves smaller instances of the same problem and then solving these subproblems ${\color{peru}\text{recursively}}$. Such an approach is known as *divide and conquer*, and it is typically described by a ${\color{peru}\text{recurrence relation}}$, which expresses the solution to a problem in terms of solutions to smaller instances of the same problem.

A divide-and-conquer algorithm consists of three steps at each level of the recursion:

1. ${\color{Mediumorchid}\text{Divide}}$ the problem into a number of subproblems that are smaller instances of the same problem.
2. ${\color{Mediumorchid}\text{Conquer}}$ the subproblems by solving them recursively.
3. ${\color{Mediumorchid}\text{Combine}}$ the solutions to the subproblems into a solution for the original problem.

After sufficiently many levels of recursion, the recursion bottoms out and the subproblems become so small that they can be solved directly. As recursion unwinds, the solutions to the subproblems are then combined to give a solution to the original problem.

Note that if the subproblems are not independent (i.e. solutions to subproblems depend on solutions to other subproblems), the divide-and-conquer approach is not suitable, since the same subproblem would be solved multiple times. In such cases, [dynamic programming](https://github.com/pl3onasm/Algorithms/tree/main/algorithms/dynamic-programming) is the recommended approach.
