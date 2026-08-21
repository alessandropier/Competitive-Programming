# Rubabandiera V2

_Link to the Training Olinfo problem [here](https://training.olinfo.it/task/ctf)._\
_Source code_:
1. Math Solution [here](<./method 1 (math).cpp>)
2. Bit Manipulation Solution [here](<./method 2 (bit manipulation).cpp>)

## Overview
1. **Game Setup**: $N$ players are arranged in a circle.
2. **Rules**: Over $N-1$ turns, each active player eliminates the first remaining player to their left (stealing their flag) and passes the turn to the next active player.
3. **Goal**: Determine the starting position (the winning spot) for a given number of players $N$ so that Edoardo always wins, across $Q$ test cases where $N$ can be as large as $10^{18}$

### Example with $N = 5$ Players
1. **Turn 1**: Player 1 eliminates player 2. (Remaining players: 1, 3, 4, 5)
2. **Turn 2**: Player 3 eliminates player 4. (Remaining players: 1, 3, 5)
3. **Turn 3**: Player 5 eliminates player 1. (Remaining players: 3, 5)
4. **Turn 4**: Player 3 eliminates player 5. (Player 5 is eliminated)
5. **Winner**: Player 3.

### Example with $N = 6$ Players
1. **Turn 1**: Player 1 eliminates player 2.
2. **Turn 2**: Player 3 eliminates player 4.
3. **Turn 3**: Player 5 eliminates player 6.
4. **Turn 4**: Player 1 eliminates player 3 (since players 2 and 4 are already out, player 1 is the first one to the left of player 5, who just completed the previous turn).
5. **Turn 5**: Player 5 eliminates player 1.
6. **Winner**: Player 5. 

This is the famous [**Josephus problem**](https://www.youtube.com/watch?v=uCsD3ZGzMgE).

## Solution

Solving manually the first 16, it is obvious that there is a sort of pattern:
1. Even number cannot win
2. If the number of players $N$ is a power of $2$, the winner is the player $1$
3. If the number of players $N$ is not a power of $2$, how can we determine the winner?

<div align="center">

| N (Players) | Binary Representation | Winner |
| :---: | :---: | :---: |
| 1 | `0b1` | **1** |
| 2 | `0b10` | **1** |
| 3 | `0b11` | 3 |
| 4 | `0b100` | **1** |
| 5 | `0b101` | 3 |
| 6 | `0b110` | 5 |
| 7 | `0b111` | 7 |
| 8 | `0b1000` | **1** |
| 9 | `0b1001` | 3 |
| 10 | `0b1010` | 5 |
| 11 | `0b1011` | 7 |
| 12 | `0b1100` | 9 |
| 13 | `0b1101` | 11 |
| 14 | `0b1110` | 13 |
| 15 | `0b1111` | 15 |
| 16 | `0b10000` | **1** |
</div>

The number of players $N$ can be written as $N = 2^a + L$ while $2^a \le N$

After $L$ players are eliminated, there are $2^a$ players left. Since $2^a$ is a power of $2$ the winner is <u>*whoever starts*</u> as shown previously; therefore, the winner of the game when there are $N$ players could be found by **using that** $L$.

Each number of players $N$ could be represented as $2^a + L$

<div align="center">

| Decomposition of $N$ | Remainder ($L$) | Winner ($N$) |
| :---: | :---: | :---: |
| $5 = 2^2 + 1$ | $L = 1$ | $Winner(N) = 3$ |
| $7 = 2^2 + 3$ | $L = 3$ | $Winner(N) = 7$ |
| $11 = 2^3 + 3$ | $L = 3$ | $Winner(N) = 7$ |
| $14 = 2^3 + 6$ | $L = 6$ | $Winner(N) = 13$ |
</div>

Is there a way to use the remainder $L$ to calculate the initial position of the winning player?

$$
\text{Winner}(N) = 2 * L + 1
$$

## Implementation 1 (math)

```cpp
long long solve(long long N)
{
	// find the greatest power of 2 less than equal to N
	int exp = 1;
	while(exp * 2 <= N)
		exp = exp * 2;
	
	// winner is (2 * L + 1)
	return 2 * (N - exp) + 1;
}
```

The first **while** loop finds the greatest power of 2 less than equal to $N$ (the $2^a$ power).

After that, $L$ is calculated by doing $N - exp$ and the final formula is $2 * L + 1$.

## Implementation 2 (bit manipulation)

Since it is a simple math formula, it could be implemented by using **bit manipulation** as well. 

To do so, this idea is used: _if $2^a$ is removed from $N$ we are left with $L$; therefore, we can calculate the winner_. How do we remove the biggest power of $2$ from $N$? We consider the **binary representation** of the number of players $N$ and find the **MSB** (*most significant bit*).

**Example with** $N = 7$ <br>
if $N = 7$, in binary is $00000111$ and the MSB is in position $2$. To subtract that number ($2^2 = 4$) from $N = 7$, we turn off that bit. Now it becomes $L = 00000011 = 3$. 

To multiply by 2 a shift to the left is performed.
$3 = 00000011$ becomes $00000110 = 4 + 2 = 6$

Lastly, an $OR$ $1$ operation is performed to $add$ $1$.
$6 = 00000110$ OR $00000001 = 00000111 = 7$ which is the winner.

```cpp
long long solve(long long N)
{	
	int index = 63 - __builtin_clzll(N);

	N = N & ~(1LL << index);

	return (N << 1) | 1;
}
```
where:
1. The first instruction calculates the position of the MSB. $63$ is the number of possible positions of a **long long** variable and **__builtin_clzll** is a function that calculates the number of **leading zeros** of a long long. If these $2$ values are subtracted, we get the position of the MSB.

2. The second instruction creates a **long long** number that has a $1$ **only** in the position of the MSB $(1LL << index)$. After that, a $NOT$ operation is performed to get the opposite: everything is equal to $1$ except the MSB. Lastly, the resulting number is bitwise $ANDed$ with the initial number of players $N$. As a result,  _everything remains the same except the MSB that becomes $0$ because of the $AND$ operation_.

3. The last instruction is a multiplication by $2$ $(N << 1)$ and then we sum $1$ by doing $OR$ $1$. This **always** sums $1$ because the number is always **even** because it has been multiplied by $2$; therefore, the last bit is always $0$.

Moreover, it could be written in one single line like this:
```cpp
return (N & ~(1LL << (63 - __builtin_clzll(N)))) << 1 | 1; 
```