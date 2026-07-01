# Table of Contents
1. [Big O Notation](#1-big-o-notation)
    - 1.1 [list of notations](#11-list-of-notations)
    - 1.2 [choosing the approach based on input size](#12-choosing-the-approach-based-on-input-size)
2. [Next Topic](#2-next-topic)

---

## 1. Big O Notation
**Big O notation** is a mathematical way to describe how an algorithm’s performance _(usually time used)_ scales as the input size grows. It ignores exact execution times or hardware differences, focusing instead on the upper bound of the growth rate to help developers write scalable code.

### 1.1 List of Notations:

- **O(1) - Constant Time**: Performance remains the same regardless of input size. (e.g. accessing an item in an array by its index).
- **O(log n) - Logarithmic Time**: Performance scales slowly. As the input doubles, the number of operations only increases by a small, fixed amount. (e.g. binary search).
- **O(n) - Linear Time**: The number of operations scales directly in proportion to the input size. (e.g. searching for an unsorted item).
- **O(n log n) - Linearithmic Time**: Common in efficient sorting algorithms where each item is processed logarithmically. (e.g. merge sort, heap sort).
- **O(n<sup>2</sup>) - Quadratic Time**: The number of operations grows proportionally to the square of the input. As inputs get large, these algorithms slow down drastically. (e.g. bubble sort).
- **O(n<sup>c</sup>) - Polinomiale**: The number of operations grows proportionally to the input size raised to a constant power c (where c > 2, such as cubic time O(n<sup>3</sup>)). While still considered _"feasible"_ in theorety, these algorithms slow down heavily as inputs grow. (e.g. the naive three-nested-loops approach to the 3-Sum problem).
- **O(2<sup>n</sup>) - Exponential Time**: The number of operations doubles with every additional input. These algorithms become extremely inefficient very quickly. (e.g. solving the traveling salesperson problem via brute force).

> Using **Big O notation** is crucial when writing code, as it helps us determine whether a solution is feasible even before we start coding. If the code's complexity is O(2<sup>n</sup>) we already know there has to be a more efficient solution.

> **note**: obviously, some problems inherently require an exponential complexity. For instance, the _Towers of Hanoi_ problem requires an $O(2^n)$ time complexity because the sequence of steps itself is $2^n$ operations long.

### 1.2 Choosing the Approach Based on Input Size

Estimating the efficiency of a solution is one of the most common tricks used by programmers and in coding competitions. It's called **Target Run Time Analysis**. Modern computers perform about $10^8$ (100 million) operations per second in a single thread. Knowing that the maximum allowed execution time for an algorithm is usually 1 or 2 seconds, you can look at the maximum input size ($n$) and immediately understand the maximum complexity you can afford.

| Input Size ($n$) | Maximum Allowable Complexity | Typical Algorithms / Approaches |
| :--- | :--- | :--- |
| **$n \le 10$** to **$12$** | $O(n!)$ or $O(n^2 \cdot 2^n)$ | Permutations, brute-force search |
| **$n \le 20$** | $O(2^n)$ | Backtracking, Dynamic Programming with bitmasks |
| **$n \le 500$** | $O(n^3)$ | Matrix multiplication, Floyd-Warshall algorithm |
| **$n \le 5,000$** | $O(n^2)$ | Double nested for-loops, Bubble Sort, Insertion Sort |
| **$n \le 10^5$** to **$10^6$** | $O(n \log n)$ or $O(n)$ | Sorting (Merge/Quick Sort), Hash Maps/Sets, Two-pointers |
| **$n \le 10^8$** | $O(n)$ | A single, simple linear for-loop |
| **$n > 10^8$** | $O(\log n)$ or $O(1)$ | Binary Search, direct mathematical formulas, Bitwise operations |

Therefore, looking at the input size you can instantly determine which solutions are worth considering.

## 2. Next Topic