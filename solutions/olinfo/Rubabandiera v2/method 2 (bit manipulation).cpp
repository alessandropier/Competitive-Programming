#include <iostream>
using namespace std;

// bit implementation of the "L * 2 + 1" formula
long long solve(long long N)
{	
	// GOAL 1: reset the most significant bit (MSB) so that we are left w. "L" only
	// Step 1: find MSB position
	// LL numbers are made of 64 digits; therefore, the index goes from 0 to 63
	// es: N = 11 (binary: 00...01011) -> 8 + 3 | index = 63 - 60 = 3 which is the MSB position
	int index = 63 - __builtin_clzll(N); 
	
	// Step 2: reset the index position
	// n & ~(1 << x) flips the x-th bit in the number n
	// we re-use N to not waste memory but any LL variable would do
	N = N & ~(1LL << index);
	// a. 1LL << index: creates 1 as a LL and shifts the "1 bit" to the left by index positions
	// with index = 3 we get 000...000...0001000
	// b. ~(1LL << index): ~ negates everything getting 111...111...1110111
	// c. N & ~(1LL << index): N is in AND with a number of ALL 1s except for the   
	// 						   index-th bit (MSB) which is 0 so we reset it after the AND
	// es: N = 11 (00...01011) we get 111...111...1110111 AND
	//								  000...000...0001011 ===
	//								  000...000...0000011
	// which equals to 11 - 8 = 3, we're left w. L
	
	// GOAL 2: multiply "L times 2" (N << 1) and sum 1 (OR 1, | 1)
	// es: L = 3 = 000...000...0000011 after the shift (x 2) is
	// L = 3 * 2 = 6 = 000...000...00000110 OR
	// 								      1 ==
	//			   			    ...00000111
	// which equals to 1 + 2 + 4 = 7 (3 * 2 + 1)
	return (N << 1) | 1;
	
	// all at once is --> return (N & ~(1LL << (63 - __builtin_clzll(N)))) << 1 | 1; 
}

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	
	// no. of games
	int Q;
	// no. of players
	long long N;
	
	cin >> Q;
	while(Q--)
	{
		cin >> N;
		
		// winning position for N players
		cout << solve(N) << "\n";
	}
}
