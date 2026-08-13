// https://training.olinfo.it/task/ois_pancakes

#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <vector>

// constraints
#define MAXN 100000
#define MAXL 999999

using namespace std;

// input data
int N, B, P, D2, D3, D5, D7;
int L[MAXN]; // value constraint: [1 - 999'999] (included)

// defined by me globally
// is_prime[i] has:
// 'true' if is_prime[i] is prime
// 'false' if is_prime[i] is not prime

// note: when declared, every number is 'true', will be
// adjusted as the "sieve" runs

// MAXL + 1 because each value goes from 1 to 999'999
// + 1 is used for the cell [0]
// by doing this we can see if the number "i" is 
// prime byb checking is_prime[i]
vector<bool> is_prime(MAXL + 1, true);

// it calculates the score of a specific label
int score(int label)
{
	int points = 0;
	
	// standard price (B) for a pancake
    points += B;
	
	// 1st criteria - (number prime)
	if(is_prime[label])
		points += (label + P);
		
	// 2nd, 3rd and 4th criteria
	// 2nd: each digit prime
	// 3rd: sum of all digits prime
	// 4th: product of all digits prime
	int digit, sum_digits = 0, prod_digits = 1;
	while(label > 0)
	{
		digit = label % 10;
		label /= 10;
		
		// 2nd
		if(digit == 2)
			points += D2;
		else if(digit == 3)
			points += D3;
		else if(digit == 5)
			points += D5;
		else if(digit == 7)
			points += D7;
			
		// 3rd - summing
		sum_digits += digit;
		
		// 4th - multiplying
		prod_digits *= digit;
	}
	
	// 3rd
	if(is_prime[sum_digits])
		points += sum_digits;
		
	// 4th
	if(is_prime[prod_digits])
		points += prod_digits;
	
	return points;
}

// it finds the best possible lable changing "up to 1 digit"

// note1: since the value constraint is 1 <= Li <= 999'999
// there are at maximum 54 possible outcomes
// 9 for each digit (except the most significant that can't be 0)
// for a total of 53, plus when the 'label' doesn't change, 54!
// note2: it's 9 each because there are 9 new digits to substitute
// the current one
int what_label(int label)
{
	// variables containing the 'highest values possible'
	// at the start, they value 'label' given in input
	int final_label = label;
	int final_score = score(label);
	
	// aux variables
	int new_label;
	int temp = label;
    int divider = 1;

	// it compares every possible digit and checks what's the
	// highest score and saves the best label
    while (temp > 0) 
	{
        int original_digit = temp % 10;
        int left_side = label / (divider * 10);
        int right_side = label % divider;

        // changing the current digit with every other digits (from 0 to 9)
        // changing each digit once
        for (int new_digit = 0; new_digit <= 9; new_digit++) 
		{
            
            // 1. The most significant digit can't be 0
            if (left_side == 0 && new_digit == 0) 
			{
                continue; 
                // continue means jumps to the next iteration avoiding this current one
            }

            // 2. The digit mus be DIFFERENT from the original one
            // (altrimenti il numero non cambia, o cambierebbero zero cifre)
            if (new_digit == original_digit) 
			{
                continue; 
                // continue means jumps to the next iteration avoiding this current one
            }

            // Recreating the number changing ONE digit
            new_label = left_side * (divider * 10) + (new_digit * divider) + right_side;
            
            // comparing values - looking for the maximum score
			if(score(new_label) > final_score)
			{
				final_label = new_label;
				final_score = score(new_label);
			}

        }
        
        temp /= 10;
        divider *= 10;
    }
	
	return final_label;
}

// sieve of Eratosthenes used to locate all the prime numbers from 1 to n
// stores everything in "is_prime"
void sieve(int n) 
{
    is_prime[0] = is_prime[1] = false; // 0 and 1 are not prime
    
    for(int p = 2; p * p <= n; p++)
    	if(is_prime[p])
    		for(int i = p * p; i <= n; i+=p)
	    		is_prime[i] = false;
}

int main() 
{
	// fast input
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

    cin >> N;
    
    for(int i=0; i<N; i++)
        cin >> L[i];
        
    cin >> B >> P;
    cin >> D2 >> D3 >> D5 >> D7;

	// price can be super heavy and big! int doesn't work
    long long price = 0;
    // finding all the prime numbers
    sieve(MAXL);
    
    // for each label
    for(int i = 0; i < N; i++)
    {
    	// 1. pass the label to "what_label" to find the label
    	// 2. we determine the score of it
    	// 3. we sum it into price
    	price += score(what_label(L[i]));
    	
    	// 4. we move to the next label
	}
    
    cout << price;
}

/* The idea is to:
1. calculate all the prime numbers to avoid doing it once at a time
also we can then find them in O(1). They're found using the sieve of
Eratosthenes and stored in the vector 'is_prime'. 

The numbers range from 1 to 999'999 which is MAXL. 

A MAXL+1 size is used for the vector to allow us the usage of the 
index 'i' as a direct correlation to the number of which the score 
is being calculated. We occupy the [0] cell with "false" and the
rest is used as [i] to find what the number "i" is.

2. for each label:
	2.1 use a cycle to find the maximum score for the 60 possibilities of
	the labels

	2.2 sums the maximum score into "price" and goes to the next one
*/
