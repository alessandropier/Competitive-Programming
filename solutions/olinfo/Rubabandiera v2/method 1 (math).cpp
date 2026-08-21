#include <iostream>
using namespace std;

long long solve(long long N)
{
	// find the greatest power of 2 less than equal to N
	int exp = 1;
	while(exp * 2 <= N)
		exp = exp * 2;
	
	// winner is (2 * L + 1)
	return 2 * (N - exp) + 1;
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
