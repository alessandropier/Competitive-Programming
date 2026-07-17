# scrigni

_Link to the Training Olinfo problem [here](https://training.olinfo.it/task/ois_scrigni)._\
_Source code [here](https://github.com/alessandropier/Competitive-Programming/blob/main/solutions/olinfo/scrigni/scrigni.cpp)._

## Overview

The problem asks us to find the expected number of operations needed to open a set of chests. Every time a wrong chest is opened, the player receives an electric shock. 

### Examples

<div align="center">

| Input ($N$) | Expected Output |
| :---: | :---: |
| `3` | `1.5` |
| `4` | `3.0` |
</div>

### Scenario Explanation ($N = 3$)

Suppose there are 3 chests arranged in this order: **3, 1, 2**. William does not know the correct sequence, so he decides to pick a chest at random. 

1. He tries to open the leftmost chest (**3**). It is not chest number 1, so it won't open, and he gets an electric shock.
2. William opens the middle chest (**1**), which is the correct one.
3. He tries the leftmost chest again (**3**), getting a second electric shock.
4. He tries the rightmost chest (**2**), which is the last one remaining and opens correctly.
5. Finally, he opens the leftmost chest (**3**).

In this specific scenario, William got shocked **2 times**. 
> The problem asks: **on average, how many times will Super Marco get shocked before the correct order is found?**

## Solution

### 1. Average of Each Chest Pick:
The average of each chest pick is the average between the **best scenario** and the **worst scenario**. 

1. **Best Scenario**: gets 0 electric shocks
2. **Worst Scenario**: gets all the electric shocks, which is $n-1$, $n$ being the number of closed chests

Therefore, the average number of electric shocks for each chest pick is: $\frac{0+(n-1)}{2} = \frac{n-1}{2}$

### 2. Solving Steps:
Let's consider $4$ chests:

- $4$ chests left: there are $3$ ways to gets the electric shock, avg: $1.5$
- $3$ chests left: there are $2$ ways to gets the electric shock, avg: $1$
- $2$ chests left: there are $1$ ways to gets the electric shock, avg: $0.5$
- $1$ chests left: there are $0$ ways to gets the electric shock, avg: $0$

Therefore, summing all the averages, the result is: $1.5 + 1 + 0.5 + 0 = 3$

Summing all the singular averages $\frac{n-1}{2}$ is the equivalent of summing every number from $1$ to $n-1$ and dividing it by $2$: $$ \sum_{i=1}^{n} \frac{(n-i)}{2} $$

which could be rewrote as: $$\frac{1}{2} \sum_{i=1}^{n} (n-i)$$

### 3. Final Formula:
The last summation could be solved in $O(1)$ using the Gauss's Method:

$$S = \frac{n(n + 1)}{2}$$

where $n = n - 1$ _(the final number of the summation)_

$$\frac{1}{2} \sum_{i=1}^{n} (n-i) = \frac{1}{2} \frac{(n-1)(n-1+1)}{2} = \frac{1}{2} \frac{(n-1)n}{2} = \frac{n(n-1)}{4}$$

Therefore, the final formula is:

$$shocks = \frac{n(n - 1)}{4}$$