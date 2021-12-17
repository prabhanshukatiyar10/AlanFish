#include <bits/stdc++.h>
using namespace std;

int main()
{
	set<int> x = {2,4,3,1,5};
	for(int a:x)
		cout << a << " ";
	cout << endl;
	*(x.find(3))=9;

	for(int a:x)
		cout << a << " ";
	cout << endl;

}