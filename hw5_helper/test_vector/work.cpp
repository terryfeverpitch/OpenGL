#include <vector>
#include <iostream>

using namespace std;

vector<int> aa[10];

int main()
{
	for(int i = 0 ; i < 10 ; i++)
	{
		aa[i].push_back(i);
		cout << aa[i].pop_back;
	}
	
}