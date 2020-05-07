#include <iostream>
#include <string>

#include <list>
#include <vector>
using namespace std;


struct entry {
	string name;
	int number;
};

int test()
{

	vector<entry> test(10);

	cout << test.size() << endl;

	test.resize(test.size() + 9);

	cout << test.size() << endl;
	return 0;
}
