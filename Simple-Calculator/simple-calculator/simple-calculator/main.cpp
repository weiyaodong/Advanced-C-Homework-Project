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
		getline(cin, str);

		cout << get_value<double>(str) << endl;
		print_structure<double>(str);
	}
	return 0;
}
