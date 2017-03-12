#include <iostream>
#include <string>
#include "mycalc.h"

using namespace std;

int main(int argc, char* argv[])
{
	AbstractSyntaxTreeNode<double>* temp;
	string str;
	while (true)
	{
		
	
		cin >> str;

		build(temp, str, 0, str.length());

		cout << temp->get_value() << endl;
	
	}
	return 0;
}
