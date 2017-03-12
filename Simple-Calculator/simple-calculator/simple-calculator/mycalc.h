#pragma once

#include <string>
#include <cmath>
#include <sstream>

const double EPS = 1e-11;

enum NODE_TYPE
{
	NUMBER,
	PLUS,
	MINUS,
	MULTIPLY,
	DIVIDE,
	POWER,
	SIN,
	COS,
	TAN,
	IF,
};

NODE_TYPE mapping(char c)
{
	switch (c)
	{
	case '+':return PLUS;
	case '-':return MINUS;
	case '*':return MULTIPLY;
	case '/':return DIVIDE;
	case '^':return POWER;
	default:return NUMBER;
	}
}

int priority(NODE_TYPE type)
{
	switch (type)
	{
	case PLUS:
	case MINUS:
		return 1;
	case MULTIPLY:
	case DIVIDE:
		return 2;
	case POWER:
		return 3;
	default:
		return 10000;
	}
}

template <typename data_type>
class AbstractSyntaxTreeNode
{
public:

	AbstractSyntaxTreeNode()
	{
		type = NUMBER;
		child1 = child2 = child3 = nullptr;
		data = 0;
	}

	AbstractSyntaxTreeNode(const AbstractSyntaxTreeNode& ast_node):
		type(ast_node.type), child1(ast_node.child1), child2(ast_node.child2), child3(ast_node.child3), data(ast_node.data)
	{
	}

	// ReSharper disable once CppNonExplicitConvertingConstructor
	AbstractSyntaxTreeNode(const data_type& data_)
	{
		type = NUMBER;
		child1 = child2 = child3 = nullptr;
		data = data_;
	}

	typedef AbstractSyntaxTreeNode* ast_pointer;

	data_type get_value() const
	{
		switch (type)
		{
		case NUMBER:
			return data;
		case PLUS:
			return child1->get_value() + child2->get_value();
		case MINUS:
			return child1->get_value() - child2->get_value();
		case MULTIPLY:
			return child1->get_value() * child2->get_value();
		case DIVIDE:
			return child1->get_value() / child2->get_value();
		case POWER:
			return pow(static_cast<double>(child1->get_value()), static_cast<double>(child2->get_value()));
		case SIN:
			return sin(child1->get_value());
		case COS:
			return cos(child1->get_value());
		case TAN:
			return tan(child1->get_value());
		case IF:
			return (child1->get_value() < EPS && child1->get_value() > -EPS) ? child3->get_value() : child2->get_value();
		default:
			return 0;
		}
	}

	NODE_TYPE type;

	ast_pointer child1;

	ast_pointer child2;

	ast_pointer child3;

	data_type data;
};


template <typename data_type>
void clear(AbstractSyntaxTreeNode<data_type>* ast_node)
{
	if (ast_node->child1 != nullptr)
	{
		clear(ast_node->child1);
		delete ast_node->child1;
	}
	if (ast_node->child2 != nullptr)
	{
		clear(ast_node->child2);
		delete ast_node->child1;
	}
	if (ast_node->child3 != nullptr)
	{
		clear(ast_node->child4);
		delete ast_node->child1;
	}
}

template <typename data_type>
void build(AbstractSyntaxTreeNode<data_type>*& current_node, const std::string& str, int begin, int end)
{
	current_node = new AbstractSyntaxTreeNode<data_type>;

	int min_pri = 10000;
	int pos = -1;

	for (int i = begin; i < end; ++i)
	{
		if (str[i] == '(')
		{
			int temp = 1;
			int j;
			for (j = i + 1; j < end && temp; ++j)
			{
				if (str[j] == '(')
				{
					temp++;
				}
				if (str[j] == ')')
				{
					temp--;
				}
			}
			if (j == end && i==begin)
			{
				build(current_node, str, begin + 1, end - 1);
				return;
			}
			i = j;
		}
		if (str[i] >= '0' && str[i] <= '9')
		{
			continue;
		}
		NODE_TYPE type = mapping(str[i]);
		if (priority(type) < min_pri)
		{
			min_pri = priority(type);
			pos = i;
		}
	}

	if (pos != -1)
	{
		current_node->type = mapping(str[pos]);
		//current_node->child1 = new AbstractSyntaxTreeNode<data_type>;
		//current_node->child2 = new AbstractSyntaxTreeNode<data_type>;
		build(current_node->child1, str, begin, pos);
		build(current_node->child2, str, pos + 1, end);
	}

	else
	{
		if (str[begin] == '(' && str[end - 1] == ')')
		{
			begin++;
			end--;
		}
		data_type flag = 1;
		if (mapping(str[begin]) == MINUS)
		{
			flag = -1;
			begin++;
		}
		data_type temp;
		std::istringstream ss(str.substr(begin, end));
		ss >> temp;
		temp *= flag;
		current_node->type = NUMBER;
		current_node->data = temp;
	}
}
