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

		syntax_analysis_pretreatment(str);
		auto temp1 = convert_to_syntax_element_sequence<double>(str);
		build(temp, temp1, 0, temp1.size());

		cout << temp->get_value() << endl;
		print_structure(temp->child1, "");
	}
	return 0;
}
