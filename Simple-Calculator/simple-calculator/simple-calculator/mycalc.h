#pragma once

#include <string>
#include <cmath>
#include <vector>
#include <sstream>

const double EPS = 1e-11;

enum Node_Type
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
	IF_2,
	LEFT_BRACKET,
	RIGHT_BRACKET,
	DOT,
	NONE,
};

inline Node_Type type_mapping(char c)
{
	switch (c)
	{
	case '+':return PLUS;
	case '-':return MINUS;
	case '*':return MULTIPLY;
	case '/':return DIVIDE;
	case '^':return POWER;
	case '(':return LEFT_BRACKET;
	case ')':return RIGHT_BRACKET;
	case '?':return IF;
	case ':':return IF_2;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':return NUMBER;
	case '.':return DOT;
	default:return NONE;
	}
}

inline int priority(Node_Type type)
{
	switch (type)
	{
	case IF:
	case IF_2:
		return 1;
	case PLUS:
	case MINUS:
		return 2;
	case MULTIPLY:
	case DIVIDE:
		return 3;
	case POWER:
		return 4;
	default:
		return 10000;
	}
}

template <typename data_type>
class SyntaxElement
{
public:

	SyntaxElement()
	{
		type = NUMBER; 
		data = 0;
	}

	// ReSharper disable once CppNonExplicitConvertingConstructor
	SyntaxElement(char ch)
	{
		type = type_mapping(ch);
		data = 0;
	}

	// ReSharper disable once CppNonExplicitConvertingConstructor
	SyntaxElement(std::string str)
	{
		std::istringstream ss(str);
		ss >> data;
		type = NUMBER;
	}

	bool operator == (char ch) const
	{
		return type == type_mapping(ch);
	}

	bool operator == (Node_Type ntype) const
	{
		return type == ntype;
	}

	Node_Type type;
	data_type data;
};

inline void syntax_analysis_pretreatment(std::string& str)
{
	std::string temp_string = "(";
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (type_mapping(str[i]) != NONE)
		{
			temp_string += str[i];
		}
	}
	temp_string += ")";
	str = temp_string;
}

template<typename data_type>
std::vector<SyntaxElement<data_type>> convert_to_syntax_element_sequence(const std::string& str)
{
	std::vector<SyntaxElement<data_type>> result;

	for (int i = 0; i < str.length(); ++i)
	{
		if (type_mapping(str[i]) == NUMBER || type_mapping(str[i]) == DOT)
		{
			int dot_counter = 1;
			int flag = i;
			while (flag < str.length() && (type_mapping(str[flag]) == NUMBER || type_mapping(str[flag]) == DOT))
			{
				if (type_mapping(str[flag]) == DOT)
				{
					if (dot_counter)
					{
						dot_counter--;
					}
					else
					{
						std::cerr << "above one dots in a number" << std::endl;
					}
				}
				flag++;
			}
			result.push_back(SyntaxElement<data_type>(str.substr(i, flag - i)));
			i = flag - 1;
		}
		else if (type_mapping(str[i]) == MINUS && result[result.size() - 1].type == LEFT_BRACKET)
		{
			result.push_back(SyntaxElement<data_type>("-1"));
			result.push_back(SyntaxElement<data_type>('*'));
		}
		else
		{
			result.push_back(SyntaxElement<data_type>(str[i]));
		}
	}
	return result;
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
			return pow(static_cast<double>(child1->get_value()), 
				static_cast<double>(child2->get_value()));
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

	Node_Type type;

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


template<typename data_type>
void build(AbstractSyntaxTreeNode<data_type>*& current_node, const std::vector<SyntaxElement<data_type>>& vec, int begin, int end)
{
	current_node = new AbstractSyntaxTreeNode<data_type>;
	
	int min_pri = 10000;
	int pos = -1;

	for (int i = begin; i < end; ++i)
	{
		if (vec[i] == LEFT_BRACKET)
		{
			int temp = 1;
			int j;
			for (j = i + 1; j < end && temp; ++j)
			{
				if (vec[j] == LEFT_BRACKET)
				{
					temp++;
				}
				if (vec[j] == RIGHT_BRACKET)
				{
					temp--;
				}
			}
			if (j == end && i == begin)
			{
				build(current_node, vec, begin + 1, end - 1);
				return;
			}
			i = j;
		}
		if (vec[i].type == NUMBER)
		{
			continue;
		}
		Node_Type type = vec[i].type;
		if (priority(type) < min_pri)
		{
			min_pri = priority(type);
			pos = i;
		}
	}

	if (pos != -1)
	{
		if (vec[pos].type == IF)
		{
			int pos2 = -1;
			for (int i = pos + 1; i < end; ++i)
			{
				if (vec[i].type == IF_2)
				{
					pos2 = i;
					break;
				}
			}
			if (pos2 == -1)
			{
				std::cerr << "i think you need a ':' " << std::endl;
				return;
			}
			current_node->type = vec[pos].type;
			build(current_node->child1, vec, begin, pos);
			build(current_node->child2, vec, pos + 1, pos2);
			build(current_node->child3, vec, pos2 + 1, end);
			
		}
		else
		{
			current_node->type = vec[pos].type;
			build(current_node->child1, vec, begin, pos);
			build(current_node->child2, vec, pos + 1, end);
		}
	}

	else
	{
		if (vec[begin] == LEFT_BRACKET && vec[end - 1] == RIGHT_BRACKET)
		{
			begin++;
			end--;
		}
		current_node->type = NUMBER;
		current_node->data = vec[begin].data;
	}
}
