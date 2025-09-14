$\huge{\color{Cadetblue}\text{AVL Trees}}$

<br/>

Just like [red-black trees](../rbtrees/README.md), AVL trees are the first type of self-balancing binary search trees to be invented. They were introduced in 1962 by Adelson-Velsky and Landis and are named after them. In order to keep trees balanced, AVL trees use ${\color{peru}\text{node height}}$ as a balancing factor instead of color, where the height of a node is defined as the number of edges on the longest path from the node to a leaf, or equivalently, the maximum subtree height of the node's children plus one.

AVL trees maintain the binary search tree property, and in addition, satisfy the following ${\color{peru}\text{AVL properties}}$:

1. The height of the left and right subtrees of any node differ by at most 1.
2. The height of an empty tree is -1.
3. The height of a leaf node is 0.
4. The height of a node is the maximum height of its left and right subtrees plus 1.

AVL trees are height balanced binary search trees, meaning that if at any time the height of the left and right subtrees of a node differ by more than one, the tree is rebalanced by performing rotations. These rotations can be of two types: left rotation or right rotation, and are performed to maintain the height balance property of the tree after an insertion or deletion operation. Which rotation to perform depends on the balance factor of the node, which is the difference between the height of the left and right subtrees of the node. If the balance factor is greater than 1, a right rotation is performed, and if it is less than -1, a left rotation is performed. If the balance factor is between -1 and 1, the tree is already height balanced, and no rotation is needed.

<br/>

$\Large{\color{darkseagreen}\text{Playlist}}$  

[![AVL trees](https://img.youtube.com/vi/DB1HFCEdLxA/0.jpg)](https://www.youtube.com/watch?v=DB1HFCEdLxA&list=PL9xmBV_5YoZOUFgdIeOPuH6cfSnNRMau-)
