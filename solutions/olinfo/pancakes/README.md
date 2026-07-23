# pancakes

_Link to the Training Olinfo problem [here](https://training.olinfo.it/task/ois_pancakes)._\
_Source code [here](https://github.com/alessandropier/Competitive-Programming/blob/main/solutions/olinfo/pancakes/pancakes.cpp)._

## Overview

The problem asks us to find the final price of $N$ pancakes following some rules to **maximize** its value. The rules are:

- The starting price of a pancake is $B$ euro.
- If the label’s value $Li$ is **prime**, the price is **increased** by $Li + P$ _(primeness bonus)_ euro.
- For each digit $d$ of the label: if $d$ is prime (that is, if it’s either $2$, $3$, $5$, or $7$) then the price of the pancake will be increased by a corresponding fixed amount ($D2$, $D3$, $D5$, or $D7$ euro).
- If the sum of all digits $\sum d$ of the label is prime, the pancake’s price is increased by $\sum d$ euro.
- If the product of all digits $\prod d$ of the label is prime, the pancake’s price is increased by $\prod d$ euro.

### Example

<div align="center">

| Input | Output |
| :---: | :---: |
| `4`<br>`12 89 941 101`<br>`5 7`<br>`8 5 3 9` | **1907** |
</div>

### Explanation

In the first sample case, we can do these changes:
* First label: $12 \to 17$, for a price of: $45$.
* Second label: stays $89$, for a price of: $118$.
* Third label: $941 \to 991$, for a price of: $1022$.
* Fourth label: $101 \to 701$, for a price of: $722$.

The sum of all prices, after the changes are done, is $1907$.

## Solution

### 1. Sieve of Eratosthenes:
The `sieve` function is used to predetermine all prime numbers up to `MAXL` (999,999). This allows checking if a number, or the sum/product of its digits, is prime in $O(1)$ time via the `is_prime` boolean vector.

```cpp
void sieve(int n) 
{
    is_prime[0] = is_prime[1] = false; // 0 and 1 are not prime
    
    for(int p = 2; p * p <= n; p++)
    	if(is_prime[p])
    		for(int i = p * p; i <= n; i+=p)
	    		is_prime[i] = false;
}
```

### 2. Scoring Function (`score`): 
Calculates the score of a single label by summing all the criteria listed above.

### 3. CORE - Variant Exploration (`what_label`): 
**For each label**, the function analyzes all possible configurations obtained by modifying a **single digit** (up to _54 combinations_). Any option where the most significant digit becomes zero is discarded.

#### 3.1 Why 54 Combinations?
Since the **label value** constraint is $1 ≤ Li ≤ 999'999$ there are at maximum $54$ possible outcomes:
- $9$ for each digit _(except the most significant digit that cannot be $0$)_
- $8$ for the most significant digit _(zero is excluded)_
- the current label number _(when the label is not changed)_

For a total of $9*5 + 8 + 1 = 54$

#### 3.2 How the Code Implements the Exploration
To check all the variants efficiently, the function uses a combination of **mathematical operations** and **loops**:
* **Digit Extraction and Splitting**: Through the `while (temp > 0)` loop, each digit is processed individually by splitting the number into three parts using powers of $10$ (`divider`):
  * **`left_side`**: The digits to the **left** of the **current one**.
  * **`original_digit`**: The **current digit** being modified.
  * **`right_side`**: The digits to the **right** of the **current one**.

```cpp
int temp = label;
int divider = 1;
...
int original_digit = temp % 10;
int left_side = label / (divider * 10);
int right_side = label % divider;
...
...
divider *= 10;
```

* **Substitution Loop**: A nested `for` loop **replaces** the **current digit** with **every value** from $0$ to $9$. Two safety conditions skip invalid or redundant cases:
  * If `left_side == 0` and `new_digit == 0`, it **prevents leading zeros**.
  * If `new_digit == original_digit`, it **skips unchanged digits**.

```cpp
if (left_side == 0 && new_digit == 0)
    continue;
```

* **Score Maximization**: The new label is reconstructed mathematically (`new_label`) and its score is evaluated. Then `final_label` is updated whenever a higher score is found.

```cpp
new_label = left_side * (divider * 10) + (new_digit * divider) + right_side;
if(score(new_label) > final_score)
    {
        final_label = new_label;
        final_score = score(new_label);
    }
```